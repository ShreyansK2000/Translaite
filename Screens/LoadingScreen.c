#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "Screens.h"
#include "../Display/Graphics.h"
#include "../Display/Fonts.h"
#include "../Display/Colours.h"

int keep_loading = 1;

void *wait(void) {
	clock_t start = clock();

	while(keep_loading){;}

	int sec = (clock() - start)/CLOCKS_PER_SEC;
	printf("LOADING TOOK %d SECONDS\n", sec);

	return NULL;
}

void loading_screen(char* loading_message)
{
	clear_screen();
	write_word(50, 50, loading_message, BLACK, WHITE, LARGE);

	keep_loading = 1;

    pthread_t thread_id;
	pthread_create(&thread_id, NULL, wait, NULL);
}
