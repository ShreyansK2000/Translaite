LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;


Entity RamAddressMapper is
	Port ( 
		RowAddress, ColumnAddress : in STD_LOGIC_VECTOR(9 downto 0) ;
		RamAddress : out STD_LOGIC_VECTOR(17 downto 0); 		-- 2^18 = 256k locations, we only use one half (could draw in one and then switch????
		ByteSelect : out STD_LOGIC 
	);	
end;

Architecture behavioural of RamAddressMapper is
begin
	--RamAddress(17) <= '0' ;	-- only use half the chip location 128k locations fr max res if 256x512
	
	--RamAddress(16 downto 9) <= RowAddress(8 downto 1) ;     -- 8 bit row address for 256 rows only 240 visible down of 2 pixels each (i.e. 480/240)
	--RamAddress(8 downto 0) <= ColumnAddress(9 downto 1) ;   -- 9 bit column address for 512 pixels across (only 320 visible) of 2 pixels in size (i.e. 640/320)
	
	RamAddress(17 downto 9) <= RowAddress(8 downto 0) ;     -- 9 bit row address for 512 rows only 480 visible down of 1 pixels each
	RamAddress(8 downto 0)  <= ColumnAddress(9 downto 1) ;   -- 9 bit column address for 1024 pixels across (only 640 visible) of 1 pixel in size
	
	ByteSelect <= ColumnAddress(0) ;
	
	-- 1 bit of the column address serves to access 1 byte in a 16 bit wide ram location
	
end ;


