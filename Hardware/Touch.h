#ifndef TOUCH_H__
#define TOUCH_H__

#define Touch_ReceiverFifoOffset 				(0x00010230)
#define Touch_TransmitterFifoOffset 			(0x00010230)
#define Touch_InterruptEnableRegOffset 			(0x00010232)
#define Touch_InterruptIdentificationRegOffset 	(0x00010234)
#define Touch_FifoControlRegOffset 				(0x00010234)
#define Touch_LineControlRegOffset 				(0x00010236)
#define Touch_ModemControlRegOffset 			(0x00010238)
#define Touch_LineStatusRegOffset 				(0x0001023A)
#define Touch_ModemStatusRegOffset 				(0x0001023C)
#define Touch_ScratchRegOffset 					(0x0001023E)
#define Touch_DivisorLatchLSBOffset				(0x00010230)
#define Touch_DivisorLatchMSBOffset 			(0x00010232)

#define Touch_LineControlReg_WordLengthSelect0 0
#define Touch_LineControlReg_WordLengthSelect1 1
#define Touch_LineControlReg_DivisorLatchAccessBit 7

#define Touch_FifoControlReg_ReceiveFIFOReset 1
#define Touch_FifoControlReg_TransmitFIFOReset 2

#define Touch_LineStatusReg_DataReady 0
#define Touch_LineStatusReg_TransmitterHoldingRegister 5

// Dimensions of the touch screen
#define XRES 800
#define YRES 480

// Datatype to hold the x,y coordinates of a touch/release event
typedef struct { int x, y; } Point ;

/*
 * Subroutine to initialise the Touch Port by writing some data
 * to the internal registers.
 *
 * Call this function at the start of the program before you attempt
 * to read or write to data via the Touch port.
 *
 * Refer to UART data sheet for details of registers and programming.
*/
void init_touch(void);

/*
 * Remove/flush the UART receiver buffer by removing any unread characters.
 */
void touch_flush(void);

/*
 * Writes a character to the touch screen serial port.
 */
int put_char_touch(int c);

/*
 * Blocks until a character is received from the touch screen serial port.
 */
int get_char_touch(void);

/*
 * Blocks and tests if the next touch screen event was a touch.
 */
int screen_touched( void );

/*
 * Blocks and tests if the next touch screen event was a release.
 */
int screen_released( void );

/*
 * Blocks until a touch event is detected.
 */
void wait_for_touch();

/*
 * Blocks until a release event is detected.
 */
void wait_for_release();

/*
 * This function polls the UART to determine if any character
 * has been received. It doesn't wait for one, or read it, it simply tests
 * to see if one is available to read from the FIFO
 */
int touch_test_for_received_data(void);

/*
 * This function waits for a touch screen press event and returns X,Y coord
 */
Point get_press(void);

/*
 * This function waits for a touch screen release event and returns X,Y coord
 */
Point get_release(void);

/*
 * This function waits for a touch screen press event, and then a release event
 * and returns X,Y coord of the release.
 */
Point get_touch(void);

#endif
