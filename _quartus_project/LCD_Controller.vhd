LIBRARY ieee;
USE ieee.std_logic_1164.all;
use ieee.std_logic_arith.all; 
use ieee.std_logic_unsigned.all; 

------------------------------------------------------------------------------------------------------------------------------
entity LCD_Controller is
	Port( 	LCDDataOut : out std_logic_vector(7 downto 0) ;				-- 8 bit data connected to LCD Data in pins, 
				RS : out std_logic ;													-- connected to RS (register select pin) on LCD Display
				EN : out std_logic ;													-- Enable signal to the LCD display
				RW : out std_logic ;													-- Read(1)/Write(0) signal to the LCD display, set to 0 when writing characters commands, 1 when reading status
				
				Clk : in std_logic ;													-- 25MHz clock driving state machine
				Reset_L: in std_logic ;												-- Reset to initialise the state machine and to force initialisation of the LCD display

				DataIn : in std_logic_vector(7 downto 0) ;					-- Data in to this state machine that will get written to the LCD display
				WriteEnable_H : in std_logic; 										-- Signal to ask the state machine to write DataIn to the LCD display
				LCDCommandOrDisplayData : in std_logic							-- signal which defines the value of the RS register select signal sent to the LCD display
		);
end LCD_Controller ;

--------------------------------------------------------------------------------------------------------------------------------
	
architecture bhvr of LCD_Controller is 

-- define a set of names for the states that the state machine can be in - used for Next_State and Current_State
	
    type System_state is (Idle, AssertRW, Dummy1, AssertE_Data, WaitingForETimer, RemoveE, RemoveRW, RemoveData);
		
-- signals connecting processes in the circuit together

   Signal Next_state : System_state;				-- Next state records which state the machine will go to next when the CLK occurs, can be assigned one of the names defined above
	Signal Current_state  : System_state;		-- current state records the current state of the state machine, can be assigned one of the names defined above
	
	Signal StartETimer : std_logic ;				-- this signal starts the 1st timer (a process) that measure at least 240ns (by measuring CLK pulses). 
																-- Used to make sure 'E' pulse to LCD is at least 240ns wide
	Signal ETimerDone : std_logic ;				-- this signal is produced by the 1st timer when the 240ns has elapsed
	

BEGIN


----------------------------------------------------------------------------------------------------------------------------------------------------------
-- concurrent process#2 : counter to simulate time delay of at least 240ns (min 7 clock cycles at 25Mhz)	
-- this is used to ensure the pulse width of E timing requirement is met
-- Counter workd by preloading it with a suitable value and then counting down to 0 with each clock pulse
-- when it reaches 0 it outputs a signal to indicate time has elapsed
-- Time delay = preloaded value * (1/12500000)
----------------------------------------------------------------------------------------------------------------------------------------------------------

   Process(Clk, StartETimer)
			variable timer : integer range 0 to 8 ;							-- variable with sufficient range to count clock pulses
	BEGIN
			if(StartETimer = '1') then											-- if we get the signal from another process to start the timer
				timer  := 8 ;															-- Preload timer with a margin of safety count of 8
			elsif (rising_edge(Clk) and (timer /= 0)) then				-- otherwise, provided timer has not already counted down to 0, on the next rising edge of the clock		
				timer := timer - 1 ;												-- subtract 1 from the timer value
			end if;
			
			if(timer = 0) then														-- if timer has counted down to 0
				ETimerDone <= '1' ;												-- output '1' to indicate time has elapsed
			else																			-- otherwise output '0' to indicate we are still timing
				ETimerDone <= '0' ;
			end if ;
	END Process;			

---------------------------------------------------------------------------------------------------------------------
-- concurrent process#3: state registers
-- this process RECORDS  the current state of the system.
-- it can be reset to the IDLE state when presented with the signal 'Reset' 
--
-- Otherwise on the rising edge of the CLK the system will always transfer the value of Next_State 
-- signals to Current-State signal and change the state of the state machine
-- The decision about what is the Next-State is made by another process below
----------------------------------------------------------------------------------------------------------------------

   Process(Reset_L, Clk)
	BEGIN
			if(Reset_L = '0') then
				Current_state <= IDLE ;
			elsif (rising_edge(Clk)) then		-- state can change only on low-to-high transition of clock
				Current_state <= Next_State;	
				if(Current_State = Idle) then
					LCDDataOut <= DataIn ; 								-- save data we are writing to lcd display
					RS <= LCDCommandOrDisplayData ;					-- save if we are writing a command or data
				end if ;
			end if;
	END Process;	
		
  
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 -- concurrent process#1: Logic to define the signals that drive the set the next state and the signals to the LCD display
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	Process(Current_state, Clk, WriteEnable_H, ETimerDone, DataIn, LCDCommandOrDisplayData)
    BEGIN

