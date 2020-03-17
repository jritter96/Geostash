#ifndef _16550UART_DEFS_H_
#define _16550UART_DEFS_H_


/**
 * See the documentation for the 16550 UART core.
 * These are properties of the chip.
 */
#define _16550UART_ADDR_RBR(base) (*(volatile unsigned char *)(base+0x0))
#define _16550UART_ADDR_THR(base) (*(volatile unsigned char *)(base+0x0))
#define _16550UART_ADDR_IER(base) (*(volatile unsigned char *)(base+0x2))
#define _16550UART_ADDR_IIR(base) (*(volatile unsigned char *)(base+0x4))
#define _16550UART_ADDR_FCR(base) (*(volatile unsigned char *)(base+0x4))
#define _16550UART_ADDR_LCR(base) (*(volatile unsigned char *)(base+0x6))
#define _16550UART_ADDR_MCR(base) (*(volatile unsigned char *)(base+0x8))
#define _16550UART_ADDR_LSR(base) (*(volatile unsigned char *)(base+0xA))
#define _16550UART_ADDR_MSR(base) (*(volatile unsigned char *)(base+0xC))
#define _16550UART_ADDR_SCR(base) (*(volatile unsigned char *)(base+0xE))
#define _16550UART_ADDR_DLL(base) (*(volatile unsigned char *)(base+0x0))
#define _16550UART_ADDR_DLM(base) (*(volatile unsigned char *)(base+0x2))


#endif
