-- Implements a simple Nios II system for the DE-series board.
-- CLOCK_50 is the system clock
-- KEY0 is the active-low system reset
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.ALL;

ENTITY MBlight IS
PORT (
CLOCK_50 : IN std_logic;
KEY : IN std_logic_vector (3 DOWNTO 0);
LEDR : OUT std_logic_vector (17 downto 0);
LEDG : out std_logic_vector (7 downto 0)
);
END MBlight;

ARCHITECTURE MBlight_rtl OF MBlight IS

COMPONENT MBlight_niosII
port (
		clk_clk            : in  std_logic                    := '0';             --         clk.clk
		i2c_sda_in         : in  std_logic                    := '0';             --         i2c.sda_in
		i2c_scl_in         : in  std_logic                    := '0';             --            .scl_in
		i2c_sda_oe         : out std_logic;                                       --            .sda_oe
		i2c_scl_oe         : out std_logic;                                       --            .scl_oe
		led_adress_export  : out std_logic_vector(7 downto 0);                    --  led_adress.export
		led_control_export : in  std_logic_vector(7 downto 0) := (others => '0'); -- led_control.export
		led_data_export    : out std_logic_vector(7 downto 0);                    --    led_data.export
		reset_reset_n      : in  std_logic                    := '0'              --       reset.reset_n
	);
END COMPONENT;

BEGIN
NiosII : MBlight_niosII
PORT MAP(
clk_clk => CLOCK_50,
reset_reset_n => KEY(0)
);
LEDG(0) <= CLOCK_50;
END MBlight_rtl;