module GraphicsLCD_Controller_Verilog ( 
		input 							Clock, Reset,
		input unsigned [7:0] 		Red, Green, Blue,
	
		// scroll offsets
		input unsigned [9:0] 		VScrollOffset,	HScrollOffset,
		
		output reg 						H_Sync_out, V_Sync_out,
		output 							VideoBlanking_L,
		output unsigned [7:0] 		Red_out, Green_out, Blue_out,
		output unsigned [9:0] 		Column_out, Row_out
	);


	//////////////////////////////////////////////////////////////////////////////////////
	// Timings - USE THESE (BCDE/PQRS) for 640x480 @60HZ with 25Mhz clock
	//////////////////////////////////////////////////////////////////////////////////////
	
	// H_RES 640;
	// V_RES 480;

	// Horizontal timings
	// B 95;			// sync pulse has to be 3.77us approx or 95*0.04us = 3.8us
	// C 45;			// sync has to return high for 1.79us before video = 45*0.04us = 1.8us
	// D H_RES;		// 800 pixel columns per row; count starts from 0
	// E 20;			// front porch = 0.79uS or 
	
	// Vertical timings
	//P  2;			// vertical sync pulse = at least 64uS so at least 2 horizontal periods = 76.8us
	//Q 27;			// back porch = 102uS (min) so 27 lines of 38.4 uS = at least 26.56 so round up to 27
	//R V_RES	;		// 480 pixel rows per screen; count starts from 0
	//S 12;			// front porch = 450uS (min) so this is at lest 11.8 line periods of 38.4 uS rounded up to 12
	
	//////////////////////////////////////////////////////////////////////////////////////
	// Timings - USE THESE (BCDE/PQRS)for 800x480 @50HZ with 25Mhz clock
	//////////////////////////////////////////////////////////////////////////////////////
	
	// H_RES: := 800;
	// V_RES: := 480;
	
	// Horizontal timings	
	//  B  72;			// sync pulse has to be 3.77us approx or 95*0.04us = 3.8us
	//  C  90;			// back porch has to return high for 1.79us before video = 45*0.04us = 1.8us
	//  D  H_RES;		// 800 pixel columns per row; count starts from 0
	//  E  24;			// front porch = 0.79uS or 
	
	// total horizontal period =  992 clock periods at 25Mhz = 38.4uS

	//  P   7;			// vertical sync pulse
	//  Q   12;			// back porch = 102uS (min) so 27 lines of 38.4 uS = at least 26.56 so round up to 27
	//  R   V_RES	;		// 480 pixel rows per screen; count starts from 0
	//  S   5;				// front porch = 450uS (min) so this is at lest 11.8 line periods of 38.4 uS rounded up to 12	
	
	
	////////////////////////////////////////////////////////////////////////-
	// Timings - USE THESE (BCDE/PQRS)for 800x480 @60HZ with 30Mhz clock
	////////////////////////////////////////////////////////////////////////-
	
	 parameter H_RES = 800;
	 parameter V_RES = 480;
	
	// Horizontal timings	
	// increase back porch count to move image "right" and reduce to move image "left"

	 parameter B =  72;			// sync pulse has to be 3.77us approx or 95*0.04us = 3.8us
	 parameter C =  96;			// back porch has to return high for 1.79us before video = 45*0.04us = 1.8us
	 parameter D =  H_RES;		// 800 pixel columns per row; count starts from 0
	 parameter E =  24;			// front porch = 0.79uS or 
	
	// Vertical timings 
	// increase back porch count to move image "down" screen and reduce to move "up" screen


	 parameter P =   7;			// vertical sync pulse
	 parameter Q =   12;			// back porch = 102uS (min) so 27 lines of 38.4 uS = at least 26.56 so round up to 27
	 parameter R = V_RES;		// 480 pixel rows per screen; count starts from 0
	 parameter S =  3;				// front porch = 450uS (min) so this is at lest 11.8 line periods of 38.4 uS rounded up to 12	
	
	
	// Universal Timings - Use these regardless of resolution / refresh rate
	
	parameter H_TOTAL = (B+C+D+E);
	parameter H_RESPLUS = H_RES ;	
	
	parameter V_TOTAL = (P+Q+R+S);
	parameter V_RESPLUS = (V_RES);	


	reg unsigned [9:0] HCount, VCount; 		// horizontal and vertical counters
	reg unsigned [9:0] Column_out_sig; 		// horizontal counter to include offset
	reg unsigned [9:0] Row_out_sig; 			// horizontal counter to include offset
	
	reg V_Clock;									// clock for vertical counter
	reg Blank_L;									// video DAC Blanking (NOT USED YET)
	reg H_Data_On, V_Data_On;


	always@(posedge Clock) begin							// Horizontal_Counter: columns
		if (Reset == 0) begin
			HCount <= 0;
			Column_out_sig <= HScrollOffset;
			
			H_Data_On <= 0;
			H_Sync_out <= 1;
		end
			
		else begin
			Column_out_sig <= Column_out_sig + 10'd1 ;
			HCount <= HCount + 10'd1;
			V_Clock <= 0;
			
			// Horizontal counter
			if (HCount == B+C+D+E) begin
				HCount <= 0;
				Column_out_sig <= HScrollOffset ;
				V_Clock <= 1;								// generate clock for vertical counter
			end

			// generate H_Sync_out
			if (HCount == D+E+B)
				H_Sync_out <= 1;
				
			else if(HCount == D+E)
				H_Sync_out <= 0;


			// generate H_Data_On
			if (HCount == D+E+B+C)
				H_Data_On <= 1;
				
			else if (HCount == D)
				H_Data_On <= 0;
		end
	end

	always@(posedge V_Clock)  begin				//Vertical_Counter: rows
		if (Reset == 0) begin
			VCount 			<= 0;
			Row_out_sig 	<= VScrollOffset;
			
			V_Data_On 		<= 0;
			V_Sync_out 		<= 1;
		end 
		
		else begin
			// Vertical counter
			Row_out_sig 	<= Row_out_sig + 10'd1 ;
			VCount 			<= VCount + 10'd1;
			
			if(VCount == R+S+P+Q) begin
				VCount 		<= 0;
				Row_out_sig <= VScrollOffset ;
			end

			// generate V_Sync_out
			if (VCount == R+S+P)
				V_Sync_out <= 1;
			else if (VCount == R+S)
				V_Sync_out <= 0;

			// generate V_Data_On
			if (VCount == R+S+P+Q)
				V_Data_On <= 1;
			else if (VCount == R)
				V_Data_On <= 0;
		end
	end


	
