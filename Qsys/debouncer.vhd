library 	ieee;
use 		ieee.std_logic_1164.all;
use 		ieee.std_logic_unsigned.all;


entity debouncer is
	port(
   clk    	: in	std_logic;  			-- input clock
   button 	: in	std_logic;  			-- input signal to be debounced
	reset		: in	std_logic;				-- reset for counter
   result 	: out	std_logic				-- debounced signal
	);
end entity;


architecture rtl of debouncer is
constant 	min_ticks : integer := 0;			
constant 	max_ticks : integer := 1000;		 
shared variable ticks : integer range min_ticks to (max_ticks + 1) := 0;	-- integer with range to count
begin																								-- the ticks

process(clk, reset)
begin

if reset = '1' then				-- when 'reset' is made high, the process resets to its initial state
	ticks := 0;
	result <= '0';
elsif rising_edge(clk) then
	if (button = '0') and ticks < max_ticks then 	-- when 'button' gives a logic low 'ticks' increments 1
		ticks := ticks + 1;
		if ticks = max_ticks then						 	-- when 'ticks' is at max, result is set to '1'
			result <= '1';
		end if;
	elsif (button = '1') and ticks > min_ticks then	-- when 'button' is released 'ticks' decreases by 1
		ticks := ticks - 1;									-- until 'ticks' equals zero, then 'result' is set to '0'
		if ticks = 0 then 		
			result <= '0';
		end if;
	end if;
end if;
end process;

end architecture;