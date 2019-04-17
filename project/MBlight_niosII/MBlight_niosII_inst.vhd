	component MBlight_niosII is
		port (
			clk_clk                     : in    std_logic                     := 'X';             -- clk
			i2c_sda_in                  : in    std_logic                     := 'X';             -- sda_in
			i2c_scl_in                  : in    std_logic                     := 'X';             -- scl_in
			i2c_sda_oe                  : out   std_logic;                                        -- sda_oe
			i2c_scl_oe                  : out   std_logic;                                        -- scl_oe
			led_adress_export           : out   std_logic_vector(7 downto 0);                     -- export
			led_control_export          : in    std_logic_vector(7 downto 0)  := (others => 'X'); -- export
			led_data_export             : out   std_logic_vector(7 downto 0);                     -- export
			reset_reset_n               : in    std_logic                     := 'X';             -- reset_n
			sdram_io_addr               : out   std_logic_vector(10 downto 0);                    -- addr
			sdram_io_ba                 : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_io_cas_n              : out   std_logic;                                        -- cas_n
			sdram_io_cke                : out   std_logic;                                        -- cke
			sdram_io_cs_n               : out   std_logic;                                        -- cs_n
			sdram_io_dq                 : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_io_dqm                : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_io_ras_n              : out   std_logic;                                        -- ras_n
			sdram_io_we_n               : out   std_logic;                                        -- we_n
			sdram_control_address       : in    std_logic_vector(21 downto 0) := (others => 'X'); -- address
			sdram_control_byteenable_n  : in    std_logic_vector(1 downto 0)  := (others => 'X'); -- byteenable_n
			sdram_control_chipselect    : in    std_logic                     := 'X';             -- chipselect
			sdram_control_writedata     : in    std_logic_vector(15 downto 0) := (others => 'X'); -- writedata
			sdram_control_read_n        : in    std_logic                     := 'X';             -- read_n
			sdram_control_write_n       : in    std_logic                     := 'X';             -- write_n
			sdram_control_readdata      : out   std_logic_vector(15 downto 0);                    -- readdata
			sdram_control_readdatavalid : out   std_logic;                                        -- readdatavalid
			sdram_control_waitrequest   : out   std_logic                                         -- waitrequest
		);
	end component MBlight_niosII;

	u0 : component MBlight_niosII
		port map (
			clk_clk                     => CONNECTED_TO_clk_clk,                     --           clk.clk
			i2c_sda_in                  => CONNECTED_TO_i2c_sda_in,                  --           i2c.sda_in
			i2c_scl_in                  => CONNECTED_TO_i2c_scl_in,                  --              .scl_in
			i2c_sda_oe                  => CONNECTED_TO_i2c_sda_oe,                  --              .sda_oe
			i2c_scl_oe                  => CONNECTED_TO_i2c_scl_oe,                  --              .scl_oe
			led_adress_export           => CONNECTED_TO_led_adress_export,           --    led_adress.export
			led_control_export          => CONNECTED_TO_led_control_export,          --   led_control.export
			led_data_export             => CONNECTED_TO_led_data_export,             --      led_data.export
			reset_reset_n               => CONNECTED_TO_reset_reset_n,               --         reset.reset_n
			sdram_io_addr               => CONNECTED_TO_sdram_io_addr,               --      sdram_io.addr
			sdram_io_ba                 => CONNECTED_TO_sdram_io_ba,                 --              .ba
			sdram_io_cas_n              => CONNECTED_TO_sdram_io_cas_n,              --              .cas_n
			sdram_io_cke                => CONNECTED_TO_sdram_io_cke,                --              .cke
			sdram_io_cs_n               => CONNECTED_TO_sdram_io_cs_n,               --              .cs_n
			sdram_io_dq                 => CONNECTED_TO_sdram_io_dq,                 --              .dq
			sdram_io_dqm                => CONNECTED_TO_sdram_io_dqm,                --              .dqm
			sdram_io_ras_n              => CONNECTED_TO_sdram_io_ras_n,              --              .ras_n
			sdram_io_we_n               => CONNECTED_TO_sdram_io_we_n,               --              .we_n
			sdram_control_address       => CONNECTED_TO_sdram_control_address,       -- sdram_control.address
			sdram_control_byteenable_n  => CONNECTED_TO_sdram_control_byteenable_n,  --              .byteenable_n
			sdram_control_chipselect    => CONNECTED_TO_sdram_control_chipselect,    --              .chipselect
			sdram_control_writedata     => CONNECTED_TO_sdram_control_writedata,     --              .writedata
			sdram_control_read_n        => CONNECTED_TO_sdram_control_read_n,        --              .read_n
			sdram_control_write_n       => CONNECTED_TO_sdram_control_write_n,       --              .write_n
			sdram_control_readdata      => CONNECTED_TO_sdram_control_readdata,      --              .readdata
			sdram_control_readdatavalid => CONNECTED_TO_sdram_control_readdatavalid, --              .readdatavalid
			sdram_control_waitrequest   => CONNECTED_TO_sdram_control_waitrequest    --              .waitrequest
		);