-- These are the default signals to send to LCD display
-- these get overrridden later in the process as required
-- remember that signals are only output at the end of the process and thus assume the value
-- of the last assignment '<=' performed on them

		EN <= '0' ;													-- disable EN so LCD will not respond
		RW <= '0' ;												
		StartETimer <= '1' ;									-- Keep LCD E timer in reset state
		Next_State <= IDLE ;
		
-----------------------------------------------------------------------------------------------------------------------------------------------------
-- State: IDLE. In this state we sit idle waiting for a WriteData signal to tell us to start writing data to the display
-----------------------------------------------------------------------------------------------------------------------------------------------------
		
		if( current_state = Idle) then						-- if we are in the idle state (doing nothing)
			RW <= '1' ;
			if(WriteEnable_H = '1')  then					-- if we get the signal to begin writing to the LCD display
				Next_State <= AssertRW ;					-- Signal to state register that our next state should be AssertRW
			else
				Next_State <= IDLE ;								-- then stay in this state until told to write to the LCD display
			end if ;
			
-----------------------------------------------------------------------------------------------------------------------------------------------------
-- State AssertRW - start of an LCD write cycle - 
-- in this state we assert the LCD's  RW signal 
-----------------------------------------------------------------------------------------------------------------------------------------------------

		elsif(current_state = AssertRW) then			
			Next_State <= Dummy1 ;						-- Signal to state register that our next state should be Dummy1
			
-----------------------------------------------------------------------------------------------------------------------------------------------------
-- state 2 - Dummy state 
-- in this state we simply to waste time (1 clock cycle) to ensure setup time of RW relative to E (taken from LCD data sheet)
----------------------------------------------------------------------------------------------------------------------------------------------------

		elsif(current_state = Dummy1) then				-- waste 1/25000000 of a sec
			Next_State <= AssertE_Data ;					-- Signal to state register that our next state should be AssertE_Data

-----------------------------------------------------------------------------------------------------------------------------------------------------
-- state 3 - Assert E  appropriate time after RW
-- in this state we set the LCDs 'E' signal to 0 and start the ETimer by releasing it's start/reset intput
-----------------------------------------------------------------------------------------------------------------------------------------------------

		elsif(Current_state = AssertE_Data) then
			EN <= '1' ;												-- Assert LCDs E signal to make it listen 
			StartETimer <= '0' ;								-- start 'E' timing clock to make sure we don't remove E until min-E pulse width of 230ns is met
			
			Next_State <= WaitingForETimer;			-- Signal to state register that our next state should be WaitingForETimer

-----------------------------------------------------------------------------------------------------------------------------------------------------
-- state 4 - waiting for 230ns to pass before removing E (cycle time of E)
-----------------------------------------------------------------------------------------------------------------------------------------------------

		elsif(Current_state = WaitingForETimer) then 
			EN <= '1' ;												--  LCDs E signal still at 1 to make it listen 
			StartETimer <= '0' ;								-- start timing clock to make sure we don't remove E until min-E pulse width of 230ns is met
			
			if(ETimerDone = '1') then						-- if timer has counted down to 0
				Next_State <= RemoveE;					-- Signal to state register (process 3) that our next state should be RemoveE
			else														-- otherwise stay in this state and continue to count
				Next_State <= WaitingForETimer ;		-- Signal to state register (process 3) that our next state should be this one
			end if;
			
-----------------------------------------------------------------------------------------------------------------------------------------------------
-- state 5 - remove E
-- in this state the LCDs E signal is removed to terminate the write cycle
-----------------------------------------------------------------------------------------------------------------------------------------------------

		elsif(Current_state = RemoveE) then
			Next_State <= RemoveRW ;					-- Signal to state register (process 3) that our next state should be RemoveRW

-----------------------------------------------------------------------------------------------------------------------------------------------------------
-- state 6 - remove Read Write	
-- in this state the LCDs R/W signal is removed (this has to happen after E has been removed)
-- we also start the cycle timer to measure 1.64 ms to make sure we cannot begin writing to the LCD display until this time has passed
-----------------------------------------------------------------------------------------------------------------------------------------------------------
		
		elsif(Current_state = RemoveRW) then
			RW <= '1' ;										-- remove the RW 
			Next_State <= Idle ;		-- Signal to state register (process 3) that our next state should be WaitingCycleTime
		end if ;
	end Process ;
end ;