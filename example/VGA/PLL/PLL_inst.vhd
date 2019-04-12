	component PLL is
		port (
			clkin_clk   : in  std_logic := 'X'; -- clk
			reset_reset : in  std_logic := 'X'; -- reset
			clkout_clk  : out std_logic         -- clk
		);
	end component PLL;

	u0 : component PLL
		port map (
			clkin_clk   => CONNECTED_TO_clkin_clk,   --  clkin.clk
			reset_reset => CONNECTED_TO_reset_reset, --  reset.reset
			clkout_clk  => CONNECTED_TO_clkout_clk   -- clkout.clk
		);