// Asserting blank low, is a signal to graphics chip saying no video is being display so it safe
// to draw to the screen. We could use it to control the video DAC on the DE2, but setting colour outputs to zero 
// has same effect.

	always@(posedge Clock) begin
		if(((HCount > H_RESPLUS) && (HCount < (H_TOTAL))) || ((VCount > V_RESPLUS) && (VCount < V_TOTAL))) 
			Blank_L 		<= 0;
		else
			Blank_L 		<= 1;
	end
	
	assign Red_out[7] = H_Data_On & V_Data_On & Red[7];
	assign Red_out[6] = H_Data_On & V_Data_On & Red[6];
	assign Red_out[5] = H_Data_On & V_Data_On & Red[5];
	assign Red_out[4] = H_Data_On & V_Data_On & Red[4];
	assign Red_out[3] = H_Data_On & V_Data_On & Red[3];
	assign Red_out[2] = H_Data_On & V_Data_On & Red[2];
	assign Red_out[1] = H_Data_On & V_Data_On & Red[1];
	assign Red_out[0] = H_Data_On & V_Data_On & Red[0];
	
	assign Green_out[7] = H_Data_On & V_Data_On & Green[7];
	assign Green_out[6] = H_Data_On & V_Data_On & Green[6];
	assign Green_out[5] = H_Data_On & V_Data_On & Green[5];
	assign Green_out[4] = H_Data_On & V_Data_On & Green[4];			
	assign Green_out[3] = H_Data_On & V_Data_On & Green[3];
	assign Green_out[2] = H_Data_On & V_Data_On & Green[2];			
	assign Green_out[1] = H_Data_On & V_Data_On & Green[1];
	assign Green_out[0] = H_Data_On & V_Data_On & Green[0];			
	
	assign Blue_out[7] = H_Data_On & V_Data_On & Blue[7];
	assign Blue_out[6] = H_Data_On & V_Data_On & Blue[6];
	assign Blue_out[5] = H_Data_On & V_Data_On & Blue[5];
	assign Blue_out[4] = H_Data_On & V_Data_On & Blue[4];
	assign Blue_out[3] = H_Data_On & V_Data_On & Blue[3];
	assign Blue_out[2] = H_Data_On & V_Data_On & Blue[2];
	assign Blue_out[1] = H_Data_On & V_Data_On & Blue[1];
	assign Blue_out[0] = H_Data_On & V_Data_On & Blue[0];
	
//	generate column,row signals

	assign Column_out			= Column_out_sig ; 		//	WHEN (H_Data_On = '1' & V_Data_On = '1');
	assign Row_out 			= Row_out_sig ; 			//	WHEN (H_Data_On = '1' & V_Data_On = '1');
	assign VideoBlanking_L  = Blank_L ;					// low when video is turned off, can be used to allow the graphics chip to draw without iterferring with display

endmodule

