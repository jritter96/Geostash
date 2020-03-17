/*
 * ColourPalletteData.c
 *
 *  Created on0xApr 12, 2015
 *      Author0xPaul
 */

// see --http://www.rapidtables.com/web/color/RGB_Color.htm for example
// colours
//
// Constants for each pallette number using the name for the colour
// can be found in the header file "Colours.h"
//
// this data represents the 24 bit RGB values of 256 colours. DE2 can
// display all 256 simultaneously, but DE1 can only display 64 at any
// one time. It should be setup in hardware in a ROM file in Quartus,
// but the software ('C' code) version of the data is also given below
// and can be used as a reference to the original data in graphics chip
// ROM.
//
// You should program the colour pallette (DE2 = 256, DE1 = 64) at the
// start, i.e in main() by calling ProgramPallette(BLACK, ColourPalletteData[0])
// for each colour and programming the colour(s) you want, 1 RGB value
// per pallette. BLACK is enumerated as 0 in Colours.h header file.
//
// You should include this as part of the project so it gets compiled,
// but if you want to reference it in multiple source file you should
// put an "extern" declaration in those source files
// e.g. `extern const unsigned int ColourPalletteData[256]`

const unsigned int ColourPalletteData[256] = {
0x00000000, // Black
0x00FFFFFF, // White
0x00FF0000, // Red
0x0000FF00, // Green/Lime
0x000000FF, // Blue
0x00FFFF00, // Yellow
0x0000FFFF, // Cyan
0x00FF00FF, // Magenta
0x00C0C0C0, // Silver
0x00808080, // Gray
0x00800000, // Maroon
0x00808000, // Olive
0x00008000, // Green
0x00800080, // Purple
0x00008080, // Teal
0x00000080, // Navy
0x00A52A2A, // Brown


0x00DC143C, // Crimson
0x00FF7F50, // Coral
0x00FA8072, // Salmon
0x00FF4500, // Orange Red
0x00FFA500, // Orange
0x00FFD700, // Gold
0x00DAA520, // Golden Rod
0x009ACD32, // Yellow Green
0x007FFF00, // Chart Reuse
0x00ADFF2F, // Green Yellow
0x0032CD32, // Lime Green
0x0000FF7F, // Spring Green
0x0000FFFF, // Aqua/Cyan
0x0040E0D0, // Turquoise
0x007FFFD4, // Aqua Marine
0x004682B4, // Steel Blue
0x0000BFFF, // Deep Sky Blue
0x001E90FF, // Dodger Blue
0x0087CEEB, // Sky Blue
0x004169E1, // Royal Blue
0x008A2BE2, // Blue Violet
0x004B0082, // Indigo
0x006A5ACD, // Slate Blue
0x00CF69CF, // Light Purple
0x00BA55D3, // Medium Orchid
0x00EE82EE, // Violet
0x00FF1493, // Deep Pink
0x00FF69B4, // Hot Pink
0x00FFC0CB, // Pink
0x008B4513, // Saddle Brown
0x00D2691E, // Chocolate
0x00D2B48C, // Tan


0x00708090, // Slate Gray
0x00696969, // Dim Gray
0x00A9A9A9, // Dark Gray
0x00D3D3D3, // Light Gray
0x00DCDCDC, // GainsBoro
0x00F5F5F5, // White Smoke
};
