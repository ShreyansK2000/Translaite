#include <stdio.h>

#include "Touch.h"
#include "HardwareRegisters.h"
#include "../Global.h"

volatile unsigned char *Touch_ReceiverFifo = NULL;
volatile unsigned char *Touch_TransmitterFifo = NULL;
volatile unsigned char *Touch_InterruptEnableReg = NULL;
volatile unsigned char *Touch_InterruptIdentificationReg = NULL;
volatile unsigned char *Touch_FifoControlReg = NULL;
volatile unsigned char *Touch_LineControlReg = NULL;
volatile unsigned char *Touch_ModemControlReg = NULL;
volatile unsigned char *Touch_LineStatusReg = NULL;
volatile unsigned char *Touch_ModemStatusReg = NULL;
volatile unsigned char *Touch_ScratchReg = NULL;
volatile unsigned char *Touch_DivisorLatchLSB = NULL;
volatile unsigned char *Touch_DivisorLatchMSB = NULL;

void init_touch()
{

	Touch_ReceiverFifo =(unsigned short int *)(virtual_base + (( Touch_ReceiverFifoOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_ReceiverFifo == NULL) {
		printf("ERROR, Touch_ReceiverFifo was not correctly defined. Try again");
	}

	Touch_TransmitterFifo =(unsigned short int *)(virtual_base + (( Touch_TransmitterFifoOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_TransmitterFifo == NULL) {
		printf("ERROR, Touch_TransmitterFifo was not correctly defined. Try again");
	}

	Touch_InterruptEnableReg =(unsigned short int *)(virtual_base + (( Touch_InterruptEnableRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_InterruptEnableReg == NULL) {
		printf("ERROR, Touch_InterruptEnableReg was not correctly defined. Try again");
	}

	Touch_InterruptIdentificationReg =(unsigned short int *)(virtual_base + (( Touch_InterruptIdentificationRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_InterruptIdentificationReg == NULL) {
		printf("ERROR, Touch_InterruptIdentificationReg was not correctly defined. Try again");
	}

	Touch_FifoControlReg =(unsigned short int *)(virtual_base + (( Touch_FifoControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_FifoControlReg == NULL) {
		printf("ERROR, Touch_FifoControlReg was not correctly defined. Try again");
	}

	Touch_LineControlReg =(unsigned short int *)(virtual_base + (( Touch_LineControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_LineControlReg == NULL) {
		printf("ERROR, Touch_LineControlReg was not correctly defined. Try again");
	}

	Touch_ModemControlReg =(unsigned short int *)(virtual_base + (( Touch_ModemControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_ModemControlReg == NULL) {
		printf("ERROR, Touch_ModemControlReg was not correctly defined. Try again");
	}

	Touch_LineStatusReg =(unsigned short int *)(virtual_base + (( Touch_LineStatusRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_LineStatusReg == NULL) {
		printf("ERROR, Touch_LineStatusReg was not correctly defined. Try again");
	}

	Touch_ModemStatusReg =(unsigned short int *)(virtual_base + (( Touch_ModemStatusRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_ModemStatusReg == NULL) {
		printf("ERROR, Touch_ModemStatusReg was not correctly defined. Try again");
	}

	Touch_ScratchReg =(unsigned short int *)(virtual_base + (( Touch_ScratchRegOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_ScratchReg == NULL) {
		printf("ERROR, Touch_ScratchReg was not correctly defined. Try again");
	}

	Touch_DivisorLatchLSB =(unsigned short int *)(virtual_base + (( Touch_DivisorLatchLSBOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_DivisorLatchLSB == NULL) {
		printf("ERROR, Touch_DivisorLatchLSB was not correctly defined. Try again");
	}

	Touch_DivisorLatchMSB =(unsigned short int *)(virtual_base + (( Touch_DivisorLatchMSBOffset ) &
		(HW_REGS_MASK ) ));
	if (Touch_DivisorLatchMSB == NULL) {
		printf("ERROR, Touch_DivisorLatchMSB was not correctly defined. Try again");
	}
	
	// set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
    *Touch_LineControlReg = 0x80;
	
	// set Divisor latch (LSB and MSB) with correct value for required baud rate
	// This is for baudrate of 9600
    *Touch_DivisorLatchLSB = 0x45;
    *Touch_DivisorLatchMSB = 0x01;
	
	// Reset bits to 0
    *Touch_LineControlReg = 0x0;
	
	// set bit 7 of Line control register back to 0 and
	// program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
    *Touch_LineControlReg = 0x03;

	// Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
    *Touch_FifoControlReg = 0x06;

	// Now Clear all bits in the FiFo control registers
    *Touch_FifoControlReg = 0x0;

    // Send the touch enable command
    put_char_touch(0x55);
    put_char_touch(0x1);
    put_char_touch(0x12);
}

int touch_test_for_received_data(void)
{
	// if Touch_LineStatusReg bit 0 is set to 1
	//return TRUE, otherwise return FALSE
    return ((*Touch_LineStatusReg >> Touch_LineStatusReg_DataReady) & 1) == 1;
}

int put_char_touch(int c)
{
	// wait for Transmitter Holding Register bit (5) of line status register to be '1'
	// indicating we can write to the device
    while ((*Touch_LineStatusReg & 0x20) != 0x20) {
    }
	
	// write character to Transmitter fifo register
    *Touch_TransmitterFifo = c;
	
	// return the character we printed
    return c;
}

int get_char_touch( void )
{
	// wait for Data Ready bit (0) of line status register to be '1'
    while(!touch_test_for_received_data()) {
    }
	
	// read new character from ReceiverFiFo register
	// return new character
    return *Touch_ReceiverFifo;
}

void touch_flush(void)
{
	// while bit 0 of Line Status Register == ‘1’
	// read unwanted char out of fifo receiver buffer
    while(touch_test_for_received_data()) {
        int read = *Touch_ReceiverFifo;
        read += 1;
    }

    return; // no more characters so return
}

int screen_touched( void )
{
    int firstByte = get_char_touch();

    return ((((firstByte >> 7) & 1) == 1) && ((firstByte & 1) == 1));
}

void wait_for_touch()
{
    while(!screen_touched()) {
    }
}

int screen_released( void )
{
    int firstByte = get_char_touch();

    return ((((firstByte >> 7) & 1) == 1) && ((firstByte & 1) == 0));
}

void wait_for_release()
{
    while(!screen_released()) {
    }
}

Point get_press(void)
{
    Point p1;
    // wait for a pen down command then return the X,Y coord of the point
    // calibrated correctly so that it maps to a pixel on screen
    wait_for_touch();
	int i2 = get_char_touch();
	int i3 = get_char_touch();
	int i4 = get_char_touch();
	int i5 = get_char_touch();

    int x_coordinate = (i2 + (i3 << 7)) * XRES / 4096; // scale x coordinate to fit in 800px
    int y_coordinate = (i4 + (i5 << 7)) * YRES / 4096; // scale y coordinate to fit in 480px

    p1.x = x_coordinate;
    p1.y = y_coordinate;

    return p1;
}

Point get_release(void)
{
    Point p1;
    // wait for a pen up command then return the X,Y coord of the point
    // calibrated correctly so that it maps to a pixel on screen
    wait_for_release();
	int i2 = get_char_touch();
    int i3 = get_char_touch();
    int i4 = get_char_touch();
    int i5 = get_char_touch();

    int x_coordinate = (i2 + (i3 << 7)) * XRES / 4096; // scale x coordinate to fit in 800px
    int y_coordinate = (i4 + (i5 << 7)) * YRES / 4096; // scale y coordinate to fit in 480px

    p1.x = x_coordinate;
    p1.y = y_coordinate;

    return p1;
}

Point get_touch(void)
{
	get_press();
	return get_release(); // This qualifies the touch coordinates as the release point
}
