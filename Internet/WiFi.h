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


#define BRClkFrequency 50000000
#define DesiredBaudrate 9600

#define Wifi_LineControlReg_WordLengthSelect0 0
#define Wifi_LineControlReg_WordLengthSelect1 1
#define Wifi_LineControlReg_DivisorLatchAccessBit 7

#define Wifi_FifoControlReg_ReceiveFIFOReset 1
#define Wifi_FifoControlReg_TransmitFIFOReset 2

#define Wifi_LineStatusReg_DataReady 0
#define Wifi_LineStatusReg_TransmitterHoldingRegister 5

// Wifi serial port functions
int init_wifi(void);
void flush_wifi(void);
int put_char_wifi(int c);
int get_char_wifi(void);
int wifi_test_for_received_data(void);

// Wifi module functions
void do_file(void);
int connect_wifi(void);
void play_audio(char* word, char* language);
topLevelParsed * translate(char * target_language, char * native_language);
int register_user(char* username, char* password);
int user_login(char* username, char* password);
int add_to_history(char * username, char * native_language, char * target_language, char * native_word, char * target_word);
int remove_from_history(char * username, char * native_language, char * target_language, char * native_word, char * target_word);
historyObj * get_user_history(char * username);
int open_bmp_sock(void);
int get_country_from_coords(float lattitude, float longitude);
int lua_command_no_stars(char * str, char * res, int timeout);
int lua_command_no_stars_long(char * str, char * res);
int lua_command_no_stars_short(char * str, char * res);
int lua_command_stars(char * str, char * res);

#endif /* WIFI_H__ */
