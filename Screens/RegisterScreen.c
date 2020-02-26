#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Display/Fonts.h"
#include "../Display/Colours.h"
#include "../Hardware/Touch.h"
#include "../Internet/WiFi.h"
#include "../Global.h"

char registration_username[1024] = "";
char registration_password[1024] = "";

void draw_register_screen()
{
	write_word(100, 70, "USERNAME:", BLACK, WHITE, LARGE);
	draw_bordered_box(100, 700, 100, 150, 5, WHITE, BLUE); // Username Input Box
	write_word(110, 110, registration_username, WHITE, BLACK, MEDIUM);

	// Hide password
	int i;
	char * hiddenPassword = malloc(sizeof(char)*64);
	strcpy(hiddenPassword, "");
	for(i = 0; i < strlen(registration_password); i++) {
		strcat(hiddenPassword, "*");
	}

	write_word(100, 170, "PASSWORD:", BLACK, WHITE, LARGE);
	draw_bordered_box(100, 700, 200, 250, 5, WHITE, BLUE); // Password Input Box
	write_word(110, 210, hiddenPassword, WHITE, BLACK, MEDIUM);
	free(hiddenPassword);

	draw_filled_button(265, 365, 300, 350, "Back", BLUE, BLACK); // Login Button
	draw_filled_button(385, 535, 300, 350, "Register", BLUE, BLACK); // Register Button
}

void register_screen()
{
	clear_screen();
	draw_register_screen();
	Point touch;
	while(1) {
		touch = get_touch();
		// Go back to login screen
		if (touch.x >= 265 && touch.x <= 365 && touch.y >= 300 && touch.y <= 350) {
			return;
		// Register button event
		} else if (touch.x >= 385 && touch.x <= 535 && touch.y >= 300 && touch.y <= 350) {
			if (strlen(registration_username) == 0) { // Don't allow empty username, so prompt user
				error_screen("Please enter a username");
				clear_screen();
				draw_register_screen();
				continue;
			}

			if (strlen(registration_password) == 0) { // Don't allow empty password, so prompt user
				error_screen("Please enter a password");
				clear_screen();
				draw_register_screen();
				continue;
			}

			enum register_response_state state = register_user(registration_username, registration_password);
			if (state == RegisterSuccess) {
				strcpy(username, registration_username);
				strcpy(password, registration_password);

				strcpy(registration_username, "");
				strcpy(registration_password, "");

				main_menu_screen();
				return;
			} else {
				char * msg = state == UserExists ? "Username already exists" : "Timed out waiting for server";
				error_screen(msg);
				clear_screen();
				draw_register_screen();
			}
		}
		// Username textbox touch event
		else if (touch.x >= 100 && touch.x <= 700 && touch.y >= 100 && touch.y <= 150) {
			keyboard_screen(registration_username);
			clear_screen();
			draw_register_screen();
		}
		// Password textbox touch event
		else if (touch.x >= 100 && touch.x <= 700 && touch.y >= 200 && touch.y <= 250) {
			keyboard_screen(registration_password);
			clear_screen();
			draw_register_screen();
		}
	}
}
