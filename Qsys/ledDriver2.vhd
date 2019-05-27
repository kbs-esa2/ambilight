
library ieee; 
USE ieee.std_logic_1164.ALL; 
USE ieee.std_logic_unsigned.ALL;
use IEEE.Numeric_STD.all;

entity ledDriver IS     --name of top level entity
	generic (
		numberOfPixels : integer := 2;
		returnticks : integer := 30 
	);
  port ( 
		reset : IN std_logic;  
		ledclock 	: IN std_logic;		
		red : IN std_logic_vector(7 downto 0);
		green : IN std_logic_vector(7 downto 0);
		blue : IN std_logic_vector(7 downto 0);
		getnext : OUT std_logic;	
		ledreturn : out std_logic;	
		ledout     	: OUT std_logic		
	); 
END ledDriver; 

architecture arch of ledDriver is


	


	
begin
	
	
	process( reset,ledclock,red,green,blue )
	
	type state_type is (RET,B1,B0,BL);  --type of state machine.
	variable current_s,next_s: state_type;

	variable bitindex : integer range 0 to 23;
	variable pixelindex : integer;
	variable currentBit : std_logic;
	
	variable ticks : integer;
	 variable redreg : std_logic_vector(7 downto 0);
	 variable greenreg : std_logic_vector(7 downto 0);
	 variable bluereg : std_logic_vector(7 downto 0);


	begin
		if (reset='1') then
			bitindex := 0;
			byteindex := 0;
			pixelindex := 0;
			ticks := 0;

			current_s := RET;
			next_s := current_s; 

		elsif (rising_edge(ledclock)) then
			ticks := ticks + 1;	
			current_s := next_s;
			case( current_s ) is --case for getting the current bit it has to write
				when RET =>
				ledout <= '0';
				ledreturn <= '1';
				bitindex := 0;
				pixelindex := 0;
				if (ticks = (returnticks-20)) then--get pixel information from dma controller
					getnext <= '1';
				elsif (ticks = returnticks) then--fetch new data and start sending bits
					getnext <= '0';
					redreg := red;
					greenreg := green;
					bluereg := blue;
					if (redreg(0)) then
						next_s := B1;
					else
						next_s := B0;
					end if ;					
				end if ;

			when B1 =>
			ledout <= '1';
			if (ticks = 15) then
				next_s := BL;
			end if ;

			when B0 =>
			ledout <= '1';
			if (ticks = 7) then
				next_s := BL;
			end if ;

			when BL =>
			ledout <= '0';
			if (ticks = 26) then--done sending bit
				if (bitindex = 23) then--done sending pixel
					bitindex := 0;
					getnext <= '0';
					redreg := red;
					greenreg := green;
					bluereg := blue;
					if (pixelindex = numberOfPixels-1) then--all pixels sent 
					pixelindex := 0;
					next_s := RET;
					else
					pixelindex := pixelindex + 1;
					end if ;
				else
				getnext <= '1';
				bitindex := bitindex + 1;
					if (bitindex < 8) then
						currentbit := greenreg(bitindex);
					elsif (bitindex < 16) then
						currentbit := redreg(bitindex - 8);
					else
						currentbit := bluereg(bitindex - 16);
					end if ;
					if currentbit then
						next_s := B1;
					else
						next_s := B0;
					end if ;				
				end if ;				
			end if ;
			end case;
		end if;
	end process ; -- ws2812b


end arch ; -- arch