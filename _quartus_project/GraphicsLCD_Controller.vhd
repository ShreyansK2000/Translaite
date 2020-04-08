LIBRARY IEEE;
USE  IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY GraphicsLCD_Controller IS 
	PORT (
		Clock, Reset						: IN STD_LOGIC;
		Red,Green,Blue						: IN STD_LOGIC_VECTOR(7 downto 0);		
	
		-- scroll offsets
		VScrollOffset						: IN STD_LOGIC_VECTOR(9 downto 0);		
		HScrollOffset						: IN STD_LOGIC_VECTOR(9 downto 0);		
	
		H_Sync_out,
		V_Sync_out, 
		VideoBlanking_L					: OUT STD_LOGIC;
		
		Red_out,
		Green_out,
		Blue_out								: OUT STD_LOGIC_VECTOR(7 downto 0);
		
		Column_out,
		Row_out								: OUT STD_LOGIC_VECTOR(9 DOWNTO 0)
	);
END;

ARCHITECTURE FSM OF GraphicsLCD_Controller IS

	---------------------------------------------------------------------------------
	-- Timings - USE THESE (BCDE/PQRS) for 640x480 @60HZ with 25Mhz clock
	---------------------------------------------------------------------------------
	
	-- CONSTANT H_RES: integer:= 640;
	-- CONSTANT V_RES: integer:= 480;

	-- Horizontal timings
	--CONSTANT B: INTEGER :=  95;			-- sync pulse 
	--CONSTANT C: INTEGER :=  45;			-- sync has to return high f
	--CONSTANT D: INTEGER :=  H_RES;		-- 800 pixel columns per row; count starts from 0
	--CONSTANT E: INTEGER :=  20;			-- front porch 
	
	-- Vertical timings
	--CONSTANT P: INTEGER :=   2;			-- vertical sync pulse 
	--CONSTANT Q: INTEGER :=  27;			-- back porch
	--CONSTANT R: INTEGER :=  V_RES	;	-- 480 pixel rows per screen; count starts from 0
	--CONSTANT S: INTEGER :=  12;			-- front porch 
	
	---------------------------------------------------------------------------------
	-- Timings - USE THESE (BCDE/PQRS)for 800x480 @50HZ with 25Mhz clock
	---------------------------------------------------------------------------------
	
	--CONSTANT H_RES: integer:= 800;
	--CONSTANT V_RES: integer:= 480;
	
	-- Horizontal timings	
	--CONSTANT B: INTEGER :=  72;			-- sync pulse 
	--CONSTANT C: INTEGER :=  90;			-- back porch 
	--CONSTANT D: INTEGER :=  H_RES;		-- 800 pixel columns per row; count starts from 0
	--CONSTANT E: INTEGER :=  24;			-- front porch 
	
	-- total horizontal period =  992 clock periods at 25Mhz = 38.4uS

	--CONSTANT P: INTEGER :=   7;			-- vertical sync pulse
	--CONSTANT Q: INTEGER :=   12;			-- back porch 
	--CONSTANT R: INTEGER :=  V_RES	;		-- 480 pixel rows per screen; count starts from 0
	--CONSTANT S: INTEGER :=  5;				-- front porch 
	
	
	---------------------------------------------------------------------------------
	-- Timings - USE THESE (BCDE/PQRS)for 800x480 @60HZ with 30Mhz clock
	---------------------------------------------------------------------------------
	
	 CONSTANT H_RES: integer:= 800;
	 CONSTANT V_RES: integer:= 480;
	
	-- Horizontal timings	
	-- increase back porch count to move image "right" and reduce to move image "left"

	 CONSTANT B: INTEGER :=  72;			-- sync pulse h
	 CONSTANT C: INTEGER :=  96;			-- back porch 
	 CONSTANT D: INTEGER :=  H_RES;		-- 800 pixel columns per row; count starts from 0
	 CONSTANT E: INTEGER :=  24;			-- front porch 
	
	-- Vertical timings 
	-- increase back porch count to move image "down" screen and reduce to move "up" screen


	 CONSTANT P: INTEGER :=   7;			-- vertical sync pulse
	 CONSTANT Q: INTEGER :=   12;			-- back porch 
	 CONSTANT R: INTEGER :=  V_RES	;	-- 480 pixel rows per screen; count starts from 0
	 CONSTANT S: INTEGER :=  3;			-- front porch 
	
	---------------------------------------------------------------------------------
	-- Universal Timings - Use these regardless of resolution / refresh rate
	---------------------------------------------------------------------------------
	
	CONSTANT H_TOTAL: INTEGER := (B+C+D+E)	;
	CONSTANT H_RESPLUS: INTEGER := (H_RES) ;	
	
	CONSTANT V_TOTAL: INTEGER := (P+Q+R+S) ;
	CONSTANT V_RESPLUS: INTEGER := (V_RES) ;	


	SIGNAL HCount, VCount 			: STD_LOGIC_VECTOR(9 DOWNTO 0) ; 		-- horizontal and vertical counters
	SIGNAL Column_out_sig			: STD_LOGIC_VECTOR(9 DOWNTO 0) ; 		-- horizontal counter to include offset
	SIGNAL Row_out_sig		   	: STD_LOGIC_VECTOR(9 DOWNTO 0) ; 		-- horizontal counter to include offset
	SIGNAL V_Clock 					: STD_LOGIC ;									-- clock for vertical counter
	SIGNAL Blank_L						: STD_LOGIC ;									-- video DAC Blanking (NOT USED YET)
	SIGNAL H_Data_on, V_Data_on	: STD_LOGIC ;

