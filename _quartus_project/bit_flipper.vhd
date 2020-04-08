library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity bit_flipper is
port (
    clock: in std_logic;
    reset_n: in std_logic;
    addr: in std_logic_vector(1 downto 0);
    rd_en: in std_logic;
    wr_en: in std_logic;
    dataOut: out std_logic_vector(31 downto 0);
    dataIn: in std_logic_vector(31 downto 0)
);
end bit_flipper;

architecture rtl of bit_flipper is
    signal saved_value: std_logic_vector(31 downto 0);
begin
    
    --saved_value
    process (clock)
    begin
        if rising_edge(clock) then
            if (reset_n = '0') then
                saved_value <= X"00000000";
            elsif (wr_en = '1' and addr = "00") then
                saved_value <= dataIn;
            elsif (wr_en = '1' and addr = "01") then
                saved_value <= std_logic_vector(unsigned(saved_value) + 1);
            end if;
        end if;
    end process;
    
    --readdata
    process (rd_en, addr, saved_value)
    begin
        dataOut <= X"00000000";
        if (rd_en = '1') then
            if (addr = "00") then
                -- bit-flip
                for i in 0 to 31 loop
                    dataOut(i) <= saved_value(31-i);
                end loop;
            elsif (addr = "01") then
                dataOut <= saved_value;
            elsif (addr = "10") then
                dataOut <= not saved_value;
            end if;
        end if;
    end process;
end rtl;