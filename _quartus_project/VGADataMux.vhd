LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;


Entity VGADataMux is
	Port ( 
			SRamDataIn 	: in STD_LOGIC_VECTOR(15 downto 0);	-- data in (2 sets or 6 bits each) from sram
			Sel 			: in STD_LOGIC ;
			
			VGADataOut 	: out STD_LOGIC_VECTOR(5 downto 0) 
		);	
end;

Architecture a of VGADataMux is
begin
	process(SRamDataIn, Sel)
	begin
		if(Sel = '1') then
			VGADataOut <= SRamDataIn(5 downto 0) ;
		else
			VGADataOut <= SRamDataIn(13 downto 8) ;
		end if ;
	end process ;
end ;
