#include <stdio.h>

#include "../Global.h"
#include "../Hardware/HardwareRegisters.h"

#include "GPS.h"

volatile unsigned char *GPS_ReceiverFifo = NULL;
volatile unsigned char *GPS_TransmitterFifo = NULL;
volatile unsigned char *GPS_InterruptEnableReg = NULL;
volatile unsigned char *GPS_InterruptIdentificationReg = NULL;
volatile unsigned char *GPS_FifoControlReg = NULL;
volatile unsigned char *GPS_LineControlReg = NULL;
volatile unsigned char *GPS_ModemControlReg = NULL;
volatile unsigned char *GPS_LineStatusReg = NULL;
volatile unsigned char *GPS_ModemStatusReg = NULL;
volatile unsigned char *GPS_ScratchReg = NULL;
volatile unsigned char *GPS_DivisorLatchLSB = NULL;
volatile unsigned char *GPS_DivisorLatchMSB = NULL;

#define GPS_LineControlReg_WordLengthSelect0 0
#define GPS_LineControlReg_WordLengthSelect1 1
#define GPS_LineControlReg_DivisorLatchAccessBit 7
#define GPS_FifoControlReg_ReceiveFIFOReset 1
#define GPS_FifoControlReg_TransmitFIFOReset 2
#define GPS_LineStatusReg_DataReady 0
#define GPS_LineStatusReg_TransmitterHoldingRegister 5

#define TIMEOUT_END 2000000

// global to hold the data being streamed by the gps module
char data[100];

