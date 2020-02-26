#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "WiFi.h"
#include "../Hardware/HardwareRegisters.h"
#include "Parse.h"
#include "../Global.h"

volatile unsigned char *Wifi_ReceiverFifo = NULL;
volatile unsigned char *Wifi_TransmitterFifo = NULL;
volatile unsigned char *Wifi_InterruptEnableReg = NULL;
volatile unsigned char *Wifi_InterruptIdentificationReg = NULL;
volatile unsigned char *Wifi_FifoControlReg = NULL;
volatile unsigned char *Wifi_LineControlReg = NULL;
volatile unsigned char *Wifi_ModemControlReg = NULL;
volatile unsigned char *Wifi_LineStatusReg = NULL;
volatile unsigned char *Wifi_ModemStatusReg = NULL;
volatile unsigned char *Wifi_ScratchReg = NULL;
volatile unsigned char *Wifi_DivisorLatchLSB = NULL;
volatile unsigned char *Wifi_DivisorLatchMSB = NULL;

void init_wifi_serial()
{

	Wifi_ReceiverFifo =(unsigned short int *)(virtual_base + (( Wifi_ReceiverFifoOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_ReceiverFifo == NULL) {
		printf("ERROR, Wifi_ReceiverFifo was not correctly defined. Try again");
	}

	Wifi_TransmitterFifo =(unsigned short int *)(virtual_base + (( Wifi_TransmitterFifoOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_TransmitterFifo == NULL) {
		printf("ERROR, Wifi_TransmitterFifo was not correctly defined. Try again");
	}

	Wifi_InterruptEnableReg =(unsigned short int *)(virtual_base + (( Wifi_InterruptEnableRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_InterruptEnableReg == NULL) {
		printf("ERROR, Wifi_InterruptEnableReg was not correctly defined. Try again");
	}

	Wifi_InterruptIdentificationReg =(unsigned short int *)(virtual_base + (( Wifi_InterruptIdentificationRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_InterruptIdentificationReg == NULL) {
		printf("ERROR, Wifi_InterruptIdentificationReg was not correctly defined. Try again");
	}

	Wifi_FifoControlReg =(unsigned short int *)(virtual_base + (( Wifi_FifoControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_FifoControlReg == NULL) {
		printf("ERROR, Wifi_FifoControlReg was not correctly defined. Try again");
	}

	Wifi_LineControlReg =(unsigned short int *)(virtual_base + (( Wifi_LineControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_LineControlReg == NULL) {
		printf("ERROR, Wifi_LineControlReg was not correctly defined. Try again");
	}

	Wifi_ModemControlReg =(unsigned short int *)(virtual_base + (( Wifi_ModemControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_ModemControlReg == NULL) {
		printf("ERROR, Wifi_ModemControlReg was not correctly defined. Try again");
	}

	Wifi_LineStatusReg =(unsigned short int *)(virtual_base + (( Wifi_LineStatusRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_LineStatusReg == NULL) {
		printf("ERROR, Wifi_LineStatusReg was not correctly defined. Try again");
	}

	Wifi_ModemStatusReg =(unsigned short int *)(virtual_base + (( Wifi_ModemStatusRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_ModemStatusReg == NULL) {
		printf("ERROR, Wifi_ModemStatusReg was not correctly defined. Try again");
	}

	Wifi_ScratchReg =(unsigned short int *)(virtual_base + (( Wifi_ScratchRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_ScratchReg == NULL) {
		printf("ERROR, Wifi_ScratchReg was not correctly defined. Try again");
	}

	Wifi_DivisorLatchLSB =(unsigned short int *)(virtual_base + (( Wifi_DivisorLatchLSBOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_DivisorLatchLSB == NULL) {
		printf("ERROR, Wifi_DivisorLatchLSB was not correctly defined. Try again");
	}

	Wifi_DivisorLatchMSB =(unsigned short int *)(virtual_base + (( Wifi_DivisorLatchMSBOffset ) &
		(HW_REGS_MASK ) ));
	if (Wifi_DivisorLatchMSB == NULL) {
		printf("ERROR, Wifi_DivisorLatchMSB was not correctly defined. Try again");
	}

 // set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
    *Wifi_LineControlReg = 0x80;

 // set Divisor latch (LSB and MSB) with correct value for required baud rate
 // This is for baudrate of 9600
    *Wifi_DivisorLatchLSB = 0x1b;
    *Wifi_DivisorLatchMSB = 0x00;

 // Reset bits to 0
    *Wifi_LineControlReg = 0x00;

 // set bit 7 of Line control register back to 0 and
 // program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
    *Wifi_LineControlReg = 0x03;

 // Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
    *Wifi_FifoControlReg = 0x06;

 // Now Clear all bits in the FiFo control registers
    *Wifi_FifoControlReg = 0x00;


}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO
int wifi_test_for_received_data(void)
{
 // if Wifi_LineStatusReg bit 0 is set to 1
 //return TRUE, otherwise return FALSE
    return ((*Wifi_LineStatusReg >> Wifi_LineStatusReg_DataReady) & 1) == 1;
}

int put_char_wifi(int c)
{
 // wait for Transmitter Holding Register bit (5) of line status register to be '1'
 // indicating we can write to the device
    while ((*Wifi_LineStatusReg & 0x20) != 0x20) {
    }

 // write character to Transmitter fifo register
    *Wifi_TransmitterFifo = c;

 // return the character we printed
    return c;
}

int get_char_wifi( void )
{
 // wait for Data Ready bit (0) of line status register to be '1'
    while(!wifi_test_for_received_data()) {
    }

 // read new character from ReceiverFiFo register
 // return new character
    return *Wifi_ReceiverFifo;
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//
void flush_wifi(void)
{
 // while bit 0 of Line Status Register == ‘1’
 // read unwanted char out of fifo receiver buffer
    while(wifi_test_for_received_data()) {
        int read = *Wifi_ReceiverFifo;
        read += 1;
    }

    return; // no more characters so return
}

int init_wifi(void)
{
	init_wifi_serial(virtual_base);
	flush_wifi();
	do_file();
	return connect_wifi();
}

void do_file(void)
{
	char * command = "dofile(\"api.lua\")\r\n";
	char buf[1024] = "";
	lua_command_no_stars_short(command, buf);

	printf("%s\n", buf);
}

int connect_wifi(void)
{
	char * command = "connect_wifi()\r\n";
	char buf[1024] = "";
	lua_command_stars(command, buf);

	printf("%s\n", buf);

	return strcmp("connect_wifi()\r\nOK_CONNECT\n", buf);
}

topLevelParsed * translate(char* target_language, char* native_language)
{
	/*
	 * Example command:
	 * translate('french', 'english')
	 */ 
	char * command[80];
	strcpy(command, "translate(\'");
	strcat(command, target_language);
	strcat(command, "\',\'");
	strcat(command, native_language);
	strcat(command, "\')\r\n");
	char buf[4096] = "";
	lua_command_stars(command, buf);

	printf("%s\n", buf);

	return parse_translation_buffer(buf);
}

int register_user(char* username, char* password)
{
	/*
	 * Example command:
	 * register_user('user', 'password')
	 */ 
	char * command[80];
	strcpy(command, "register_user(\'");
	strcat(command, username);
	strcat(command, "\',\'");
	strcat(command, password);
	strcat(command, "\')\r\n");
	char buf[1024] = "";
	lua_command_stars(command, buf);

	printf("%s\n", buf);

	return parse_register_buffer(buf);
}

int user_login(char* username, char* password)
{
	/*
	 * Example command:
	 * user_login('user', 'password')
	 */ 
	char * command[80];
	strcpy(command, "user_login(\'");
	strcat(command, username);
	strcat(command, "\',\'");
	strcat(command, password);
	strcat(command, "\')\r\n");
	char buf[1024] = "";
	lua_command_stars(command, buf);

	printf("%s\n", buf);

	return parse_login_buffer(buf);
}

void play_audio(char* word, char* language)
{
	// replace spaces with %20 for HTTP params
	char * replaced = spaces_replaced(word);

	/*
	 * Example command:
	 * play_audio("computer%20mouse", "english")
	 */ 
	char * command[100];
	strcpy(command, "play_audio(\"");
	strcat(command, replaced);
	strcat(command, "\",\"");
	strcat(command, language);
	strcat(command, "\")\r\n");
	char buf[1024] = "";
	lua_command_stars(command, buf);

	printf("%s\n", buf);

	free(replaced);
}

int add_to_history(char * username, char * native_language, char * target_language, char * native_word, char * target_word)
{
	// replace spaces with %20 for HTTP requests
	char * native_replaced = spaces_replaced(native_word);
	char * target_replaced = spaces_replaced(target_word);

	/*
	 * Example command:
	 * add_to_history("user", "english", "french", "cat", "chat")
	 */ 
	char * command[1024];
	strcpy(command, "add_to_history(\"");
	strcat(command, username);
	strcat(command, "\",\"");
	strcat(command, native_language);
	strcat(command, "\",\"");
	strcat(command, target_language);
	strcat(command, "\",\"");
	strcat(command, native_replaced);
	strcat(command, "\",\"");
	strcat(command, target_replaced);
	strcat(command, "\")\r\n");
	char buf[1024] = "";
	lua_command_stars(command, buf);

	printf("%s\n", buf);

	free(native_replaced);
	free(target_replaced);

	return parse_add_history_buffer(buf);
}

int remove_from_history(char * username, char * native_language, char * target_language, char * native_word, char * target_word)
{
	// replace spaces with %20 for HTTP requests
	char * native_replaced = spaces_replaced(native_word);
	char * target_replaced = spaces_replaced(target_word);

	/*
	 * Example command:
	 * remove_from_history("user", "english", "french", "cat", "chat")
	 */
	char * command[1024];
	strcpy(command, "remove_from_history(\"");
	strcat(command, username);
	strcat(command, "\",\"");
	strcat(command, native_language);
	strcat(command, "\",\"");
	strcat(command, target_language);
	strcat(command, "\",\"");
	strcat(command, native_replaced);
	strcat(command, "\",\"");
	strcat(command, target_replaced);
	strcat(command, "\")\r\n");
	char buf[1024] = "";
	lua_command_stars(command, buf);

	printf("%s\n", buf);

	free(native_replaced);
	free(target_replaced);

	return parse_remove_history_buffer(buf);
}

historyObj * get_user_history(char * username)
{
	/*
	 * Example command:
	 * add_to_history("user", "english", "french", "cat", "chat")
	 */
	char * command[1024];
	strcpy(command, "get_user_history(\'");
	strcat(command, username);
	strcat(command, "\')\r\n");
	char buf[4096] = "";
	lua_command_stars(command, buf);

	printf("%s\n", buf);

	return parse_history_buffer(buf);
}

int open_bmp_sock()
{
	char * command[1024];
	strcpy(command, "open_bmp_sock()\r\n");
	char buf[4096] = "";
	lua_command_stars(command, buf);

	printf("%s\n", buf);

	return parse_open_sock_buffer(buf);
}

int get_country_from_coords(float latitude, float longitude) 
{
	char * command[1024];
	char * longVal[1024];
	char * latVal[1024];

	// Convert floats to string for HTTP request
	snprintf(latVal, sizeof(latVal), "%.6f", latitude);
	snprintf(longVal, sizeof(longVal), "%.6f", longitude);

	/*
	 * Example command:
	 * get_location('43.99', '-123.5')
	 */
	strcpy(command, "get_location(\'");
	strcat(command, latVal);
	strcat(command, "\',\'");
	strcat(command, longVal);
	strcat(command, "\')\r\n");

	char buf[1024] = "";
	lua_command_stars(command, buf);

	return parse_location_buffer(buf);
}

int lua_command_no_stars(char * str, char * res, int timeout) {
	int i;
	int bytes_received = 0;
	printf("executing %s\n", str);

	// Put the command into the buffer
	while (*str) {
		put_char_wifi(*str);
		str++;

		// Get any data that may be coming back after writing the last char
		for(i=0; i<1000; i++) {
			if(wifi_test_for_received_data() == 1) {
				res[bytes_received++] = get_char_wifi();
			}
		}
	}

	// Before the timeout expires, try to get data
	int j;
	for(j=0; j<timeout; j++){
		if(wifi_test_for_received_data() == 1) {
			res[bytes_received++] = get_char_wifi();
			j=0; // reset the timeout timer if you got data
		}
	}

	res[bytes_received] = '\0';

	return bytes_received;
}

int lua_command_no_stars_short(char * str, char * res) {
	return lua_command_no_stars(str, res, 200000);
}

int lua_command_no_stars_long(char * str, char * res) {
	return lua_command_no_stars(str, res, 20000000);
}

int lua_command_stars(char * str, char * res) {
	int i;

	int bytes_received = 0;
	printf("executing %s\n", str);

	char star = '*';
	int star_count = 0;

	while (*str) {
		put_char_wifi(*str);
		str++;

		for(i=0; i<1000; i++) {
			if(wifi_test_for_received_data() == 1) {
				char c = get_char_wifi();

				if (c == star) { // Start counting * characters
					star_count++;

					if (star_count == 4) { // break if we received the last * character
						res[bytes_received] = '\0';

						return bytes_received;
					}
				} else { // reset the star counter
					while(star_count > 0) {
						res[bytes_received++] = star;
						star_count--;
					}
					res[bytes_received++] = c;
				}
			}
		}
	}

	for(i=0; i<30000000; i++) { // start the timeout counter
		if(wifi_test_for_received_data() == 1) {
			char c = get_char_wifi();

			if (c == star) { // start counting stars
				star_count++;
				if (star_count == 4) { // stop receiving data because last star is received
					res[bytes_received] = '\0';

					return bytes_received;
				}
			} else { // reset the star counter
				while(star_count > 0) {
					res[bytes_received++] = star;
					star_count--;
				}
				res[bytes_received++] = c;
			}

			i = 0; // reset the timeout timer if we received data
		}
	}
	
	res[bytes_received] = '\0';

	return bytes_received;
}
