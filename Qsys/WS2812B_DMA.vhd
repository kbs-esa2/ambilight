library ieee; 
USE ieee.std_logic_1164.ALL; 
USE ieee.std_logic_unsigned.ALL;
use IEEE.Numeric_STD.all;

entity ws2812b_dma is
    generic (
        numleds : integer := 2;
        base : integer := 200;
        returnticks : integer := 4000
    );
  port (
    clk : IN std_logic;
		  reset : IN std_logic;  
		waitrequest : IN std_logic;
		address : out std_logic_vector(31 downto 0);
		read : out std_logic;
        readdata : in std_logic_vector(31 downto 0);

        ledclk : IN std_logic;
        ledout : OUT std_logic
  ) ;
end ws2812b_dma;

architecture arch of ws2812b_dma is

    signal reddata : std_logic_vector(7 downto 0);
    signal greendata : std_logic_vector(7 downto 0);
    signal bluedata : std_logic_vector(7 downto 0);
    signal getnextpixel : std_logic;
    signal ledreturn : std_logic;

    component ledDriver IS     --name of top level entity
	generic (
		numberOfPixels : integer := 5;
		returnticks : integer := 4000 
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
END component;

    component rgbdma is
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
    end component;

begin

    U0: rgbdma
    generic map (
        baseAdress   => base
    )
    port map    (
        clk => clk,
        reset => reset,
        waitrequest => waitrequest,
        address => address,
        read => read,
        readdata => readdata,

        red => reddata,
        green => greendata,
        blue => bluedata,
        getnext => getnextpixel,
        ledreturn => ledreturn
    );

    U1: ledDriver
    generic map (
        numberOfPixels   => numleds,
        returnticks => returnticks
    )
    port map    (

        ledclock => ledclk,
        ledout => ledout,
        reset => reset,
        red => reddata,
        green => greendata,
        blue => bluedata,
        getnext => getnextpixel,
        ledreturn => ledreturn
    );

end arch ; -- arch
