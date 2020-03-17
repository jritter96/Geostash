#ifndef _GRAPHICSHARDWARE_DEFS_
#define _GRAPHICSHARDWARE_DEFS_

// graphics register addresses
#define GraphicsCommandReg          (*(volatile unsigned short int *)(0xFF210000))
#define GraphicsStatusReg           (*(volatile unsigned short int *)(0xFF210000))
#define GraphicsX1Reg               (*(volatile unsigned short int *)(0xFF210002))
#define GraphicsY1Reg               (*(volatile unsigned short int *)(0xFF210004))
#define GraphicsX2Reg               (*(volatile unsigned short int *)(0xFF210006))
#define GraphicsY2Reg               (*(volatile unsigned short int *)(0xFF210008))
#define GraphicsColourReg           (*(volatile unsigned short int *)(0xFF21000E))
#define GraphicsBackGroundColourReg (*(volatile unsigned short int *)(0xFF210010))

/************************************************************************************************
** This macro pauses until the graphics chip status register indicates that it is idle
***********************************************************************************************/
#define WAIT_FOR_GRAPHICS       while((GraphicsStatusReg & 0x0001) != 0x0001);

// #defined constants representing values we write to the graphics 'command' register to get
// it to draw something. You will add more values as you add hardware to the graphics chip
// Note DrawHLine, DrawVLine and DrawLine at the moment do nothing - you will modify these
#define DrawHLine   1
#define DrawVLine   2
#define DrawLine    3
#define PutAPixel   0xA
#define GetAPixel   0xB
#define ProgramPaletteColour 0x10

#endif
