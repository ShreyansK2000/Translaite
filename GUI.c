#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "Display/Fonts.h"
#include "Hardware/Touch.h"
#include "Hardware/HardwareRegisters.h"
#include "Display/Colours.h"
#include "Screens/Screens.h"
#include "Display/Graphics.h"
#include "Internet/WiFi.h"
#include "Global.h"

int target_language = FRENCH;
int native_language = ENGLISH;
void * virtual_base = NULL;
enum user_modes user_mode = User;

int main(int argc, char **argv)
{
	// Check for count of arguments. Must be 2
	if (argc != 2) {
		printf("Expected 2 arguments, got %d\n", argc);
		printf("Usage: module1 <usermode>\n");
		printf("usermode:\n\t-t : Test\n");
		printf("\t-u : User\n");
		return 0;
	} else {
		if (!strcmp(argv[1],"-t")) {
			user_mode =  Test;
		} else if (!strcmp(argv[1],"-u")) {
			user_mode = User;
		} else {
			printf("Unknown argument %s\n", argv[1]);
			printf("Usage: module1 <usermode>\n");
			printf("usermode:\n\t-t : Test\n");
			printf("\t-u : User\n");
			return 0;
		}

		printf("Set user mode to: %d\n", user_mode);
	}

	int fd;

	// Open memory as if it were a device for read and write access
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	// map 2Mbyte of memory starting at 0xFF200000 to user space
	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ),
	MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return(1);
	}

	// Initialize the Touch screen
	init_touch();
	touch_flush();

	// Initialize the graphics
	if(!init_graphics()) {
		return 0;
	}

	// Initialize the wifi
	loading_screen("INITIALIZING WIFI..");
	if(!init_wifi()) {
		return 0;
	}
	keep_loading = 0;

	switch(user_mode) {
	// Test mode
	case Test:
		test_screen();
		break;
	// User mode
	case User:
		logo_screen();
		break;
	// Break on default, dont show anything
	default:
		printf("Mode %d not implemented yet\n", user_mode);
		break;
	}

	return 0;
}
