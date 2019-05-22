library 	ieee;
use 		ieee.std_logic_1164.all;
use 		ieee.std_logic_unsigned.all;
use 		ieee.numeric_std.all;

entity helderheid is
	port(
   clk			: in std_logic; 							-- input clock
   button_up	: in std_logic; 							-- button to increase the counter value
	button_down	: in std_logic; 							-- button to decrease the counter value
	reset			: in std_logic;							-- button to reset the counter
	result		:out std_logic_vector(3 downto 0)	-- output value in 4 bits
	);
end entity;


architecture rtl of helderheid is

type state_type is (A, B, C, D, E); 					-- define of the states of the state machine

signal	state			: state_type;						-- signal of the state machine type to write values to
signal 	count 		: std_logic_vector (3 downto 0);	-- register to save the brightness value
constant max_value	: std_logic_vector (3 downto 0) := (others => '1'); -- max cap for brightness
constant min_value	: std_logic_vector (3 downto 0) := (others => '0'); -- minimal cap for brightnes


begin 

process (clk, reset, count)
  begin
   if (reset = '1') then							-- when 'reset' is high, the process goes to the initial state
	state <= A;											-- state sets to A (initial state)		 
	count	<= (others => '0');						-- count resets to initial value of '0000'
   elsif rising_edge(clk) then

	case state is
		when A => 	
			if button_up = '1' then 				-- state 'A' checks which button is made high and sets the next
				state <= B;								-- state according to the button
			elsif button_down = '1' then
				state <= C;
			end if; 
		when B => 
			if count < max_value then 				-- state 'B' and 'C' check if 'count' is not higher than the
				state <= D;								-- max value or lower than the minimal value
			else 
				state <= A;
			end if; 
		when C => 
			if count > min_value then 				
				state <= E;
			else 
				state <= A;
			end if; 
		when D => 										-- state 'D' and 'E' increase or decrease the count value
			if button_up = '0' then
				count <= count + '1'; 
				state <= A;
			end if;
		when E =>
			if button_down = '0' then
				count <= count - '1';
				state <= A;
			end if;
		when others =>									-- if the state is not recognised, the process goes back to
				state <= A;								-- the initial state
	end case; 
   end if; 
end process;

result <= count;										-- the 'count' signal's value goes on the output port
end architecture;