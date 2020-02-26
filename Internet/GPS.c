#include <stdio.h>

#include "../Global.h"
#include "../Hardware/HardwareRegisters.h"

#include "GPS.h"

volatile unsigned char *RS232_ReceiverFifo = NULL;
volatile unsigned char *RS232_TransmitterFifo = NULL;
volatile unsigned char *RS232_InterruptEnableReg = NULL;
volatile unsigned char *RS232_InterruptIdentificationReg = NULL;
volatile unsigned char *RS232_FifoControlReg = NULL;
volatile unsigned char *RS232_LineControlReg = NULL;
volatile unsigned char *RS232_ModemControlReg = NULL;
volatile unsigned char *RS232_LineStatusReg = NULL;
volatile unsigned char *RS232_ModemStatusReg = NULL;
volatile unsigned char *RS232_ScratchReg = NULL;
volatile unsigned char *RS232_DivisorLatchLSB = NULL;
volatile unsigned char *RS232_DivisorLatchMSB = NULL;

#define RS232_LineControlReg_WordLengthSelect0 0
#define RS232_LineControlReg_WordLengthSelect1 1
#define RS232_LineControlReg_DivisorLatchAccessBit 7
#define RS232_FifoControlReg_ReceiveFIFOReset 1
#define RS232_FifoControlReg_TransmitFIFOReset 2
#define RS232_LineStatusReg_DataReady 0
#define RS232_LineStatusReg_TransmitterHoldingRegister 5

char data[100];

