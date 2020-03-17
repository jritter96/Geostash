#include "touchscreen.h"

#include "../system_defs.h"
#include "ar1100_defs.h"

#include <time.h>



void Init_Touch(void) {
    // Set the UART chip's baud rate:
    const unsigned int divisorLatch = SYSTEM_CLOCK_FREQ / 9600 / 16;
    AR1100_UART_ADDR_LCR = 0x80;
    AR1100_UART_ADDR_DLL = divisorLatch & 0xFF;
    AR1100_UART_ADDR_DLM = divisorLatch >> 8;
    AR1100_UART_ADDR_LCR = 0x00;

    // Initialize the UART chip's Line Control Settings:
    // Set bit width of data to <eight>.
    AR1100_UART_ADDR_LCR = 0b11;

    // Activate the FIFO's:
    AR1100_UART_ADDR_FCR = 0b110;
    AR1100_UART_ADDR_FCR = 0;

    /*
    // Set the Touch Mode:
    AR1100_UART_ADDR_RBR = AR1100_SYNC;
    AR1100_UART_ADDR_RBR = 1 + 2; // num regs + 2
    AR1100_UART_ADDR_RBR = AR1100_CMD_WRITE;
    AR1100_UART_ADDR_RBR = 0x00;
    AR1100_UART_ADDR_RBR = AR1100_CFG_ADDR_TOUCHMODE;
    AR1100_UART_ADDR_RBR = AR1100_CFG_VAL_TOUCHMODE;
    */

    AR1100_UART_ADDR_IER = 0;

    // Enable Touch Reporting:
    AR1100_UART_ADDR_RBR = AR1100_SYNC;
    AR1100_UART_ADDR_RBR = 0x1;
    AR1100_UART_ADDR_RBR = AR1100_CMD_TOUCH_ENABLE;
}


bool ScreenTouched(void) {
    // check the line status register:
    return (AR1100_UART_ADDR_LSR & 0b1);
}


static Point WaitTouchReport(
        const enum AR1100_PenEdge desiredEdge,
        const unsigned int timeout,
        volatile bool *const timedOut) {
    Point report = { 0 };
    clock_t clockStart = clock();
    *timedOut = 0;

waitTouchReportRedo:
    // Wait for the screen to be touched in the desired way:
    while (!(ScreenTouched() && AR1100_UART_ADDR_THR == (0x80 | desiredEdge))) {
        if (clock() - clockStart > timeout * CLOCKS_PER_SEC) {
            *timedOut = 1;
            return report;
        }
    }
    // Did not time out.

    report.x  = (AR1100_UART_ADDR_THR);
    report.x |= (AR1100_UART_ADDR_THR << 7);
    report.y  = (AR1100_UART_ADDR_THR);
    report.y |= (AR1100_UART_ADDR_THR << 7);

    if (report.x >= TSCR_RES_NATIVE_BOUND || report.y >= TSCR_RES_NATIVE_BOUND) {
        goto waitTouchReportRedo;
    }

    // Scale the coordinate values to map to pixels.
    report.x = report.x * TSCR_RES_WIDTH  / TSCR_RES_NATIVE_BOUND;
    report.y = report.y * TSCR_RES_HEIGHT / TSCR_RES_NATIVE_BOUND;
    return report;
}



Point GetPress(const unsigned int timeout, volatile bool *const timedOut) {
    // wait for a pen down command then return the X,Y coord of the point
    // calibrated correctly so that it maps to a pixel on screen
    return WaitTouchReport(DOWN, timeout, timedOut);
}


Point GetRelease(const unsigned int timeout, volatile bool *const timedOut) {
    // wait for a pen up command then return the X,Y coord of the point
    // calibrated correctly so that it maps to a pixel on screen
    return WaitTouchReport(UP, timeout, timedOut);
}

bool isPointInRectangle(Point p, Rectangle r) {
    // Reminder: Values increase in the bottom-right direction.
    return (p.x >= r.x)
        && (p.x <= r.x + r.width)
        && (p.y >= r.y)
        && (p.y <= r.y + r.height);
}

