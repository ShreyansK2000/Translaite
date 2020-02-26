#include "Fonts.h"
#include "../Hardware/Touch.h"
#include "Graphics.h"

void print_character_medium_font(int x, int y, int colour, int backgroundcolour, int c, int Erase)
{
	register int 	row,
					column,
					theX = x,
					theY = y ;
	register int 	pixels ;
	register char 	theColour = colour  ;
	register int 	BitMask,
					theCharacter = c,
					theRow, theColumn;


    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))  // if start off edge of screen don't bother
        return ;

	if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {			// if printable character
		theCharacter -= 0x20 ;																			// subtract hex 20 to get index of first printable character (the space character)
		theRow = FONT2_YPIXELS;
		theColumn = FONT2_XPIXELS;

		for(row = 0; row < theRow ; row ++)	{
			pixels = Font10x14[theCharacter][row] ;		     								// get the pixels for row 0 of the character to be displayed
			BitMask = 512 ;							   											// set of hex 200 i.e. bit 7-0 = 0010 0000 0000
			for(column = 0; column < theColumn;   )  	{
				if((pixels & BitMask))														// if valid pixel, then write it
					write_pixel(theX+column, theY+row, theColour) ;
				else {																		// if not a valid pixel, do we erase or leave it along (no erase)
					if(Erase == 1)
						write_pixel(theX+column, theY+row, backgroundcolour) ;
					// else leave it alone
				}
					column ++ ;
				BitMask = BitMask >> 1 ;
			}
		}
	}
}

void print_character_large_font(int x, int y, int colour, int backgroundcolour, int c, int Erase)
{
	register int 	row,
					column,
					theX = x,
					theY = y ;
	register int 	pixels ;
	register char 	theColour = colour  ;
	register int 	BitMask,
					theCharacter = c,
					theRow, theColumn;


    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))  // if start off edge of screen don't bother
        return ;

	if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {			// if printable character
		theCharacter -= 0x20 ;																			// subtract hex 20 to get index of first printable character (the space character)
		theRow = FONT3_YPIXELS;
		theColumn = FONT3_XPIXELS;

		for(row = 0; row < theRow ; row ++)	{
			pixels = (Font16x27[theCharacter*54 + row*2] << 8) | Font16x27[theCharacter*54 + row*2 + 1];
			BitMask = 32768;							   											// set of hex 200 i.e. bit 7-0 = 0010 0000 0000
			for(column = 0; column < theColumn;   )  	{
				if((pixels & BitMask))														// if valid pixel, then write it
					write_pixel(theX+column, theY+row, theColour) ;
				else {																		// if not a valid pixel, do we erase or leave it along (no erase)
					if(Erase == 1)
						write_pixel(theX+column, theY+row, backgroundcolour) ;
					// else leave it alone
				}
					column ++ ;
				BitMask = BitMask >> 1 ;
			}
		}
	}
}

void print_character_giant_font(int x, int y, int colour, int backgroundcolour, int c, int Erase)
{
	register int 	row,
					column,
					theX = x,
					theY = y ;
	register long 	pixels ;
	register char 	theColour = colour  ;
	register long 	BitMask,
					theCharacter = c,
					theRow, theColumn;


    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))  // if start off edge of screen don't bother
        return ;

	if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {			// if printable character
		theCharacter -= 0x20 ;																			// subtract hex 20 to get index of first printable character (the space character)
		theRow = FONT4_YPIXELS;
		theColumn = FONT4_XPIXELS;

		for(row = 0; row < theRow ; row ++)	{
			pixels = (Font22x40[theCharacter*120 + row*3] << 16) | (Font22x40[theCharacter*120 + row*3 + 1] << 8) | Font22x40[theCharacter*120 + row*3 + 2];		     								// get the pixels for row 0 of the character to be displayed
			BitMask = 2097152;
			for(column = 0; column < theColumn;   )  	{
				if((pixels & BitMask))														// if valid pixel, then write it
					write_pixel(theX+column, theY+row, theColour) ;
				else {																		// if not a valid pixel, do we erase or leave it along (no erase)
					if(Erase == 1)
						write_pixel(theX+column, theY+row, backgroundcolour) ;
					// else leave it alone
				}
					column ++ ;
				BitMask = BitMask >> 1 ;
			}
		}
	}
}

void print_character_massive_font(int x, int y, int colour, int backgroundcolour, int c, int Erase)
{
	register int 	row,
					column,
					theX = x,
					theY = y ;
	register int 	pixels ;
	register char 	theColour = colour  ;
	register int 	BitMask,
					theCharacter = c,
					theRow, theColumn;


    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))  // if start off edge of screen don't bother
        return ;

	if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {			// if printable character
		theCharacter -= 0x20 ;																			// subtract hex 20 to get index of first printable character (the space character)
		theRow = FONT5_YPIXELS;
		theColumn = FONT5_XPIXELS;

		for(row = 0; row < theRow ; row ++)	{
			pixels = (Font38x59[theCharacter*236 + row*4] << 24) | (Font38x59[theCharacter*236 + row*4 + 1] << 16)
			| (Font38x59[theCharacter*236 + row*4 + 2] << 8) | Font38x59[theCharacter*236 + row*4 + 3] ;		     								// get the pixels for row 0 of the character to be displayed
			BitMask = 2147483648 ;
			for(column = 0; column < theColumn;   )  	{
				if((pixels & BitMask))														// if valid pixel, then write it
					write_pixel(theX+column, theY+row, theColour) ;
				else {																		// if not a valid pixel, do we erase or leave it along (no erase)
					if(Erase == 1)
						write_pixel(theX+column, theY+row, backgroundcolour) ;
					// else leave it alone
				}
					column ++ ;
				BitMask = BitMask >> 1 ;
			}
		}
	}
}
