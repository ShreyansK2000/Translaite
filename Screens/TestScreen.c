#include <stdio.h>
#include <stdlib.h>

#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Hardware/Touch.h"
#include "../Display/Colours.h"
#include "../Display/Fonts.h"
#include "../Global.h"
#include "../Test/Test.h"

/**
 * Draw start test button, requires user input
 */
void draw_result_screen(int passed)
{
	clear_screen();
	if (passed) {
		draw_filled_box(0,800,0,480, LIME);
		write_word(50, 50, "PASSED ALL TESTS", LIME, BLACK, LARGE);
	} else {
		draw_filled_box(0,800,0,480, RED);
		write_word(50, 50, "FAILED TESTS", RED, BLACK, LARGE);
	}
	
}

/**
 * Draw start test button and return to user mode button, requires user input
 */
void draw_test_screen(void)
{
	draw_filled_button(250, 550, 100, 380, "Start Tests", LIME, BLACK);
	draw_hollow_button(500, 750, 25, 75, "Application", BLACK, WHITE, WHITE);
}

/**
 * Execute all tests
 */
void start_tests()
{
	printf("Starting tests: \n");

	int passed = 1;

	passed &= test_parse_register();
	passed &= test_parse_login();
	passed &= test_parse_add_history();
	passed &= test_parse_remove_history();
	passed &= test_parse_open_sock();
	passed &= test_parse_location();
	passed &= test_parse_Q_to_Q();
	passed &= test_check_empty_array();
	passed &= test_parse_translation();
	passed &= test_parse_history();

	draw_result_screen(passed);
}

void test_screen(void)
{
	clear_screen();
	draw_test_screen();
	while(1) {
	  Point touch = get_touch();

	  // Start test touch event
	  if (touch.x >= 250 && touch.x <= 550 && touch.y >= 100 && touch.y <= 380) {
		  loading_screen("Running tests...");
		  start_tests();
		  keep_loading = 0;

		  get_touch();

		  clear_screen();
		  draw_test_screen();
	  }
	  // Return to gui
	  else if (touch.x >= 500 && touch.x <= 750 && touch.y >= 25 && touch.y <= 75) {
		  logo_screen();
		  return;
	  }

	}
}
