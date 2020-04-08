LIBRARY ieee;
USE ieee.std_logic_1164.all;
use ieee.std_logic_arith.all; 
use ieee.std_logic_unsigned.all; 

entity HexTo7SegmentDisplay is
	Port (
				Input1 : in std_logic_vector(7 downto 0);
				
				Display1 : out std_logic_vector(6 downto 0) ;
				Display0 : out std_logic_vector(6 downto 0) 
	 );
end ;

architecture bhvr of HexTo7SegmentDisplay is
Begin
	process(Input1)
	Begin
		if(Input1(7 downto 4) = X"0") then 	   Display1 <= b"1000000";
		elsif(Input1(7 downto 4) = X"1") then 	Display1 <= b"1111001";
		elsif(Input1(7 downto 4) = X"2") then 	Display1 <= b"0100100";
		elsif(Input1(7 downto 4) = X"3") then 	Display1 <= b"0110000";
		elsif(Input1(7 downto 4) = X"4") then 	Display1 <= b"0011001";
		elsif(Input1(7 downto 4) = X"5") then 	Display1 <= b"0010010";
		elsif(Input1(7 downto 4) = X"6") then 	Display1 <= b"0000010";
		elsif(Input1(7 downto 4) = X"7") then 	Display1 <= b"1111000";
		elsif(Input1(7 downto 4) = X"8") then 	Display1 <= b"0000000";
		elsif(Input1(7 downto 4) = X"9") then 	Display1 <= b"0010000";
		elsif(Input1(7 downto 4) = X"A") then 	Display1 <= b"0001000";
		elsif(Input1(7 downto 4) = X"B") then 	Display1 <= b"0000011";
		elsif(Input1(7 downto 4) = X"C") then 	Display1 <= b"1000110";
		elsif(Input1(7 downto 4) = X"D") then 	Display1 <= b"0100001";
		elsif(Input1(7 downto 4) = X"E") then 	Display1 <= b"0000110";
		else												Display1 <= b"0001110";
		end if;
	End Process;

	process(Input1)
	Begin
		if(Input1(3 downto 0) = X"0") then 	    Display0 <= b"1000000";
		elsif(Input1(3 downto 0) = X"1") then 	Display0 <= b"1111001";
		elsif(Input1(3 downto 0) = X"2") then 	Display0 <= b"0100100";
		elsif(Input1(3 downto 0) = X"3") then 	Display0 <= b"0110000";
		elsif(Input1(3 downto 0) = X"4") then 	Display0 <= b"0011001";
		elsif(Input1(3 downto 0) = X"5") then 	Display0 <= b"0010010";
		elsif(Input1(3 downto 0) = X"6") then 	Display0 <= b"0000010";
		elsif(Input1(3 downto 0) = X"7") then 	Display0 <= b"1111000";
		elsif(Input1(3 downto 0) = X"8") then 	Display0 <= b"0000000";
		elsif(Input1(3 downto 0) = X"9") then 	Display0 <= b"0010000";
		elsif(Input1(3 downto 0) = X"A") then 	Display0 <= b"0001000";
		elsif(Input1(3 downto 0) = X"B") then 	Display0 <= b"0000011";
		elsif(Input1(3 downto 0) = X"C") then 	Display0 <= b"1000110";
		elsif(Input1(3 downto 0) = X"D") then 	Display0 <= b"0100001";
		elsif(Input1(3 downto 0) = X"E") then 	Display0 <= b"0000110";
		else												Display0 <= b"0001110";
		end if ;
	End Process;
End;