module RamAddressMapper_Verilog ( 
		input unsigned [9:0] RowAddress, ColumnAddress,
		
		output unsigned [17:0] RamAddress, 		// 2^18 = 256k locations
		output ByteSelect 
	);	

	assign RamAddress[17:9] = RowAddress[8:0];     	// 9 bit row address for 512 rows only 480 visible down of 1 pixels each
	assign RamAddress[8:0]  = ColumnAddress[9:1];   // 9 bit column address for 1024 pixels across (only 800 visible) of 1 pixel in size
	assign ByteSelect 		= ColumnAddress[0];		// 1 bit of the column address serves to access 1 byte in a 16 bit wide ram location
endmodule


