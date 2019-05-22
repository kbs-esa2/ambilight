library 	ieee;
use 		ieee.std_logic_1164.all;
use 		ieee.numeric_std.all;


entity leds is 
	port(
	clock_50 :	in std_logic;									-- clock signal 50 Mhz.
	key 		: 	in std_logic_vector(2 downto 0);			-- input to alter the values of the brightness
	hex0 		:	out std_logic_vector(6 downto 0)			-- output of the brightness value to 7-segment display
	);
end entity;


architecture ex1 of leds is

component debouncer is
	port(
   clk     	: in	std_logic;   								-- input clock
   button  	: in	std_logic;   								-- input signal to be debounced
	reset		: in	std_logic;	 								-- button to reset the debouncer component
   result	: out	std_logic	 								-- debounced signal
	);  
end component;

component helderheid is
	port(
   clk			: in std_logic; 								-- input clock
   button_up	: in std_logic;								-- button to increase the counter value
	button_down	: in std_logic; 								-- button to decrease the counter value
	reset			: in std_logic; 								-- button to reset the counter
	result		: out std_logic_vector(3 downto 0)		-- output value in 4 bits
	);
end component;

component hex7seg IS 
	port( 
	hex		: in std_logic_vector(3 downto 0);			-- input value in 4 bits
	display	: out std_logic_vector(0 TO 6)				-- output value in 7 bits
	);
end component;
 

 signal		posReset		: std_logic;						-- signal to trigger an positive reset
 signal 		btn_up  		: std_logic;						-- signal to connect the button up
 signal 		btn_down		: std_logic;						-- signal to connect the button down
 signal 		hexDisplay	: std_logic_vector(3 downto 0); -- signal to connect the 7-segment display


begin

 PosReset <= not key(0);										-- inverting of the value of the button
 
 seg: hex7seg	  port map		(hex => hexDisplay, display => hex0);
 db1: debouncer  port map 		(clk => clock_50, button => key(1),reset => posReset, result => btn_up); 
 db2: debouncer  port map 		(clk => clock_50, button => key(2),reset => posReset, result => btn_down);
 hd1: helderheid port map 		(clk => clock_50, 
										 button_up => btn_up, 
										 button_down => btn_down, 
										 reset => posReset, 
										 result => hexDisplay); 

end architecture;