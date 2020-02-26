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

int main(void)
{
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

	// Initialize the wifi
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

	// Draw the logo screen
	logo_screen();

	return 0;
}
