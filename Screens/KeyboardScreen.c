#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Display/Fonts.h"
#include "../Display/Colours.h"
#include "../Hardware/Touch.h"
#include "../Global.h"

#define LOW_INPUT_COUNT 20
#define MAX_INPUT_COUNT 30

#define KEY_WIDTH 70
#define MARGIN 50
#define TOP_ROW 220
#define MID_ROW 290
#define BOT_ROW 360
#define BOTTOM 430

const char *keysRowMajor[26] = {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
							 	"a", "s", "d", "f", "g", "h", "j", "k", "l",
							 	"z", "x", "c", "v", "b", "n", "m"};

int should_accept_erase(int wordLen) {
	return wordLen > 0;
}

int should_accept_input(int wordLen) {
	return wordLen < MAX_INPUT_COUNT;
}

/*
 * Update the max char count at the top right of the screen. Changes the colour of the font to indicate if
 * the user is reaching the maximum limit.
 */
void draw_char_count(int wordLen) {
	char* text = malloc(sizeof(char)*64);
	sprintf(text, "%d", wordLen);

	int colour;

	if (wordLen < LOW_INPUT_COUNT) {
		colour = LIME;
	} else if (wordLen < MAX_INPUT_COUNT) {
		colour = YELLOW;
	} else {
		colour = RED;
	}

	// TODO: Use some library to add the integer to this string instead of hardcoding 30 here
	text = strcat(text, "/30");

	write_word(MARGIN + KEY_WIDTH*9, MARGIN/2, text, BLACK, colour, LARGE);

	free(text);
}

/*
 * Update the user input field given the pointer to the string
 */
void draw_user_input(char * toPopulate)
{
	draw_bordered_box(MARGIN, MARGIN+KEY_WIDTH*10, TOP_ROW-2*KEY_WIDTH, TOP_ROW-KEY_WIDTH, 5, WHITE, RED);
	write_word(MARGIN+10, TOP_ROW-2*KEY_WIDTH + KEY_WIDTH/4 + 5, toPopulate, WHITE, BLACK, LARGE);
}

/*
 * Draws all the keys and special keys
 */
void draw_keyboard()
{
	int i;

	for (i = 0; i < 10; i++) {
		draw_hollow_button(MARGIN + KEY_WIDTH*i, MARGIN + KEY_WIDTH*(i+1), TOP_ROW, TOP_ROW + KEY_WIDTH, keysRowMajor[i], BLACK, WHITE, WHITE);
	}
	for (i = 0; i < 9; i++) {
		draw_hollow_button(MARGIN + KEY_WIDTH*i, MARGIN + KEY_WIDTH*(i+1), MID_ROW, MID_ROW + KEY_WIDTH, keysRowMajor[i+10], BLACK, WHITE, WHITE);
	}
	for (i = 0; i < 7; i++) {
		draw_hollow_button(MARGIN + KEY_WIDTH*i, MARGIN + KEY_WIDTH*(i+1), BOT_ROW, BOT_ROW + KEY_WIDTH, keysRowMajor[i+19], BLACK, WHITE, WHITE);
	}

	draw_hollow_button(MARGIN + KEY_WIDTH*7, MARGIN + KEY_WIDTH*9, BOT_ROW, BOT_ROW + KEY_WIDTH, "ENTER", BLACK, WHITE, WHITE);
	draw_hollow_button(MARGIN + KEY_WIDTH*9, MARGIN + KEY_WIDTH*10, MID_ROW, MID_ROW + KEY_WIDTH, "ERASE", BLACK, WHITE, WHITE);
	draw_hollow_button(MARGIN + KEY_WIDTH*9, MARGIN + KEY_WIDTH*10, BOT_ROW, BOT_ROW + KEY_WIDTH, "CANCEL", BLACK, WHITE, WHITE);
}

/*
 * Draw all the components of the keyboard screen (Keyboard, user input, character count)
 */
void draw_keyboard_screen(char * toPopulate)
{
	draw_keyboard();
	draw_user_input(toPopulate);
	draw_char_count(strlen(toPopulate));
}

void keyboard_screen(char * toPopulate)
{
	char* inputCopy = malloc(sizeof(char)*1024);
	strcpy(inputCopy, toPopulate);

	clear_screen();
	draw_keyboard_screen(toPopulate);
	Point touch;
	while (1) {
		touch = get_touch();
		int wordLen = strlen(toPopulate);
		// Top row touch event
		if (touch.y >= TOP_ROW && touch.y <= TOP_ROW + KEY_WIDTH) {
			// Characters event
			if (touch.x >= MARGIN && touch.x <= MARGIN + KEY_WIDTH * 10) {
				// Only accept input if within max character count
				if (should_accept_input(wordLen)) {
					toPopulate = strcat(toPopulate, keysRowMajor[(touch.x - MARGIN) / KEY_WIDTH]);
					draw_user_input(toPopulate);
					draw_char_count(wordLen + 1);
				}
			}
		}
		// Middle row touch event
		else if (touch.y >= MID_ROW && touch.y <= MID_ROW + KEY_WIDTH) {
			// Actual characters event
			if (touch.x >= MARGIN && touch.x <= MARGIN + KEY_WIDTH * 9) {
				// Only accept input if within max character count
				if (should_accept_input(wordLen)) {
					toPopulate = strcat(toPopulate, keysRowMajor[(touch.x - MARGIN) / KEY_WIDTH + 10]);
					draw_user_input(toPopulate);
					draw_char_count(wordLen + 1);
				}
			}
			// Erase button event
			else if (touch.x >= MARGIN + KEY_WIDTH * 9 && touch.x <= MARGIN + KEY_WIDTH * 10) {
				if (should_accept_erase(wordLen)) {
					toPopulate[strlen(toPopulate) - 1] = 0;
					draw_user_input(toPopulate);
					draw_char_count(wordLen - 1);
				}
			}
		} 
		// Bottom row touch event
		else if (touch.y >= BOT_ROW && touch.y <= BOT_ROW + KEY_WIDTH) {
			// Actual characters touch event
			if (touch.x >= MARGIN && touch.x <= MARGIN + KEY_WIDTH * 7) { 
				toPopulate = strcat(toPopulate, keysRowMajor[(touch.x - MARGIN) / KEY_WIDTH + 19]);
				draw_user_input(toPopulate);
				draw_char_count(wordLen+1);
			}
			// Enter button event
			else if (touch.x >= MARGIN + KEY_WIDTH * 7 && touch.x <= MARGIN + KEY_WIDTH * 9) {
				return;
			}
			// Cancel button event
			else if (touch.x >= MARGIN + KEY_WIDTH * 9 && touch.x <= MARGIN + KEY_WIDTH * 10) {
				// Restore the value of toPopulate string
				strcpy(toPopulate, inputCopy);
				free(inputCopy);
				return;
			}
		}
	}
}
