module VGADataMux_Verilog( 
	input unsigned [15:0] SRamDataIn,  //data in (2 sets of 6 bits each) from sram
	input Sel,
			
	output reg unsigned [5:0] VGADataOut 
);	

	always@(*)	begin
		if(Sel == 1) 
			VGADataOut <= SRamDataIn[5:0] ;
		else
			VGADataOut <= SRamDataIn[13:8] ;
	end
endmodule
