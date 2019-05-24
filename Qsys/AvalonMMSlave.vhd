library ieee; 
USE ieee.std_logic_1164.ALL; 
USE ieee.std_logic_unsigned.ALL;
use IEEE.Numeric_STD.all;

entity avalonmmslave is
  port (
    --standard ports
    clock       : in std_logic;
    reset       : in std_logic;
    --avalon memory mapped slave port
    irq         : out std_logic;
    readdata    : out std_logic_vector(7 downto 0) := "00000000";
    read        : in std_logic;
    --hardware ports
    keys         : in std_logic_vector(1 downto 0);
    hex        : out std_logic_vector(6 downto 0)
  ) ;
end avalonmmslave;

architecture arch of avalonmmslave is
    
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

    signal value : std_logic_vector(3 downto 0);
    signal cnt_up : std_logic;
    signal cnt_dwn : std_logic;

begin
    seg: hex7seg	  port map		(hex => value, display => hex);
    db1: debouncer  port map 		(clk => clock, button => keys(0),reset => reset, result => cnt_up); 
    db2: debouncer  port map 		(clk => clock, button => keys(1),reset => reset, result => cnt_dwn);
    hd1: helderheid port map 		(clk => clock, 
										 button_up => cnt_up, 
										 button_down => cnt_dwn, 
										 reset => reset, 
                                         result => value); 
                                         
    slave : process( reset,clock,value,read )

    variable lastValue : std_logic_vector(3 downto 0);
    begin
        if (reset = '1') then
            readdata <= "00000000";
            irq <= '0';
        elsif (rising_edge(clock)) then
            if (lastValue /= value) then
                irq <= '1';
                lastValue := value;
            end if;
            if (read = '1') then
                irq <= '0';
                readdata(7 downto 4) <= "0000";
                readdata(3 downto 0) <= value;
            end if ;
        end if ;

    end process ; -- slave

end arch ; -- arch