void init_gps_serial(void)
{
	GPS_ReceiverFifo =(unsigned short int *)(virtual_base + (( GPS_ReceiverFifoOffset ) &
			(HW_REGS_MASK ) ));
	if (GPS_ReceiverFifo == NULL) {
		printf("ERROR, GPS_ReceiverFifo was not correctly defined. Try again");
	}

	GPS_TransmitterFifo =(unsigned short int *)(virtual_base + (( GPS_TransmitterFifoOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_TransmitterFifo == NULL) {
		printf("ERROR, GPS_TransmitterFifo was not correctly defined. Try again");
	}

	GPS_InterruptEnableReg =(unsigned short int *)(virtual_base + (( GPS_InterruptEnableRegOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_InterruptEnableReg == NULL) {
		printf("ERROR, GPS_InterruptEnableReg was not correctly defined. Try again");
	}

	GPS_InterruptIdentificationReg =(unsigned short int *)(virtual_base + (( GPS_InterruptIdentificationRegOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_InterruptIdentificationReg == NULL) {
		printf("ERROR, GPS_InterruptIdentificationReg was not correctly defined. Try again");
	}

	GPS_FifoControlReg =(unsigned short int *)(virtual_base + (( GPS_FifoControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_FifoControlReg == NULL) {
		printf("ERROR, GPS_FifoControlReg was not correctly defined. Try again");
	}

	GPS_LineControlReg =(unsigned short int *)(virtual_base + (( GPS_LineControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_LineControlReg == NULL) {
		printf("ERROR, GPS_LineControlReg was not correctly defined. Try again");
	}

	GPS_ModemControlReg =(unsigned short int *)(virtual_base + (( GPS_ModemControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_ModemControlReg == NULL) {
		printf("ERROR, GPS_ModemControlReg was not correctly defined. Try again");
	}

	GPS_LineStatusReg =(unsigned short int *)(virtual_base + (( GPS_LineStatusRegOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_LineStatusReg == NULL) {
		printf("ERROR, GPS_LineStatusReg was not correctly defined. Try again");
	}

	GPS_ModemStatusReg =(unsigned short int *)(virtual_base + (( GPS_ModemStatusRegOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_ModemStatusReg == NULL) {
		printf("ERROR, GPS_ModemStatusReg was not correctly defined. Try again");
	}

	GPS_ScratchReg =(unsigned short int *)(virtual_base + (( GPS_ScratchRegOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_ScratchReg == NULL) {
		printf("ERROR, GPS_ScratchReg was not correctly defined. Try again");
	}

	GPS_DivisorLatchLSB =(unsigned short int *)(virtual_base + (( GPS_DivisorLatchLSBOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_DivisorLatchLSB == NULL) {
		printf("ERROR, GPS_DivisorLatchLSB was not correctly defined. Try again");
	}

	GPS_DivisorLatchMSB =(unsigned short int *)(virtual_base + (( GPS_DivisorLatchMSBOffset ) &
		(HW_REGS_MASK ) ));
	if (GPS_DivisorLatchMSB == NULL) {
		printf("ERROR, GPS_DivisorLatchMSB was not correctly defined. Try again");
	}
	
	// set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
    *GPS_LineControlReg = 0x80;
	
	// set Divisor latch (LSB and MSB) with correct value for required baud rate
    // This is for baudrate of 9600
    *GPS_DivisorLatchLSB = 0x45; // Paul Davies told me to do this idk what these constants mean
    *GPS_DivisorLatchMSB = 0x01;
	
	// Reset bits to 0
    *GPS_LineControlReg = 0x0;
	
	// set bit 7 of Line control register back to 0 and
	// program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
    *GPS_LineControlReg = 0x03;
	
	// Reset the Fifos in the FiFo Control Reg by setting bits 1 & 2
    *GPS_FifoControlReg = 0x06;
	
	// Now Clear all bits in the FiFo control registers
    *GPS_FifoControlReg = 0x0;
}

/* 
 * The following function polls the UART to determine if any character
 * has been received. It doesn't wait for one, or read it, it simply tests
 * to see if one is available to read from the FIFO
 */
int gps_test_for_received_data(void)
{
	// if GPS_LineStatusReg bit 0 is set to 1
	// return TRUE, otherwise return FALSE
    return ((*GPS_LineStatusReg >> GPS_LineStatusReg_DataReady) & 1) == 1;
}

/*
 * Writes a character to the gps serial port.
 */
int put_char_gps(int c)
{
	// wait for Transmitter Holding Register bit (5) of line status register to be '1'
	// indicating we can write to the device
    while (*GPS_LineStatusReg & 0x20 != 0x20) {
    }
	
	// write character to Transmitter fifo register
    *GPS_TransmitterFifo = c;
	
	// return the character we printed
    return c;
}

/*
 * Blocks until a character is received from the wifi module serial port.
 */
int get_char_gps( void )
{
	// wait for Data Ready bit (0) of line status register to be '1'
    while(!gps_test_for_received_data()) {
    }
	
	// read new character from ReceiverFiFo register
	// return new character
    return *GPS_ReceiverFifo;
}

void flush_gps(void)
{
	// while bit 0 of Line Status Register == ‘1’
	// read unwanted char out of fifo receiver buffer
    while(gps_test_for_received_data()) {
        int read = *GPS_ReceiverFifo;
        read += 1;
    }

    return; // no more characters so return
}

/*
 * Reads the string following a GGA substring in the data being streamed from the GPS module
 * 
 * The read data is put into the data buffer
 */
int collect_data(void)
{
  char c;
  int i = 0;

  int timeout = 0;
  while(timeout < TIMEOUT_END) {
    if (get_char_gps() != 'G') {
    	timeout++;
    	continue;
    }

    if (get_char_gps() != 'G') {
    	timeout++;
    	continue;
    }

    if (get_char_gps() != 'A') {
    	timeout++;
    	continue;
    }

    break;
  }

  if (timeout == TIMEOUT_END) {
	  return 0;
  }

  c = get_char_gps();
  while (c != '*') {
    c = get_char_gps();
    data[i] = c;
    i++;
  }
  data[99] = '\0';

  return 1;
}

int init_gps()
{
	init_gps_serial();
	flush_gps();

	// Start collecting data
	return collect_data();
}

float get_latitude()
{
	float degrees = (data[11] - '0') * 10 + data[12] - '0';
	float mins = (data[13] - '0') * 10 + (data[14] - '0') + (data[16] - '0') / 10.0 + (data[17] - '0') / 100.0 + (data[18] - '0') / 1000.0 + (data[19] - '0') / 10000.0;
	
	if (data[21] == 'N') {
		return degrees + mins / 60.0;
	}
	else {
		return (degrees + mins / 60.0) * -1;
	}
}

float get_longitude()
{
	float degrees = (data[23] - '0') * 100 + (data[24] - '0') * 10 + data[25] - '0';
	float mins = (data[26] - '0') * 10 + data[27] - '0' + (data[29] - '0') / 10.0 + (data[30] - '0') / 100.0 + (data[31] - '0') / 1000.0 + (data[32] - '0') / 10000.0;
	
	if (data[34] == 'E') {
		return degrees + mins / 60.0;
	}
	else {
		return (degrees + mins / 60.0) * -1;
	}
}
