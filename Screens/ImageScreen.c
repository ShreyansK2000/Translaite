#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Hardware/Touch.h"
#include "../Internet/Ethernet.h"

void draw_image_screen()
{
	draw_bitmap_image("out.bmp", "out.txt");
}

void image_screen(int download_img)
{
	clear_screen();
	if (download_img) {
		get_file("192.168.0.200", 8080, "out.bmp", "out.txt");
	}
	draw_image_screen();
	while(1) {
		// Wait for user touch and return
		get_touch();
		load_original_colour_palette();
		return;
	}
}
