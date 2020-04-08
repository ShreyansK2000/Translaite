
module SerialIODecoder (
		input unsigned [15:0] Address,
		input IOSelect_H,
		input ByteSelect_L,
		
		output reg RS232_Port_Enable,
		output reg GPS_Port_Enable,
		output reg Bluetooth_Port_Enable,
		output reg TouchScreen_Port_Enable,
		output reg WiFi_Port_Enable
	);

	always@(Address, IOSelect_H, ByteSelect_L) begin
		
// default values for the IO chip enables (default = disabled)
// they are overridden below when necessary - default values for outputs avoids inferring latches in VHDL 
// so we must do it for all our outputs
		
		RS232_Port_Enable <= 0 ;
		GPS_Port_Enable <= 0 ;
		Bluetooth_Port_Enable <= 0 ;
		TouchScreen_Port_Enable <= 0 ;
		WiFi_Port_Enable <= 0 ;
		
// IOSelect_H is driven logic 1 whenever the CPU outputs an address in the range A31:A0 = hex [FF21_0000] to [FF21_FFFF]
// that is, IOSelect_H is driven to logic 1 for all addresses in range [FF21_XXXX]. 
// All we have to do for each UART IO chip is decode the XXXX i.e. A15:A0 into 4 blocks of 16 bytes each. 
// All addresses of registers in our UART chips should be even address as they are byte wide and connected to 
// the upper half of the data bus (ByteSelect_L is asserted for an even byte transfer of D15-D8)

// decoder for the 1st UART 16550 chip (RS232 Port) - Registers located between addresses 0xFF21 0200 - 0xFF21 020F 
// so that they occupy same half of data bus on D15-D8 and ByteSelect_L = 0

		if((IOSelect_H == 1) && (Address[15:4] == 12'h020) && ByteSelect_L == 0) 		// address = 0xFF21_0200 - hex FF21_020F
			RS232_Port_Enable <= 1 ;		// enable the 1st UART device

// decoder for the 2nd UART 16550 chip (GPS Port) - Registers located between addresses 0xFF21 0210 - 0xFF21 021F 
// so that they occupy same half of data bus on D15-D8 and ByteSelect_L = 0
		if((IOSelect_H == 1) && (Address[15:4] == 12'h021) && ByteSelect_L == 0)		// address = 0xFF21_0210 - hex FF21_021F
			GPS_Port_Enable <= 1;		// enable the 2nd UART device

// decoder for the 3rd UART 16550 chip (Bluetooth Port) - Registers located between addresses 0xFF21 0220 - 0xFF21 022F 
// so that they occupy same half of data bus on D15-D8 and ByteSelect_L = 0
		if((IOSelect_H == 1) && (Address[15:4] == 12'h022) && ByteSelect_L == 0)		// address = 0xFF21_0220 - hex FF21_022F
			Bluetooth_Port_Enable <= 1;		// enable the 3rd UART device
		
// decoder for the 4th UART 16550 chip (TouchScreen Port) - Registers located between addresses 0xFF21 0230 - 0xFF21 023F 
// so that they occupy same half of data bus on D15-D8 and ByteSelect_L = 0
		if((IOSelect_H == 1) && (Address[15:4] == 12'h023) && ByteSelect_L == 0)		// address = 0xFF21_0230 - hex FF21_023F
			TouchScreen_Port_Enable <= 1;		// enable the 4th UART device

// decoder for the 5th UART 16550 chip (WiFi Port) - Registers located between addresses 0xFF21_0240 - 0xFF21 024F 
// so that they occupy same half of data bus on D15-D8 and ByteSelect_L = 0
		if((IOSelect_H == 1)	&& (Address[15:4] == 12'h024) && ByteSelect_L == 0)
			WiFi_Port_Enable <= 1; // enable the 5th UART device
	
	end
endmodule
