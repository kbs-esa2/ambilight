
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
	
	type state_type is (B0,B1,RET);  --type of state machine.
	variable current_s,next_s: state_type;

	variable bitindex : integer range 0 to 8;
	variable byteindex : integer range 0 to 4;
	variable pixelindex : integer;
	variable currentBit : std_logic;
	variable lastPixel : std_logic;
	variable lastByte : std_logic;
	variable lastBit : std_logic;
	
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
			case( byteindex ) is --case for getting the current bit it has to write
			
				when 1 => currentBit := redreg(bitindex);
				when 0 => currentBit := greenreg(bitindex);
				when 2 => currentBit := bluereg(bitindex);
			
				when others => currentBit := '0';

			end case ;
			
			if (current_s = RET) then
				if (ticks = returnticks) then
					ledreturn <= '0';
					ticks := 0;
					if (currentbit = '1') then
						current_s := B1;
					else
						current_s := B0;
					end if ;
				end if ;
			else
				
				if (ticks = 26) then
					ticks := 0;
					bitindex := bitindex + 1;
					if (bitindex = 8) then -- done sending byte go to next
						bitindex := 0;
						byteindex := byteindex + 1;
						if (byteindex = 3) then
							
							byteindex := 0;
							pixelindex := pixelindex + 1;
							if (pixelindex = numberOfPixels) then
								pixelindex := 0;
								current_s := RET;
							end if ;
						
						end if ;
					end if ;
				else
					if (currentbit = '1') then
						current_s := B1;
					else
						current_s := B0;
					end if ;	
				end if ;
			end if ;
			
			if (ticks >= 24) and (byteindex = 2)  and (bitindex = 7) then
				redreg := red;
				greenreg := green;
				bluereg := blue;
			end if;

			if (ticks >= 2) and (byteindex = 2)  and (bitindex = 7) then
				getnext <= '1';
			
			else
			getnext <= '0';
				
			end if ;

			case current_s is

				when B0 =>
					if (ticks <= 7) then
						ledout <= '1';
					else
						ledout <= '0';					
					end if ;

				when B1 =>
					if (ticks <= 15) then
						ledout <= '1';
					else
						ledout <= '0';
					end if ;


				when RET => --sending 50 ns of nothing to initiate new start of pixel data
					byteindex := 0;
					pixelindex := 0;
					ledout <= '0';
					ledreturn <= '1';
			end case;
			
		
		end if;
	end process ; -- ws2812b


end arch ; -- arch