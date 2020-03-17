#ifndef _SYSTEM_DEFS_H_
#define _SYSTEM_DEFS_H_

#define SYSTEM_CLOCK_FREQ (50000000)

#define SYSTEM_IO_BASE 0xFF210000
#define SYSTEM_IO_ADDRSPACE_SIZE 0x10000

#define SYSTEM_IO_UART_BASE (SYSTEM_IO_BASE + 0x200)
#define SYSTEM_IO_UART_CHIP_BASE(chipnum) (SYSTEM_IO_UART_BASE + ((chipnum - 1) << 4))

#define HEX7SEG_0_AND_1 (*(volatile unsigned int *)(0xFF200030))
#define HEX7SEG_2_AND_3 (*(volatile unsigned int *)(0xFF200040))
#define HEX7SEG_4_AND_5 (*(volatile unsigned int *)(0xFF200050))
#define PUSH_BUTTONS    (*(volatile unsigned int *)(0x00000010))

#endif