#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Display/Fonts.h"
#include "../Display/Colours.h"
#include "../Hardware/Touch.h"
#include "../Internet/WiFi.h"
#include "../Internet/Parse.h"
#include "../Global.h"
#include "../Hardware/GPS.h"

#include "../Translations.h"

char username[1024] = "";
char password[1024] = "";

/**
 * Draws all the components on the login screen (
 */
void draw_login_screen()
{
	write_word(100, 70, "USERNAME:", BLACK, WHITE, LARGE);
	draw_bordered_box(100, 700, 100, 150, 5, WHITE, RED); // Username Input Box
	write_word(110, 110, username, WHITE, BLACK, MEDIUM);

	// Hide password
	int i;
	char * hiddenPassword = malloc(sizeof(char)*64);
	strcpy(hiddenPassword, "");
	for(i = 0; i < strlen(password); i++) {
		strcat(hiddenPassword, "*");
	}

	write_word(100, 170, "PASSWORD:", BLACK, WHITE, LARGE);
	draw_bordered_box(100, 700, 200, 250, 5, WHITE, RED); // Password Input Box
	write_word(110, 210, hiddenPassword, WHITE, BLACK, MEDIUM);
	free(hiddenPassword);

	draw_filled_button(265, 365, 300, 350, "Login", RED, BLACK); // Login Button
	draw_filled_button(385, 535, 300, 350, "Register", RED, BLACK); // Register Button
}

void login_screen()
{
	clear_screen();
	draw_login_screen();
	Point touch;
	while(1) {
		touch = get_touch();
		// Login button event
		if (touch.x >= 265 && touch.x <= 365 && touch.y >= 300 && touch.y <= 350) {

			if (strlen(username) == 0) { // Don't allow empty usernames, so prompt the user
				error_screen("Please enter a username");
				clear_screen();
				draw_login_screen();
				continue;
			}

			if (strlen(password) == 0) { // Don't allow empty passwords, so prompt the user
				error_screen("Please enter a password");
				clear_screen();
				draw_login_screen();
				continue;
			}

			enum login_response_state state = user_login(username, password);
			if (state == LoginSuccess) {

				// GPS fetch location, set language
				loading_screen("GPS values initializing. Please wait.");
				if (init_gps()) {
					native_language = get_country_from_coords(get_latitude(), get_longitude());
				}
				keep_loading = 0;

				main_menu_screen();
				return;
			} else {
				char * msg = state == UserDNE ? "Username does not exist" :
						(state == NULLLoginFailure ? "Timed out waiting for server" : "Incorrect password");

				error_screen(msg);
				clear_screen();
				draw_login_screen();
			}
		}
		// Register button event
		else if (touch.x >= 385 && touch.x <= 535 && touch.y >= 300 && touch.y <= 350) {
			register_screen();
			clear_screen();
			draw_login_screen();
		}
		// Username text box touch event
		else if (touch.x >= 100 && touch.x <= 700 && touch.y >= 100 && touch.y <= 150) {
			keyboard_screen(username);
			clear_screen();
			draw_login_screen();
		}
		// Password text box touch event
		else if (touch.x >= 100 && touch.x <= 700 && touch.y >= 200 && touch.y <= 250) {
			keyboard_screen(password);
			clear_screen();
			draw_login_screen();
		}
	}
}
