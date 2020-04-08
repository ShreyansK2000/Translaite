LIBRARY ieee;
USE ieee.std_logic_1164.all;
use ieee.std_logic_arith.all; 
use ieee.std_logic_unsigned.all; 
 

entity Latch3Bit is
 Port (
  DataIn : in Std_logic_vector(2 downto 0) ;
  Enable: in Std_logic ;
  Clk : in Std_logic ;
  Reset : in Std_logic ;
  WE_L	: in Std_logic ;
  
  Q : out Std_Logic_vector(2 downto 0)
 );
end ;
 
architecture bhvr of Latch3Bit is
Begin
 process(DataIn, Enable, Clk, RESET, WE_L)
 Begin
  if(Reset = '0') then
	Q <= "000" ;
  elsif(rising_edge(Clk)) then
	if(Enable = '1' and WE_L = '0') then
		Q <= DataIn(2 downto 0) ;
	end if ;
  end if ;
 end process ;
end ;
