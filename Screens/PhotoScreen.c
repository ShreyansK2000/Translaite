#include <stdio.h>

#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Display/Fonts.h"
#include "../Display/Colours.h"
#include "../Hardware/Touch.h"
#include "../Internet/Wifi.h"
#include "../Internet/Parse.h"
#include "../Global.h"
#include "../Translations.h"

const char * return_button_trans[] = {"Return", "Revenir", "Regreso", "Ruckkehr", "Ritorno"};

void draw_photo_screen()
{
	//"Capture" button
	draw_hollow_circle(400, 425, 50, RED);
	draw_filled_box(380, 420, 405, 440, RED);
	draw_filled_box(405, 415, 395, 405, RED);
	draw_filled_circle(400, 425, 10, BLACK);

	//"Back" button
	draw_filled_button(0, 200, 0, 100, return_button_trans[native_language], RED, BLACK);
}

void photo_screen()
{
	clear_screen();
	draw_photo_screen();
	Point touch;
	while(1) {
		touch = get_touch();

		// Take photo button event
		if (touch.x >= 350 && touch.x <= 450 && touch.y >= 375 && touch.y <= 475) {
			loading_screen("GETTING TRANSLATIONS...");
			topLevelParsed * info = translate(languages[0][target_language], languages[0][native_language]);
			keep_loading = 0;

			if (info == NULL) {
				error_screen("Timed out waiting for server");
			} else {
				printf("%s\n", info->objects->val);
				translation_screen(info);
				free_struct(info);
			}

			clear_screen();
			draw_photo_screen();
		}
		// Return button event
		else if (touch.x >= 0 && touch.x <= 200 && touch.y >= 0 && touch.y <= 100) {
			return;
		}
	}
}