BEGIN
	PROCESS(Clock, Reset, HScrollOffset) 			-- Horizontal_Counter: columns
	BEGIN
		IF (Reset = '0') THEN
			HCount <= (OTHERS => '0');
			Column_out_sig <= HScrollOffset;
			
			H_Data_on <= '0';
			H_Sync_out <= '1';
			
		ELSIF (rising_edge(Clock)) THEN
			
			Column_out_sig <= Column_out_sig + 1 ;
			HCount <= HCount + 1;
			V_Clock <= '0';
			
			-- Horizontal counter
			IF (HCount = B+C+D+E) THEN
				HCount <= (OTHERS => '0');
				Column_out_sig <= HScrollOffset ;
				V_Clock <= '1';								-- generate clock for vertical counter
			END IF;

			-- generate H_Sync_out
			IF (HCount = D+E+B) THEN
				H_Sync_out <= '1';
				
			ELSIF (HCount = D+E) THEN
				H_Sync_out <= '0';
				
			END IF;

			-- generate H_Data_on
			IF (HCount = D+E+B+C) THEN
				H_Data_on <= '1';
				
			ELSIF (HCount = D) THEN
				H_Data_on <= '0';
			END IF;
		END IF;
	END PROCESS;

	PROCESS(V_Clock, reset, VScrollOffset)  	--Vertical_Counter: rows
	BEGIN
		IF (Reset = '0') THEN
			VCount 			<= (OTHERS => '0');
			Row_out_sig 	<= VScrollOffset;
			
			V_Data_on 		<= '0';
			V_Sync_out 		<= '1';
			
		ELSIF (V_Clock'EVENT AND V_Clock = '1') THEN
			-- Vertical counter
			Row_out_sig 	<= Row_out_sig + 1 ;
			VCount 			<= VCount + 1;
			
			IF (VCount = R+S+P+Q) THEN
				VCount 		<= (OTHERS => '0');
				Row_out_sig <= VScrollOffset ;
			END IF;

			-- generate V_Sync_out
			IF (VCount = R+S+P) THEN
				V_Sync_out <= '1';
			ELSIF (VCount = R+S) THEN
				V_Sync_out <= '0';
			END IF;

			-- generate V_Data_on
			IF (VCount = R+S+P+Q) THEN
				V_Data_on <= '1';
			ELSIF (VCount = R) THEN
				V_Data_on <= '0';
			END IF;
		END IF;
	END PROCESS;

	
-- Asserting blank low, is a signal to graphics chip saying no video is being display so it safe
-- to draw to the screen. We could use it to control the video DAC on the DE2, but setting colour outputs to zero 
-- has same effect.

	PROCESS(clock)
	BEGIN
		if(rising_edge(clock)) then
			IF(((HCount > H_RESPLUS) AND (HCount < (H_TOTAL))) OR ((VCount > V_RESPLUS) AND (VCount < V_TOTAL))) THEN
				Blank_L 		<= '0';
			ELSE
				Blank_L 		<= '1';
			END IF;
		end if;
	END PROCESS;

	Red_out(7) <= H_data_on AND V_data_on AND Red(7);
	Red_out(6) <= H_data_on AND V_data_on AND Red(6);
	Red_out(5) <= H_data_on AND V_data_on AND Red(5);
	Red_out(4) <= H_data_on AND V_data_on AND Red(4);
	Red_out(3) <= H_data_on AND V_data_on AND Red(3);
	Red_out(2) <= H_data_on AND V_data_on AND Red(2);
	Red_out(1) <= H_data_on AND V_data_on AND Red(1);
	Red_out(0) <= H_data_on AND V_data_on AND Red(0);
	
	Green_out(7) <= H_data_on AND V_data_on AND Green(7);
	Green_out(6) <= H_data_on AND V_data_on AND Green(6);
	Green_out(5) <= H_data_on AND V_data_on AND Green(5);
	Green_out(4) <= H_data_on AND V_data_on AND Green(4);			
	Green_out(3) <= H_data_on AND V_data_on AND Green(3);
	Green_out(2) <= H_data_on AND V_data_on AND Green(2);			
	Green_out(1) <= H_data_on AND V_data_on AND Green(1);
	Green_out(0) <= H_data_on AND V_data_on AND Green(0);			
	
	Blue_out(7) <= H_data_on AND V_data_on AND Blue(7);
	Blue_out(6) <= H_data_on AND V_data_on AND Blue(6);
	Blue_out(5) <= H_data_on AND V_data_on AND Blue(5);
	Blue_out(4) <= H_data_on AND V_data_on AND Blue(4);
	Blue_out(3) <= H_data_on AND V_data_on AND Blue(3);
	Blue_out(2) <= H_data_on AND V_data_on AND Blue(2);
	Blue_out(1) <= H_data_on AND V_data_on AND Blue(1);
	Blue_out(0) <= H_data_on AND V_data_on AND Blue(0);
	
--	generate column,row signals

	Column_out			<= Column_out_sig ; 		--	WHEN (H_Data_on = '1' AND V_Data_on = '1');
	Row_out 				<= Row_out_sig ; 			--	WHEN (H_Data_on = '1' AND V_Data_on = '1');
	VideoBlanking_L  	<= Blank_L ;				-- low when video is turned off, can be used to allow the graphics chip to draw without iterferring with display

END FSM;
