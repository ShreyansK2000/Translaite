#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Hardware/Touch.h"
#include "../Display/Colours.h"
#include "../Display/Fonts.h"

/**
 * Draw error prompt, requires user input
 */ 
void draw_error_screen(char * message)
{
	draw_filled_box(100, 700, 100, 380, BLUE);
	write_word(120, 120, "ERROR", BLUE, WHITE, LARGE);
	draw_filled_button(120, 680, 170, 300, message, WHITE, BLACK);
	draw_filled_button(350, 450, 320, 360, "OK", WHITE, BLACK);
}

void error_screen(char * message)
{
	clear_screen();
	draw_error_screen(message);
	while(1) {
	  Point touch = get_touch();

	  if (touch.x >= 350 && touch.x <= 450 && touch.y >= 330 && touch.y <= 370) {
		  return;
	  }
	}
}
