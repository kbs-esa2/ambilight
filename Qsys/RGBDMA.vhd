
library ieee; 
USE ieee.std_logic_1164.ALL; 
USE ieee.std_logic_unsigned.ALL;
use IEEE.Numeric_STD.all;

entity rgbdma is
    generic(
        baseAdress : integer := 0
    );
  port (
    clk : IN std_logic;
		  reset : IN std_logic;  
		waitrequest : IN std_logic;
		address : out std_logic_vector(31 downto 0);
		read : out std_logic;
        readdata : in std_logic_vector(31 downto 0);
        
        red : OUT std_logic_vector(7 downto 0);
        green : OUT std_logic_vector(7 downto 0);
        blue : OUT std_logic_vector(7 downto 0);
        ledreturn : in std_logic;
        getnext : IN std_logic
  ) ;
end rgbdma;

architecture arch of rgbdma is

    begin

    process( clk,reset,getnext,ledreturn )

    type state_type is (idle,setmaster,reading,finishread);  --type of state machine.
    variable current_s,next_s: state_type;
    variable atAdress : integer := baseAdress;
    
    begin
        if (reset = '1') then
            atAdress := baseAdress;
        elsif (rising_edge(clk)) then
            
            current_s := next_s;
            case( current_s ) is
                        

                when idle =>
                    if (getnext = '1') then
                        next_s := setmaster;
                    end if ;

                when setmaster =>
                    read <= '1';
                    if (waitrequest = '0') then
                        red <= readdata(7 downto 0);
                        green <= readdata(15 downto 8);
                        blue <= readdata(23 downto 16);
                    next_s := reading;
                    end if ;
                    

                when reading =>
                atAdress := atAdress + 4;
                    next_s := finishread;
                
                    

                when finishread =>
                    
                    read <= '0';
                    if (getnext = '0') then
                        address <= std_logic_vector(to_unsigned((atAdress), address'length));
                        next_s := idle;
                    end if ;
                    
                
            end case ;
        end if ;
        if (ledreturn = '1') then
            atAdress := baseAdress;
        end if ;
    end process ; -- dma



end arch ; -- arch