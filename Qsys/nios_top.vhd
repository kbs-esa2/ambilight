-- Implements a simple Nios II system for the DE-series board

library ieee; 
USE ieee.std_logic_1164.ALL; 
USE ieee.std_logic_unsigned.ALL;

entity nios_top IS     --name of top level entity
  port ( 
    CLOCK_50 : IN STD_LOGIC; 
    KEY      : IN STD_LOGIC_VECTOR (0 DOWNTO 0); 
    SW       : IN STD_LOGIC_VECTOR (17 DOWNTO 0); 
    LEDR     : OUT STD_LOGIC_VECTOR (17 DOWNTO 0) ); 
END nios_top; 

ARCHITECTURE NiosQsysDemo_rtl OF nios_top IS 

COMPONENT nios2 
PORT (
 SIGNAL clk_clk: IN STD_LOGIC; 
 SIGNAL reset_reset_n : IN STD_LOGIC; 
 SIGNAL switches_export : IN STD_LOGIC_VECTOR (17 DOWNTO 0); 
 SIGNAL leds_export : OUT STD_LOGIC_VECTOR (17 DOWNTO 0) ); 
END COMPONENT; 
 
BEGIN 
 NiosII : nios2 PORT MAP(
				clk_clk => CLOCK_50, 
				reset_reset_n => KEY(0), 
				switches_export => SW(17 DOWNTO 0), 
				leds_export => LEDR(17 DOWNTO 0) 
			); 
 END NiosQsysDemo_rtl;