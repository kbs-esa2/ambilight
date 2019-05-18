	component nios2 is
		port (
			av_config_wire_SDAT          : inout std_logic                     := 'X';             -- SDAT
			av_config_wire_SCLK          : out   std_logic;                                        -- SCLK
			clk_clk                      : in    std_logic                     := 'X';             -- clk
			decoder_wire_TD_CLK27        : in    std_logic                     := 'X';             -- TD_CLK27
			decoder_wire_TD_DATA         : in    std_logic_vector(7 downto 0)  := (others => 'X'); -- TD_DATA
			decoder_wire_TD_HS           : in    std_logic                     := 'X';             -- TD_HS
			decoder_wire_TD_VS           : in    std_logic                     := 'X';             -- TD_VS
			decoder_wire_clk27_reset     : in    std_logic                     := 'X';             -- clk27_reset
			decoder_wire_TD_RESET        : out   std_logic;                                        -- TD_RESET
			decoder_wire_overflow_flag   : out   std_logic;                                        -- overflow_flag
			keys_export                  : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			led_wire_adressable_led_data : out   std_logic;                                        -- adressable_led_data
			leds_export                  : out   std_logic_vector(17 downto 0);                    -- export
			reset_reset_n                : in    std_logic                     := 'X';             -- reset_n
			sdram_clk_clk                : out   std_logic;                                        -- clk
			sdram_wire_addr              : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_wire_ba                : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_wire_cas_n             : out   std_logic;                                        -- cas_n
			sdram_wire_cke               : out   std_logic;                                        -- cke
			sdram_wire_cs_n              : out   std_logic;                                        -- cs_n
			sdram_wire_dq                : inout std_logic_vector(31 downto 0) := (others => 'X'); -- dq
			sdram_wire_dqm               : out   std_logic_vector(3 downto 0);                     -- dqm
			sdram_wire_ras_n             : out   std_logic;                                        -- ras_n
			sdram_wire_we_n              : out   std_logic;                                        -- we_n
			sram_wire_DQ                 : inout std_logic_vector(15 downto 0) := (others => 'X'); -- DQ
			sram_wire_ADDR               : out   std_logic_vector(19 downto 0);                    -- ADDR
			sram_wire_LB_N               : out   std_logic;                                        -- LB_N
			sram_wire_UB_N               : out   std_logic;                                        -- UB_N
			sram_wire_CE_N               : out   std_logic;                                        -- CE_N
			sram_wire_OE_N               : out   std_logic;                                        -- OE_N
			sram_wire_WE_N               : out   std_logic;                                        -- WE_N
			switches_export              : in    std_logic_vector(17 downto 0) := (others => 'X'); -- export
			vga_wire_CLK                 : out   std_logic;                                        -- CLK
			vga_wire_HS                  : out   std_logic;                                        -- HS
			vga_wire_VS                  : out   std_logic;                                        -- VS
			vga_wire_BLANK               : out   std_logic;                                        -- BLANK
			vga_wire_SYNC                : out   std_logic;                                        -- SYNC
			vga_wire_R                   : out   std_logic_vector(7 downto 0);                     -- R
			vga_wire_G                   : out   std_logic_vector(7 downto 0);                     -- G
			vga_wire_B                   : out   std_logic_vector(7 downto 0)                      -- B
		);
	end component nios2;

	u0 : component nios2
		port map (
			av_config_wire_SDAT          => CONNECTED_TO_av_config_wire_SDAT,          -- av_config_wire.SDAT
			av_config_wire_SCLK          => CONNECTED_TO_av_config_wire_SCLK,          --               .SCLK
			clk_clk                      => CONNECTED_TO_clk_clk,                      --            clk.clk
			decoder_wire_TD_CLK27        => CONNECTED_TO_decoder_wire_TD_CLK27,        --   decoder_wire.TD_CLK27
			decoder_wire_TD_DATA         => CONNECTED_TO_decoder_wire_TD_DATA,         --               .TD_DATA
			decoder_wire_TD_HS           => CONNECTED_TO_decoder_wire_TD_HS,           --               .TD_HS
			decoder_wire_TD_VS           => CONNECTED_TO_decoder_wire_TD_VS,           --               .TD_VS
			decoder_wire_clk27_reset     => CONNECTED_TO_decoder_wire_clk27_reset,     --               .clk27_reset
			decoder_wire_TD_RESET        => CONNECTED_TO_decoder_wire_TD_RESET,        --               .TD_RESET
			decoder_wire_overflow_flag   => CONNECTED_TO_decoder_wire_overflow_flag,   --               .overflow_flag
			keys_export                  => CONNECTED_TO_keys_export,                  --           keys.export
			led_wire_adressable_led_data => CONNECTED_TO_led_wire_adressable_led_data, --       led_wire.adressable_led_data
			leds_export                  => CONNECTED_TO_leds_export,                  --           leds.export
			reset_reset_n                => CONNECTED_TO_reset_reset_n,                --          reset.reset_n
			sdram_clk_clk                => CONNECTED_TO_sdram_clk_clk,                --      sdram_clk.clk
			sdram_wire_addr              => CONNECTED_TO_sdram_wire_addr,              --     sdram_wire.addr
			sdram_wire_ba                => CONNECTED_TO_sdram_wire_ba,                --               .ba
			sdram_wire_cas_n             => CONNECTED_TO_sdram_wire_cas_n,             --               .cas_n
			sdram_wire_cke               => CONNECTED_TO_sdram_wire_cke,               --               .cke
			sdram_wire_cs_n              => CONNECTED_TO_sdram_wire_cs_n,              --               .cs_n
			sdram_wire_dq                => CONNECTED_TO_sdram_wire_dq,                --               .dq
			sdram_wire_dqm               => CONNECTED_TO_sdram_wire_dqm,               --               .dqm
			sdram_wire_ras_n             => CONNECTED_TO_sdram_wire_ras_n,             --               .ras_n
			sdram_wire_we_n              => CONNECTED_TO_sdram_wire_we_n,              --               .we_n
			sram_wire_DQ                 => CONNECTED_TO_sram_wire_DQ,                 --      sram_wire.DQ
			sram_wire_ADDR               => CONNECTED_TO_sram_wire_ADDR,               --               .ADDR
			sram_wire_LB_N               => CONNECTED_TO_sram_wire_LB_N,               --               .LB_N
			sram_wire_UB_N               => CONNECTED_TO_sram_wire_UB_N,               --               .UB_N
			sram_wire_CE_N               => CONNECTED_TO_sram_wire_CE_N,               --               .CE_N
			sram_wire_OE_N               => CONNECTED_TO_sram_wire_OE_N,               --               .OE_N
			sram_wire_WE_N               => CONNECTED_TO_sram_wire_WE_N,               --               .WE_N
			switches_export              => CONNECTED_TO_switches_export,              --       switches.export
			vga_wire_CLK                 => CONNECTED_TO_vga_wire_CLK,                 --       vga_wire.CLK
			vga_wire_HS                  => CONNECTED_TO_vga_wire_HS,                  --               .HS
			vga_wire_VS                  => CONNECTED_TO_vga_wire_VS,                  --               .VS
			vga_wire_BLANK               => CONNECTED_TO_vga_wire_BLANK,               --               .BLANK
			vga_wire_SYNC                => CONNECTED_TO_vga_wire_SYNC,                --               .SYNC
			vga_wire_R                   => CONNECTED_TO_vga_wire_R,                   --               .R
			vga_wire_G                   => CONNECTED_TO_vga_wire_G,                   --               .G
			vga_wire_B                   => CONNECTED_TO_vga_wire_B                    --               .B
		);

