#include <stdio.h>
#include <string.h>

#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Display/Fonts.h"
#include "../Display/Colours.h"
#include "../Hardware/Touch.h"
#include "../Internet/Parse.h"
#include "../Global.h"
#include "../Translations.h"
#include "../Internet/WiFi.h"
#include "../Internet/Ethernet.h"

#define ENTRIES_PER_PAGE 3

const char * photos_trans[] = {"Photos", "Photos", "Fotos", "Fotos", "Fotografie"};

int should_download_image = 0;
int page_number_trans = 0;
int num_entries_trans = 0;
int num_pages_trans = 0;
int item_num = 0;

/**
 * Traverses the info linked list for the num'th node and returns it 
 */
objTransNode* go_to_info_node(topLevelParsed * info, int num) {
	int i;
	objTransNode* curNode = info->objects->head;
	for (i = 0; i < num; i++) {
		curNode = curNode->next;
	}
	return curNode;
}

/**
 * Draws a translation item on the screen. Will be positioned differently on the screen based on the
 * number of the item.
 */
void draw_translation_item(int item_num, topLevelParsed * info)
{
	int box_y1 = (100 * (item_num + 1) + item_num * 10);
	int box_y2 = 200 + (110 * (item_num));

	draw_bordered_box(5, 650, box_y1, box_y2, 3, WHITE, BLACK);
	objTransNode * item = go_to_info_node(info, page_number_trans * ENTRIES_PER_PAGE + item_num);
	
	centre_button_text(15, 280, box_y1 + 10, box_y1 + 50, item->native, WHITE, RED);
	centre_button_text(290, 570, box_y1 + 10, box_y1 + 50, info->nativeLanguage->val, WHITE, RED);

	centre_button_text(15, 280, box_y1 + 60, box_y2, item->translated, WHITE, LIME);
	centre_button_text(290, 570, box_y1 + 60, box_y2, info->targetLanguage->val, WHITE, LIME);

	draw_filled_box(580, 640, box_y1 + 20, box_y1 + 80, BLUE);
	draw_filled_box(590, 630, box_y1 + 45, box_y1 + 55, WHITE);
	draw_filled_box(605, 615, box_y1 + 30, box_y1 + 70, WHITE);
}

/**
 * Draws the components on the translation screen (list of items, menu button, photo button)
 */
void draw_translation_screen(topLevelParsed * info)
{
	draw_hollow_button(300, 450, 25, 75, photos_trans[native_language], BLACK, WHITE, WHITE); // Photo button
	draw_hollow_button(500, 750, 25, 75, return_button_trans[native_language], BLACK, WHITE, WHITE); // Return button

	// Draw the decrement page number button
	if(page_number_trans > 0) {
		draw_bordered_box(650, 750, 150, 250, 1, RED, BLACK);
		draw_triangle(700, 170, 680, 220, 720, 220, BLACK);
	}

	// Draw the increment page number button
	if(page_number_trans < (num_pages_trans - 1)) {
		draw_bordered_box(650, 750, 260, 360, 1, LIME, BLACK);
		draw_triangle(700, 340, 680, 280, 720, 280, BLACK);
	}
	
	// Draw the first translation item
	if (num_entries_trans > page_number_trans * ENTRIES_PER_PAGE) {
		draw_translation_item(0, info);
	}

	// Draw the second translation item
	if (num_entries_trans > page_number_trans * ENTRIES_PER_PAGE + 1) {
		draw_translation_item(1, info);
	}

	// Draw the third translation item
	if (num_entries_trans > page_number_trans * ENTRIES_PER_PAGE + 2) {
		draw_translation_item(2, info);
	}
}

/**
 * Event handler for pressing a translation item.
 */
void handle_touch_translation(int item_num, topLevelParsed * info, Point * touch)
{
	objTransNode * item = go_to_info_node(info, page_number_trans * ENTRIES_PER_PAGE + item_num);
	// Play audio touch event
	if (touch->x >= 5 && touch->x <= 570) { 
		loading_screen("PLAYING AUDIO...");
		play_audio(item->translated, info->targetLanguage->val);
		keep_loading = 0;

		clear_screen();
		draw_translation_screen(info);

	}
	// Add history touch event
	else if (touch->x >= 580 && touch->x <= 640) {
		// Save item item to user history
		if(add_to_history(username, info->nativeLanguage->val, info->targetLanguage->val, item->native, item->translated) == NULLAddFailure) {
			error_screen("Timed out waiting for server");
			clear_screen();
			draw_translation_screen(info);
		}
	}
}

void translation_screen(topLevelParsed * info)
{
	clear_screen();

	should_download_image = 1;
	page_number_trans = 0;
	num_entries_trans = info->objects->objectCount;
	num_pages_trans = (ENTRIES_PER_PAGE + num_entries_trans - 1) / ENTRIES_PER_PAGE;
	
	draw_translation_screen(info);
	Point touch;
	while(1) {
		touch = get_touch();
		
		// Up page event
		if (page_number_trans > 0 && touch.x >= 650 && touch.x <= 750 && touch.y >= 150 && touch.y <= 250) {
			page_number_trans--;
			clear_screen();
			draw_translation_screen(info);
		} 
		// Down page event
		else if (page_number_trans < (num_pages_trans - 1) && touch.x >= 650 && touch.x <= 750 && touch.y >= 260 && touch.y <= 360) {
			page_number_trans++;
			clear_screen();
			draw_translation_screen(info);
		}
		// Go to main menu
		else if (touch.x >= 500 && touch.x <= 750 && touch.y >= 25 && touch.y <= 75) {
			if (should_download_image) {
				get_file("192.168.0.200", 8080, "out.bmp", "palette.txt");
			}
			return;
		}
		// Display the BMP image
		else if (touch.x >= 300 && touch.x <= 450 && touch.y >= 25 && touch.y <= 75) {
			image_screen(should_download_image);
			should_download_image = 0;
			clear_screen();
			draw_translation_screen(info);
		}
		// Text to speech for first item
		else if (num_entries_trans > (page_number_trans * ENTRIES_PER_PAGE) && touch.y >= 100 && touch.y <= 200) {
			item_num = 0;
			handle_touch_translation(item_num, info, &touch);
		}
		// Text to speech for second item
		else if (num_entries_trans > (page_number_trans * ENTRIES_PER_PAGE + 1) && touch.y >= 210 && touch.y <= 310) {
			item_num = 1;
			handle_touch_translation(item_num, info, &touch);
		}
		// Text to speech for third item
		else if (num_entries_trans > (page_number_trans * ENTRIES_PER_PAGE + 2) && touch.y >= 320 && touch.y <= 420) {
			item_num = 2;
			handle_touch_translation(item_num, info, &touch);
		}
	}
}




