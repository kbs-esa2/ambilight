library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- self resetting counter
entity srcnt8 is
port(
clk, reset : in std_logic;
top : in std_logic_vector(7 downto 0);
clkOut : out std_logic;
count : out std_logic_vector(7 downto 0));
end entity srcnt8;

architecture counter of srcnt8 is

begin
increment: process (reset, clk) -- sensitivity list

variable cntValue:  integer;
variable topValue:  integer;

begin
topValue := to_integer(unsigned(top));
if reset = '1' then -- Async reset
cntValue := 0; -- reset
clkOut <= '0';
elsif rising_edge(clk) then

cntValue := cntValue + 1 ;

end if;

if cntValue >= topValue then
cntValue := 0;
end if;

if cntValue = 0 then
clkOut <= '1';
else
clkOut <= '0';
end if;

count <= std_logic_vector(to_unsigned(cntValue,8)); -- output new value

end process increment;


end counter;
