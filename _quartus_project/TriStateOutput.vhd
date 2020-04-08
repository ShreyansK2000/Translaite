LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;


Entity TriStateOutput is
	Port ( 
			EN			: in STD_LOGIC ;
			DataOut 	: out STD_LOGIC_VECTOR(7 downto 0) 
	);	
end;

Architecture rtl of TriStateOutput is
begin
	process(EN)		-- need a signal name here to keep Quartus happy even thought it does not do anything
	begin
			DataOut <= "ZZZZZZZZ";	-- set outputs to always tri-state
	end process ;
end ;