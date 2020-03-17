#ifndef _AR1100_DEFS_H_
#define _AR1100_DEFS_H_

#include "../16550uart_defs.h"


// =====================================
// Screen Resolution in PIXELS
// =====================================
#define TSCR_RES_HEIGHT 480
#define TSCR_RES_WIDTH  800
#define TSCR_RES_NATIVE_BOUND 4096


// =====================================
// UART ADDRESSES
// using chip #four
// =====================================
#define AR1100_UART_BASE (SYSTEM_IO_UART_CHIP_BASE(4))
#define AR1100_UART_ADDR_RBR _16550UART_ADDR_RBR(AR1100_UART_BASE)
#define AR1100_UART_ADDR_THR _16550UART_ADDR_THR(AR1100_UART_BASE)
#define AR1100_UART_ADDR_IER _16550UART_ADDR_IER(AR1100_UART_BASE)
#define AR1100_UART_ADDR_IIR _16550UART_ADDR_IIR(AR1100_UART_BASE)
#define AR1100_UART_ADDR_FCR _16550UART_ADDR_FCR(AR1100_UART_BASE)
#define AR1100_UART_ADDR_LCR _16550UART_ADDR_LCR(AR1100_UART_BASE)
#define AR1100_UART_ADDR_MCR _16550UART_ADDR_MCR(AR1100_UART_BASE)
#define AR1100_UART_ADDR_LSR _16550UART_ADDR_LSR(AR1100_UART_BASE)
#define AR1100_UART_ADDR_MSR _16550UART_ADDR_MSR(AR1100_UART_BASE)
#define AR1100_UART_ADDR_SCR _16550UART_ADDR_SCR(AR1100_UART_BASE)
#define AR1100_UART_ADDR_DLL _16550UART_ADDR_DLL(AR1100_UART_BASE)
#define AR1100_UART_ADDR_DLM _16550UART_ADDR_DLM(AR1100_UART_BASE)


#define AR1100_SYNC 0x55

/**
 * *-------*-----*-------*
 * | 7 6 5 | 4 3 | 2 1 0 |
 * | U U U | M M | D D D |
 * *-------*-----*-------*
 * 
 * meaning of value for each of U, M, D:
 * 000: nothing
 * 001: 0
 * 010: 1
 * 011: 1, 0
 * 100: 0, 1, 0
 * 101: 0, 1
 *
 * The below macro value is actually the device default.
 * It means that a DOWN event sends an UP report, then a
 * DOWN report. A MOVE event sends a DOWN report, and an
 * UP event sends and UP report.
 */
#define AR1100_CFG_VAL_TOUCHMODE 0b10110001
enum AR1100_PenEdge {
    UP   = 0,
    DOWN = 1,
};


// =====================================
// COMMAND ID'S (I)
// table 8-1
// =====================================
#define AR1100_CMD_VERSION          0x01
#define AR1100_CMD_RESET            0xF0
#define AR1100_CMD_INIT             0xF1
#define AR1100_CMD_WRITE            0xF2
#define AR1100_CMD_READ             0xF3
#define AR1100_CMD_FLUSH            0xF4
#define AR1100_CMD_WRITE_MODE       0xF5


// =====================================
// COMMAND ID'S (II)
// table 5-1
// =====================================
#define AR1100_CMD_TOUCH_ENABLE     0x12
#define AR1100_CMD_TOUCH_DISABLE    0x13
#define AR1100_CMD_CALIBRATE        0x14
#define AR1100_CMD_REG_READ         0x20
#define AR1100_CMD_REG_WRITE        0x21
#define AR1100_CMD_EE_READ          0x28
#define AR1100_CMD_EE_WRITE         0x29
#define AR1100_CMD_EE_READ_PARAMS   0x2B
#define AR1100_CMD_EE_WRITE_PARAMS  0x23


// =====================================
// STATUS ID'S
// table 5-2
// =====================================
#define AR1100_STAT_OK              0x00
#define AR1100_STAT_UNRECOGNIZED    0x01
#define AR1100_STAT_TIMEOUT         0x04
#define AR1100_STAT_EEPARAMS_ERR    0x05
#define AR1100_STAT_CAL_CANCEL      0xFC


// =====================================
// CONFIGURATION REGISTER ADDRESSES
// Modify using R/W command packets
// =====================================
#define AR1100_CFG_ADDR_TOUCHMODE   0x0C


#endif
