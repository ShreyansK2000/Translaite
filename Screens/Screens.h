#ifndef SCREENS_H__
#define SCREENS_H__

#include "../Internet/Parse.h"

/**
 * The language selection screen.
 *
 * Lets the user pick their native language and target language from translation.
 */
void language_screen(void);

/**
 * The main menu screen.
 *
 * Lets the user choose if they want to take a photo, change their language, sign out of their account,
 * or view their history.
 */
void main_menu_screen(void);

/**
 * The photo request screen.
 *
 * TODO: Add a livestream of the camera attached to the Raspberry Pi.
 *
 * Lets the user click a button to send a request to the Raspberry Pi to take a photo and view translations.
 */
void photo_screen(void);

/**
 * The user history screen.
 *
 * Lets the user view all the translations they have made and saved. They can send requests to the server to listen
 * to the translations in the target language, and also remove translations from their history.
 */
void history_screen(historyObj * historyObj);

/**
 * The translation screen.
 *
 * Displays the translations of the objects detected by the server from the photo taken by the Raspberry Pi. This screen
 * allows the users to view the photo that was captured, with bounding boxes and annotations for detected objects (if any).
 * The user can also add translations to their user history if they wish.
 */
void translation_screen(topLevelParsed * info);

/**
 * The loading screen.
 *
 * Displays a string to indicate some kind of loading. This function spawns a new thread and polls a flag that indicates
 * if it should stop the loading screen and kill the thread.
 */
void loading_screen(char* loading_message);

/**
 * The log in screen.
 *
 * Allows the user to enter their username and password, or navigate to the registration screen upon bootup.
 */
void login_screen();

/**
 * The registration screen.
 *
 * Allows the user to enter a username and password, and register for a new account. They can also navigate back to the
 * login screen.
 */
void register_screen();

/**
 * The logo screen.
 *
 * Displays a BMP image on the touch screen. Waits for the user to touch the screen before returning.
 */
void logo_screen();

/**
 * The keyboard screen.
 *
 * Allows the user to type a word and populates the string that is passed in.
 */
void keyboard_screen(char * toPopulate);

/**
 * The image screen.
 *
 * Allows the user to view the image taken by the Raspberry Pi. The image contains bounding boxes and annotations.
 *
 * If the download_image flag is set, it will open a socket and read data from the Raspberry Pi. If not, it will use whatever
 * image is currently downloaded on the De1.
 */
void image_screen(int download_image);

/**
 * The error screen.
 *
 * Given an error message, displays a prompt with the error message. Waits for the user to close the prompt
 * before returning control.
 */
void error_screen(char * message);

/**
 * The test screen.
 *
 * A big green button to start the unit tests
 */
void test_screen(void);

#endif /* SCREENS_H__*/
