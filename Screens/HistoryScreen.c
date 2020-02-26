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

/**
 * History page globals
 */ 
int page_number_history = 0;
int num_entries_history = 0;
int num_pages_history = 0;
int item_num_history = 0;

/**
 * Set the globals to correct values after item removals from pages
 * to ensure the user lands on the right page afterwards
 * 
 * @param historyObj historyObj struct containing number of items information
 * 					 with updated count after removal
 */ 
void refresh_globals(historyObj * historyObj)
{
	num_entries_history = historyObj->translationCount;
	page_number_history = (ENTRIES_PER_PAGE + num_entries_history - 1) / ENTRIES_PER_PAGE != num_pages_history ? page_number_history-1 : page_number_history;

	// prevent negative page number
	page_number_history = page_number_history < 0 ? 0 : page_number_history;

	num_pages_history = (ENTRIES_PER_PAGE + num_entries_history - 1) / ENTRIES_PER_PAGE;
}

/**
 * Travels to the required node in history by index
 * 
 * @param historyObj the historyObj struct pointing to head of linkedlist
 * @param num the index of the node to be returned
 * @return historyObjectNode of the appropriate index
 */ 
historyObjectNode* go_to_translation_node(historyObj * historyObj, int num)
{
	int i;
	historyObjectNode* curNode = historyObj->head;
	for (i = 0; i < num; i++) {
		curNode = curNode->next;
	}
	return curNode;
}

/**
 * Draw uniform boxes for items translated to display on each page,
 * max ENTRIES_PER_PAGE per item.
 * 
 * @param item_num The index of the item on the age
 * @param historyObj The object containing the information about translations
 */
void draw_history_item(int item_num, historyObj * historyObj)
{
	int box_y1 = (100 * (item_num + 1) + item_num * 10);
	int box_y2 = 200 + (110 * (item_num));

	draw_bordered_box(5, 650, box_y1, box_y2, 3, WHITE, BLACK);
	historyObjectNode * item = go_to_translation_node(historyObj, page_number_history * ENTRIES_PER_PAGE + item_num);
	
	centre_button_text(15, 280, box_y1 + 10, box_y1 + 50, item->native_word, WHITE, RED);
	centre_button_text(290, 570, box_y1 + 10, box_y1 + 50, item->native_language, WHITE, RED);

	centre_button_text(15, 280, box_y1 + 60, box_y2, item->target_word, WHITE, LIME);
	centre_button_text(290, 570, box_y1 + 60, box_y2, item->target_language, WHITE, LIME);

	draw_filled_box(580, 640, box_y1 + 20, box_y1 + 80, RED);
	draw_filled_box(590, 630, box_y1 + 45, box_y1 + 55, WHITE);
}

/**
 * Draw the main buttons and entries for the history screen
 * 
 * @param historyObj historyObj containing old translations
 */
void draw_history_screen(historyObj * historyObj)
{
	draw_hollow_button(500, 750, 25, 75, return_button_trans[native_language], BLACK, WHITE, WHITE);

	// Draw upwards arrow to go back 1 page
	if(page_number_history > 0) {
		draw_bordered_box(650, 750, 150, 250, 1, RED, BLACK);
		draw_triangle(700, 170, 680, 220, 720, 220, BLACK);
	}

	// Draws the downwards arrow to go forwards 1 page
	if(page_number_history < (num_pages_history - 1)) {
		draw_bordered_box(650, 750, 260, 360, 1, LIME, BLACK);
		draw_triangle(700, 340, 680, 280, 720, 280, BLACK);
	}
	
	// Draw the first translation item
	if (num_entries_history > page_number_history * ENTRIES_PER_PAGE) {
		draw_history_item(0, historyObj);
	}

	// Draw the second translation item
	if (num_entries_history > page_number_history * ENTRIES_PER_PAGE + 1) {
		draw_history_item(1, historyObj);
	}

	// Draw the third translation item
	if (num_entries_history > page_number_history * ENTRIES_PER_PAGE + 2) {
		draw_history_item(2, historyObj);
	}
}

/**
 * Function to handle touch events in certain regions of the screen
 * Controls audio requests, removal of translations
 * 
 * @param item_num_history item index on page
 * @param historyObj historyObj containing information about the translations
 * @param touch Pointer to the touch struct containing touch coordinates
 */
void handle_touch_history(int item_num_history, historyObj * historyObj, Point * touch)
{
	int node_idx = page_number_history * ENTRIES_PER_PAGE + item_num_history;
	historyObjectNode * item = go_to_translation_node(historyObj, node_idx);
	if (touch->x >= 5 && touch->x <= 570) {
		play_audio(item->target_word, item->target_language);
	} else if (touch->x >= 580 && touch->x <= 640) {
		// Save item item to user history
		if (remove_from_history(username, item->native_language, item->target_language, item->native_word, item->target_word) == NULLRemoveFailure) {
			error_screen("Timed out waiting for server");
		} else {
			remove_history_node(historyObj, node_idx);
			refresh_globals(historyObj);
		}
		clear_screen();
		draw_history_screen(historyObj);
	}
}

void history_screen(historyObj * historyObj)
{
	clear_screen();

	page_number_history = 0;
	num_entries_history = historyObj->translationCount;
	num_pages_history = (ENTRIES_PER_PAGE + num_entries_history - 1) / ENTRIES_PER_PAGE;

	draw_history_screen(historyObj);
	Point touch;
	while(1) {
		touch = get_touch();
		
		// Up page event
		if (page_number_history > 0 && touch.x >= 650 && touch.x <= 750 && touch.y >= 150 && touch.y <= 250) {
			page_number_history--;
			clear_screen();
			draw_history_screen(historyObj);
		} 
		// Down page event
		else if (page_number_history < (num_pages_history - 1) && touch.x >= 650 && touch.x <= 750 && touch.y >= 260 && touch.y <= 360) {
			page_number_history++;
			clear_screen();
			draw_history_screen(historyObj);
		}
		// Go to main menu
		else if (touch.x >= 500 && touch.x <= 750 && touch.y >= 25 && touch.y <= 75) {
			return;
		}
		// Text to speech for first item
		else if (num_entries_history > (page_number_history * ENTRIES_PER_PAGE) && touch.y >= 100 && touch.y <= 200) {
			item_num_history = 0;
			handle_touch_history(item_num_history, historyObj, &touch);
		}
		// Text to speech for second item
		else if (num_entries_history > (page_number_history * ENTRIES_PER_PAGE + 1) && touch.y >= 210 && touch.y <= 310) {
			item_num_history = 1;
			handle_touch_history(item_num_history, historyObj, &touch);
		}
		// Text to speech for third item
		else if (num_entries_history > (page_number_history * ENTRIES_PER_PAGE + 2) && touch.y >= 320 && touch.y <= 420) {
			item_num_history = 2;
			handle_touch_history(item_num_history, historyObj, &touch);
		}
	}
}
