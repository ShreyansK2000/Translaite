#ifndef WIFI_H__
#define WIFI_H__

#include "Parse.h"

#define Wifi_ReceiverFifoOffset 				(0x00010200)
#define Wifi_TransmitterFifoOffset 				(0x00010200)
#define Wifi_InterruptEnableRegOffset 			(0x00010202)
#define Wifi_InterruptIdentificationRegOffset 	(0x00010204)
#define Wifi_FifoControlRegOffset 				(0x00010204)
#define Wifi_LineControlRegOffset 				(0x00010206)
#define Wifi_ModemControlRegOffset 				(0x00010208)
#define Wifi_LineStatusRegOffset 				(0x0001020A)
#define Wifi_ModemStatusRegOffset 				(0x0001020C)
#define Wifi_ScratchRegOffset 					(0x0001020E)
#define Wifi_DivisorLatchLSBOffset				(0x00010200)
#define Wifi_DivisorLatchMSBOffset 				(0x00010202)

#define Wifi_LineControlReg_WordLengthSelect0 0
#define Wifi_LineControlReg_WordLengthSelect1 1
#define Wifi_LineControlReg_DivisorLatchAccessBit 7

#define Wifi_FifoControlReg_ReceiveFIFOReset 1
#define Wifi_FifoControlReg_TransmitFIFOReset 2

#define Wifi_LineStatusReg_DataReady 0
#define Wifi_LineStatusReg_TransmitterHoldingRegister 5

/**
 * Subroutine to initialise the wifi serial port by writing some data
 * to the internal registers.
 *
 * Call this function at the start of the program before you attempt
 * to read or write to data via the Touch port.
 *
 * Refer to UART data sheet for details of registers and programming.
 */
int init_wifi(void);

/**
 * Remove/flush the UART receiver buffer by removing any unread characters.
 */
void flush_wifi(void);

/**
 * Writes a character to the wifi serial port.
 */
int put_char_wifi(int c);

/**
 * Blocks until a character is received from the wifi module serial port.
 */
int get_char_wifi(void);

/**
 * This function polls the UART to determine if any character
 * has been received. It doesn't wait for one, or read it, it simply tests
 * to see if one is available to read from the FIFO
 */
int wifi_test_for_received_data(void);

/**
 * Loads the lua file on the wifi module.
 */
void do_file(void);

/**
 * Connects to the wifi coded in the lua file
 */
int connect_wifi(void);

/**
 * Call the play audio endpoint on the server. word is the word you are trying to hear, languages
 * is the language you want to hear the word in. Provide word in the translated form, and language
 * in the english form.
 */
void play_audio(char* word, char* language);

/**
 * Calls the translate endpoint on the server. Provide a target language and native language to translate
 * to. These languages need to be in english (i.e translate("french", "english")).
 */
topLevelParsed * translate(char * target_language, char * native_language);

/**
 * Calls the user registration endpoint on the server. Provide a username and
 * password to try registering with. Returns the status enumeration based on
 * response from the server.
 */
int register_user(char* username, char* password);

/**
 * Calls the user login endpoint on the server. Provide a username and
 * password to try authenticating. Returns the status enumeration based
 * on response from the server.
 */
int user_login(char* username, char* password);

/**
 * Calls the history add endpoint on the server. Provide a username, native language,
 * target language, target language translated word, and native language translated word. The target
 * language and native language need to be in english (i.e french,english) while the words should be
 * translated in their languages. Returns the status enumeration based on response from the server.
 */
int add_to_history(char * username, char * native_language, char * target_language, char * native_word, char * target_word);

/**
 * Calls the history add endpoint on the server. Provide a username, native language,
 * target language, target language translated word, and native language translated word. The target
 * language and native language need to be in english (i.e french,english) while the words should be
 * translated in their languages. Returns the status enumeration based on response from the server.
 */
int remove_from_history(char * username, char * native_language, char * target_language, char * native_word, char * target_word);

/**
 * Calls the get user history endpoint on the server. Provide a username for the user you
 * are interested in getting the history for. Returns a malloc'd struct that must be freed.
 */
historyObj * get_user_history(char * username);

/**
 * Calls the endpoint on the server that opens a socket. When this function returns,
 * you can safely listen on that socket and read the incoming data. Returns the status
 * enumeration based on response from the server.
 */
int open_bmp_sock(void);

/**
 * Calls the endpoint that gets the country for a given latitude and longitude. Returns
 * the status enumeration based on response from the server.
 */
int get_country_from_coords(float latitude, float longitude);

/**
 * This function writes and reads from the serial port, and does not wait for ****
 * 
 * Provide a timeout values for a for loop to loop through. If the code you are waiting for
 * has long periods of blocking, it is recommended to use the long timeout version of this method.
 * Otherwise, you can use the short version or just pass in a custom timeout.
 */
int lua_command_no_stars(char * str, char * res, int timeout);

/**
 * This function writes and reads from the serial port, and does not wait for ****
 * 
 * This is a the long timeout version of lua_command_no_stars
 */
int lua_command_no_stars_long(char * str, char * res);

/**
 * This function writes and reads from the serial port, and does not wait for ****
 * 
 * This is a the short timeout version of lua_command_no_stars
 */
int lua_command_no_stars_short(char * str, char * res);

/**
 * This function writes and reads from the serial port, and waits for **** to indicate
 * the end of a message. Will wait as long as lua_command_no_stars_long waits
 */
int lua_command_stars(char * str, char * res);

#endif /* WIFI_H__ */