/**************************************************************************
 Subroutine to initialise the RS232 Port by writing some data
 to the internal registers.
 Call this function at the start of the program before you attempt
 to read or write to data via the RS232 port


 Refer to UART data sheet for details of registers and programming
***************************************************************************/
void Init_RS232(void)
{
	RS232_ReceiverFifo =(unsigned short int *)(virtual_base + (( RS232_ReceiverFifoOffset ) &
			(HW_REGS_MASK ) ));
	if (RS232_ReceiverFifo == NULL) {
		printf("ERROR, RS232_ReceiverFifo was not correctly defined. Try again");
	}

	RS232_TransmitterFifo =(unsigned short int *)(virtual_base + (( RS232_TransmitterFifoOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_TransmitterFifo == NULL) {
		printf("ERROR, RS232_TransmitterFifo was not correctly defined. Try again");
	}

	RS232_InterruptEnableReg =(unsigned short int *)(virtual_base + (( RS232_InterruptEnableRegOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_InterruptEnableReg == NULL) {
		printf("ERROR, RS232_InterruptEnableReg was not correctly defined. Try again");
	}

	RS232_InterruptIdentificationReg =(unsigned short int *)(virtual_base + (( RS232_InterruptIdentificationRegOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_InterruptIdentificationReg == NULL) {
		printf("ERROR, RS232_InterruptIdentificationReg was not correctly defined. Try again");
	}

	RS232_FifoControlReg =(unsigned short int *)(virtual_base + (( RS232_FifoControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_FifoControlReg == NULL) {
		printf("ERROR, RS232_FifoControlReg was not correctly defined. Try again");
	}

	RS232_LineControlReg =(unsigned short int *)(virtual_base + (( RS232_LineControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_LineControlReg == NULL) {
		printf("ERROR, RS232_LineControlReg was not correctly defined. Try again");
	}

	RS232_ModemControlReg =(unsigned short int *)(virtual_base + (( RS232_ModemControlRegOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_ModemControlReg == NULL) {
		printf("ERROR, RS232_ModemControlReg was not correctly defined. Try again");
	}

	RS232_LineStatusReg =(unsigned short int *)(virtual_base + (( RS232_LineStatusRegOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_LineStatusReg == NULL) {
		printf("ERROR, RS232_LineStatusReg was not correctly defined. Try again");
	}

	RS232_ModemStatusReg =(unsigned short int *)(virtual_base + (( RS232_ModemStatusRegOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_ModemStatusReg == NULL) {
		printf("ERROR, RS232_ModemStatusReg was not correctly defined. Try again");
	}

	RS232_ScratchReg =(unsigned short int *)(virtual_base + (( RS232_ScratchRegOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_ScratchReg == NULL) {
		printf("ERROR, RS232_ScratchReg was not correctly defined. Try again");
	}

	RS232_DivisorLatchLSB =(unsigned short int *)(virtual_base + (( RS232_DivisorLatchLSBOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_DivisorLatchLSB == NULL) {
		printf("ERROR, RS232_DivisorLatchLSB was not correctly defined. Try again");
	}

	RS232_DivisorLatchMSB =(unsigned short int *)(virtual_base + (( RS232_DivisorLatchMSBOffset ) &
		(HW_REGS_MASK ) ));
	if (RS232_DivisorLatchMSB == NULL) {
		printf("ERROR, RS232_DivisorLatchMSB was not correctly defined. Try again");
	}

 // set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
    *RS232_LineControlReg = 0x80;

 // set Divisor latch (LSB and MSB) with correct value for required baud rate
    // This is for baudrate of 9600
    *RS232_DivisorLatchLSB = 0x45; // Paul Davies told me to do this idk what these constants mean
    *RS232_DivisorLatchMSB = 0x01;

 // Reset bits to 0
    *RS232_LineControlReg = 0x0;

 // set bit 7 of Line control register back to 0 and
 // program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
    *RS232_LineControlReg = 0x03;

 // Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
    *RS232_FifoControlReg = 0x06;

 // Now Clear all bits in the FiFo control registers
    *RS232_FifoControlReg = 0x0;
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO
int RS232TestForReceivedData(void)
{
 // if RS232_LineStatusReg bit 0 is set to 1
 //return TRUE, otherwise return FALSE
    return ((*RS232_LineStatusReg >> RS232_LineStatusReg_DataReady) & 1) == 1;
}

int putcharRS232(int c)
{
 // wait for Transmitter Holding Register bit (5) of line status register to be '1'
 // indicating we can write to the device
    while (*RS232_LineStatusReg & 0x20 != 0x20) {
    }

 // write character to Transmitter fifo register
    *RS232_TransmitterFifo = c;

 // return the character we printed
    return c;
}

int getcharRS232( void )
{
 // wait for Data Ready bit (0) of line status register to be '1'
    while(!RS232TestForReceivedData()) {
    }

 // read new character from ReceiverFiFo register
 // return new character
    return *RS232_ReceiverFifo;
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//
void RS232Flush(void)
{
 // while bit 0 of Line Status Register == ‘1’
 // read unwanted char out of fifo receiver buffer
    while(RS232TestForReceivedData()) {
        int read = *RS232_ReceiverFifo;
        read += 1;
    }

    return; // no more characters so return
}

void collectData(void)
{
  char c;
  int i = 0;

  while(1) {
    if (getcharRS232() != 'G')
      continue;
    if (getcharRS232() != 'G')
      continue;
    if (getcharRS232() != 'A')
      continue;
    break;
  }


  c = getcharRS232();
  while (c != '*') {
    c = getcharRS232();
    data[i] = c;
    i++;
  }
  data[99] = '\0';
}

void init_gps()
{
	Init_RS232();
	RS232Flush();
	collectData();
}

//Returns latitude in degree decimal format
float get_latitude() {
  float degrees = (data[11] - '0') * 10 + data[12] - '0';
  float mins = (data[13] - '0') * 10 + (data[14] - '0') + (data[16] - '0') / 10.0 + (data[17] - '0') / 100.0 + (data[18] - '0') / 1000.0 + (data[19] - '0') / 10000.0;
  if (data[21] == 'N') {
    return degrees + mins / 60.0;
  }
  else {
    return (degrees + mins / 60.0) * -1;
  }
}

//Returns longitude in degree decimal format
float get_longitude() {
  float degrees = (data[23] - '0') * 100 + (data[24] - '0') * 10 + data[25] - '0';
  float mins = (data[26] - '0') * 10 + data[27] - '0' + (data[29] - '0') / 10.0 + (data[30] - '0') / 100.0 + (data[31] - '0') / 1000.0 + (data[32] - '0') / 10000.0;
  if (data[34] == 'E') {
    return degrees + mins / 60.0;
  }
  else {
    return (degrees + mins / 60.0) * -1;
  }
}
