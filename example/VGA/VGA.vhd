library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY VGA IS
PORT(
CLOCK_24: IN STD_LOGIC_VECTOR(1 downto 0);
VGA_HS,VGA_VS: OUT STD_LOGIC;
VGA_R,VGA_G,VGA_B: OUT STD_LOGIC_VECTOR(7 downto 0)
);
END VGA;

ARCHITECTURE MAIN OF VGA IS

SIGNAL VGACLK,RESET: STD_LOGIC:='0';
--------------------------------------------------------
    component PLL is
        port (
            inclk0   : in  std_logic := 'X'; -- clk
            c0  : out std_logic         -- clk
        );
    end component PLL;
--------------------------------------------------------
COMPONENT SYNC IS
PORT(
CLK: IN STD_LOGIC;
HSYNC,VSYNC: OUT STD_LOGIC;
R,G,B : OUT STD_LOGIC_VECTOR(7 downto 0)
);
END COMPONENT SYNC;
BEGIN
C1: SYNC PORT MAP(VGACLK,VGA_HS,VGA_VS,VGA_R,VGA_G,VGA_B);
C2: PLL PORT MAP(CLOCK_24(0),VGACLK);




END MAIN;
