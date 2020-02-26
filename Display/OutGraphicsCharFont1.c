#include "Fonts.h"
#include "../Hardware/Touch.h"
#include "Graphics.h"

/*************************************************************************************************
** This function draws a single ASCII character at the coord and colour specified
** it optionally ERASES the background colour pixels to the background colour
** This means you can use this to erase characters
**
** e.g. writing a space character with Erase set to true will set all pixels in the
** character to the background colour
**
*************************************************************************************************/
void print_character_small_font(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase)
{
// using register variables (as opposed to stack based ones) may make execution faster
// depends on compiler and CPU

	register int row, column, theX = x, theY = y ;
	register int pixels ;
	register char theColour = fontcolour  ;
	register int BitMask, theC = c ;

// if x,y coord off edge of screen don't bother

    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))
        return ;


// if printable character subtract hex 20
	if(((short)(theC) >= (short)(' ')) && ((short)(theC) <= (short)('~'))) {
		theC = theC - 0x20 ;
		for(row = 0; (char)(row) < (char)(7); row ++)	{

// get the bit pattern for row 0 of the character from the software font
			pixels = Font5x7[theC][row] ;
			BitMask = 16 ;

			for(column = 0; (char)(column) < (char)(5); column ++)	{

// if a pixel in the character display it
				if((pixels & BitMask))
					write_pixel(theX+column, theY+row, theColour) ;

				else {
					if(Erase == 1)

// if pixel is part of background (not part of character)
// erase the background to value of variable BackGroundColour

						write_pixel(theX+column, theY+row, backgroundcolour) ;
				}
				BitMask = BitMask >> 1 ;
			}
		}
	}
}
