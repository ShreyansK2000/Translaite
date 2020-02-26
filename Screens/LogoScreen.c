#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Hardware/Touch.h"

/**
 * Draw the logo bitmap image pixel by pixel
 */ 
void draw_logo_screen()
{
	draw_bitmap_image("logo.bmp", "logo_palette.txt");
}

void logo_screen()
{
	clear_screen();
	draw_logo_screen();
	while(1) {
	  get_touch();
	  load_original_colour_palette();
	  login_screen();
	  clear_screen();
	  draw_logo_screen();
	}
}
