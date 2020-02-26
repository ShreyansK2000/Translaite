#include <stdlib.h>

#include "../Display/Colours.h"
#include "Screens.h"
#include "../Display/Fonts.h"
#include "../Global.h"
#include "../Hardware/Touch.h"
#include "../Display/Graphics.h"
#include "../Translations.h"

#define NUM_LANGUAGES 5

const char * languages[][NUM_LANGUAGES] = {{"English", "French", "Spanish", "German", "Italian"},
							  {"Anglais", "Francais", "Espagnol", "Allemand", "Italien"},
							  {"Ingles", "Frances", "Espanol", "Aleman", "Italiano"},
							  {"Englisch", "Franzosisch", "Spanisch", "Deutsche", "Italienisch"},
                              {"Inglese", "Francese", "Spagnolo", "Tedesco", "Italiano"}};

const char * translation_languages[] = {"Translation language:", "Langue de traduction:", "Languaje de tradiccion:", 
										"Ubersetzungssprache:", "Lingua di traduzione:"};
const char * native_languages[] = {"Native language:", "Langue maternelle:", "Lengua materna:", "Muttersprache:",
									 "Lingua nativa:"};
const char * main_menu_trans[] = {"Main menu", "Menu principal", "Menu principal", "Hauptmenu", "Menu principale"};

/**
 * Draw all the language screen options, main menu button
 * 
 * Target language buttons on top row
 * Native language buttons on bottom row
 */ 
void draw_language_screen()
{
	int i;

	for (i = 0; i < NUM_LANGUAGES; i++) {
		if (i == target_language) {
			draw_filled_button(i*145+50, i*145+170, 100, 200, languages[native_language][i], WHITE, BLACK);
		} else {
			draw_filled_box(i*145+50, i*145+170, 100, 200, BLACK);
			draw_hollow_button(i*145+50, i*145+170, 100, 200, languages[native_language][i], BLACK, WHITE, WHITE);
		}

		if (i == native_language) {
			draw_filled_button(i*145+50, i*145+170, 300, 400, languages[i][i], WHITE, BLACK);
		} else {
			draw_filled_box(i*145+50, i*145+170, 300, 400, BLACK);
			draw_hollow_button(i*145+50, i*145+170, 300, 400, languages[i][i], BLACK, WHITE, WHITE);
		}
	}

	draw_filled_box(50, 400, 50, 70, BLACK);
	write_word(50, 50, translation_languages[native_language], BLACK, WHITE, MEDIUM);

	draw_filled_box(50, 400, 250, 270, BLACK);
	write_word(50, 250, native_languages[native_language], BLACK, WHITE, MEDIUM);

	draw_filled_box(500, 750, 25, 75, BLACK);
	draw_hollow_button(500, 750, 25, 75, main_menu_trans[native_language], BLACK, WHITE, WHITE);
}

/**
 * Select native and target languages based on screen touch coordinates
 * 
 * @param touch Point struct containing touch coordinates on language screen
 */ 
void change_language(Point touch)
{
	// Touch top row event (Target language)
	if (touch.y >= 100 && touch.y <= 200) {
		if (touch.x >= 50 && touch.x <= 170) {
			target_language = ENGLISH;
		} else if (touch.x >= 195 && touch.x <= 315) {
			target_language = FRENCH;
		} else if (touch.x >= 340 && touch.x <= 460) {
			target_language = SPANISH;
		} else if (touch.x >= 485 && touch.x <= 605) {
			target_language = GERMAN;
			draw_language_screen();
		} else if (touch.x >= 630 && touch.x <= 750) {
			target_language = ITALIAN;
		}
	} 
	
	// Touch bottom row event (Native language)
	else if (touch.y >= 300 && touch.y <= 400) {
		if (touch.x >= 50 && touch.x <= 170) {
			native_language = ENGLISH;
		} else if (touch.x >= 195 && touch.x <= 315) {
			native_language = FRENCH;
		} else if (touch.x >= 340 && touch.x <= 460) {
			native_language = SPANISH;
		} else if (touch.x >= 485 && touch.x <= 605) {
			native_language = GERMAN;
		} else if (touch.x >= 630 && touch.x <= 750) {
			native_language = ITALIAN;
		}
	} 
	
	// Do nothing, out of bounds
	else {
		return;
	}

	draw_language_screen();
}

void language_screen()
{
	clear_screen();
	draw_language_screen();
	Point touch;
	while(1) {
		touch = get_touch();
		// Press main menu event
		if (touch.x >= 500 && touch.x <= 750 && touch.y >= 25 && touch.y <= 75) {
			return;
		}
		// Any other touch on screen
		else {
			change_language(touch);
		}
	}
}
