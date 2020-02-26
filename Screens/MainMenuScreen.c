#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Display/Fonts.h"
#include "../Display/Colours.h"
#include "../Hardware/Touch.h"
#include "../Global.h"
#include "../Internet/WiFi.h"

const char * takephotos_trans[] = {"Take photo", "Prendre une photo", "Tomar foto", "Foto machen", "Fare foto"};
const char * signout_trans[] = {"Sign out", "Deconnexion", "Desconectar", "Ausloggen", "Disconnessione"};
const char * history_trans[] = {"History", "Histoire", "Historia", "Geschichte", "Storia"};
const char * menu_languages[] = {"Languages", "Langages", "Idiomas", "Sprachen", "Le lingue"};

void draw_main_menu_screen()
{
	draw_filled_button(50, 375, 50, 440, takephotos_trans[native_language], RED, BLACK);

	draw_filled_button(400, 750, 50, 163, signout_trans[native_language], CYAN, BLACK);
	draw_filled_button(400, 750, 188, 301, history_trans[native_language], LIME, BLACK);
	draw_filled_button(400, 750, 326, 440, menu_languages[native_language], BLUE, BLACK);
}

void main_menu_screen()
{
	clear_screen();
	draw_main_menu_screen();
	Point touch;
	while(1) {
		touch = get_touch();
		// Photo screen button event
		if (touch.x >= 50 && touch.x <= 375 && touch.y >= 50 && touch.y <= 440) {
			photo_screen();
			clear_screen();
			draw_main_menu_screen();
		}
		// Sign out button event
		else if (touch.x >= 400 && touch.x <= 750 && touch.y >= 50 && touch.y <= 163) {
			// Erase user information and return to the logo screen
			strcpy(username, "");
			strcpy(password, "");
			return;
		}
		// User history button event
		else if (touch.x >= 400 && touch.x <= 750 && touch.y >= 188 && touch.y <= 301) {
			loading_screen("GETTING USER HISTORY...");
			historyObj * translations = get_user_history(username);
			keep_loading = 0;

			if (translations == NULL) {
				error_screen("Timed out waiting for server");
			} else {
				history_screen(translations);
				free_translation_history(translations);
			}

			clear_screen();
			draw_main_menu_screen();
		}
		// Language selection button event
		else if (touch.x >= 400 && touch.x <= 750 && touch.y >= 326 && touch.y <= 440) {
			language_screen();
			clear_screen();
			draw_main_menu_screen();
		}
	}
}
