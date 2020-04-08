module bit_flipper (
    input clock,
    input reset_n,
    input unsigned [1:0] addr,
    input rd_en,
    input wr_en,
    output reg unsigned [31:0] dataOut,
    input unsigned [31:0] dataIn
);
    reg unsigned [31:0] saved_value;					// temp reg for result
	 integer i;

	 // writing
    always@(posedge clock) begin
        if (reset_n == 0) 								// synchronous reset
            saved_value <= 32'b0;					// clear result on reset
        else if (wr_en == 1 && addr == 2'b00)	// if write to address 0
            saved_value <= dataIn;				// store result
        else if (wr_en == 1 && addr == 2'b01) 	// if write to address 1
            saved_value <= saved_value + 1;		// increment result
    end
    
	 // reading
    always@(*)  begin
        dataOut <= 32'b0;								// default value is 0
        if (rd_en == 1) begin
            if (addr == 2'b00) begin				// if reading from address 0 
                for(i = 0; i < 32; i = i + 1)	
                    dataOut[i] <= saved_value[31-i];		// flip bit order
				end
				
            else if (addr == 2'b01) 				// if reading from address 1
                dataOut <= saved_value;			// give back result
            else if (addr == 2'b10) 				// if reading from address 2
                dataOut <= ~saved_value;			// give back inverse all bits of result
        end
    end
endmodule