#include <stdio.h>
#include <stdlib.h>

#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Hardware/Touch.h"
#include "../Display/Colours.h"
#include "../Display/Fonts.h"
#include "../Global.h"

void start_tests()
{
	printf("RUN THE TESTS!!!!!\n");
}

/**
 * Draw start test button, requires user input
 */
void draw_test_screen(void)
{
	draw_filled_button(250, 550, 100, 380, "Start Tests", LIME, BLACK);
}

void test_screen(void)
{
	clear_screen();
	draw_test_screen();
	while(1) {
	  Point touch = get_touch();

	  if (touch.x >= 250 && touch.x <= 550 && touch.y >= 100 && touch.y <= 380) {
		  loading_screen("Running tests...");
		  start_tests();
		  keep_loading = 0;

		  clear_screen();
		  draw_test_screen();
		  return;
	  }
	}
}
