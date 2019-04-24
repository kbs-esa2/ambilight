-- Implements a simple Nios II system for the DE-series board

library ieee; 
USE ieee.std_logic_1164.ALL; 
USE ieee.std_logic_unsigned.ALL;

entity nios_top IS     --name of top level entity
  port ( 
		--clock and reset--
		CLOCK_50 	: IN STD_LOGIC; 
		KEY      	: IN STD_LOGIC_VECTOR (3 DOWNTO 0); 
		--debug io--
		SW       	: IN STD_LOGIC_VECTOR (17 DOWNTO 0); 
		LEDR     	: OUT STD_LOGIC_VECTOR (17 DOWNTO 0);
		--SDRAM connections--
		DRAM_DQ		: inout STD_LOGIC_VECTOR (31 DOWNTO 0);
		DRAM_DQM		: out STD_LOGIC_VECTOR (3 DOWNTO 0);
		DRAM_ADDR	: out STD_LOGIC_VECTOR (12 DOWNTO 0);
		DRAM_BA		: out STD_LOGIC_VECTOR (1 DOWNTO 0);
		DRAM_CKE		: out STD_LOGIC;
		DRAM_CLK		: out STD_LOGIC;
		DRAM_WE_N	: out STD_LOGIC;
		DRAM_CAS_N	: out STD_LOGIC;
		DRAM_RAS_N	: out STD_LOGIC;
		DRAM_CS_N	: out STD_LOGIC;
		--SRAM connections--for vga buffer
		SRAM_DQ               : inout std_logic_vector(15 downto 0) := (others => '0'); --      sram_wire.DQ
		SRAM_ADDR             : out   std_logic_vector(19 downto 0);                    --               .ADDR
		SRAM_LB_N             : out   std_logic;                                        --               .LB_N
		SRAM_UB_N             : out   std_logic;                                        --               .UB_N
		SRAM_CE_N             : out   std_logic;                                        --               .CE_N
		SRAM_OE_N             : out   std_logic;                                        --               .OE_N
		SRAM_WE_N             : out   std_logic;                                        --               .WE_N
		
		--decoder connections--
		TD_CLK27      : in    std_logic                     := '0';             -- decoder_wire.TD_CLK27
		TD_DATA       : in    std_logic_vector(7 downto 0)  := (others => '0'); --             .TD_DATA
		TD_HS         : in    std_logic                     := '0';             --             .TD_HS
		TD_VS         : in    std_logic                     := '0';             --             .TD_VS
		TD_RESET_N      : out   std_logic;                                        --             .TD_RESET 
		--vga connections--
		VGA_CLK       : out   std_logic;                                        --     vga_wire.CLK
		VGA_HS        : out   std_logic;                                        --             .HS
		VGA_VS        : out   std_logic;                                        --             .VS
		VGA_BLANK_N   : out   std_logic;                                        --             .BLANK
		VGA_SYNC_N    : out   std_logic;                                        --             .SYNC
		VGA_R         : out   std_logic_vector(7 downto 0);                     --             .R
		VGA_G         : out   std_logic_vector(7 downto 0);                     --             .G
		VGA_B         : out   std_logic_vector(7 downto 0);
		--I2C bus--
		I2C_SCLK			: out   std_logic;
		I2C_SDAT			: inout std_logic := '0' 	
	); 
END nios_top; 

ARCHITECTURE NiosQsysDemo_rtl OF nios_top IS 

COMPONENT nios2 
port (
		av_config_wire_SDAT        : inout std_logic                     := '0';             -- av_config_wire.SDAT
		av_config_wire_SCLK        : out   std_logic;                                        --               .SCLK
		clk_clk                    : in    std_logic                     := '0';             --            clk.clk
		decoder_wire_TD_CLK27      : in    std_logic                     := '0';             --   decoder_wire.TD_CLK27
		decoder_wire_TD_DATA       : in    std_logic_vector(7 downto 0)  := (others => '0'); --               .TD_DATA
		decoder_wire_TD_HS         : in    std_logic                     := '0';             --               .TD_HS
		decoder_wire_TD_VS         : in    std_logic                     := '0';             --               .TD_VS
		decoder_wire_clk27_reset   : in    std_logic                     := '0';             --               .clk27_reset
		decoder_wire_TD_RESET      : out   std_logic;                                        --               .TD_RESET
		decoder_wire_overflow_flag : out   std_logic;                                        --               .overflow_flag
		keys_export                : in    std_logic_vector(3 downto 0)  := (others => '0'); --           keys.export
		leds_export                : out   std_logic_vector(17 downto 0);                    --           leds.export
		reset_reset_n              : in    std_logic                     := '0';             --          reset.reset_n
		sdram_clk_clk              : out   std_logic;                                        --      sdram_clk.clk
		sdram_wire_addr            : out   std_logic_vector(12 downto 0);                    --     sdram_wire.addr
		sdram_wire_ba              : out   std_logic_vector(1 downto 0);                     --               .ba
		sdram_wire_cas_n           : out   std_logic;                                        --               .cas_n
		sdram_wire_cke             : out   std_logic;                                        --               .cke
		sdram_wire_cs_n            : out   std_logic;                                        --               .cs_n
		sdram_wire_dq              : inout std_logic_vector(31 downto 0) := (others => '0'); --               .dq
		sdram_wire_dqm             : out   std_logic_vector(3 downto 0);                     --               .dqm
		sdram_wire_ras_n           : out   std_logic;                                        --               .ras_n
		sdram_wire_we_n            : out   std_logic;                                        --               .we_n
		sram_wire_DQ               : inout std_logic_vector(15 downto 0) := (others => '0'); --      sram_wire.DQ
		sram_wire_ADDR             : out   std_logic_vector(19 downto 0);                    --               .ADDR
		sram_wire_LB_N             : out   std_logic;                                        --               .LB_N
		sram_wire_UB_N             : out   std_logic;                                        --               .UB_N
		sram_wire_CE_N             : out   std_logic;                                        --               .CE_N
		sram_wire_OE_N             : out   std_logic;                                        --               .OE_N
		sram_wire_WE_N             : out   std_logic;                                        --               .WE_N
		switches_export            : in    std_logic_vector(17 downto 0) := (others => '0'); --       switches.export
		vga_wire_CLK               : out   std_logic;                                        --       vga_wire.CLK
		vga_wire_HS                : out   std_logic;                                        --               .HS
		vga_wire_VS                : out   std_logic;                                        --               .VS
		vga_wire_BLANK             : out   std_logic;                                        --               .BLANK
		vga_wire_SYNC              : out   std_logic;                                        --               .SYNC
		vga_wire_R                 : out   std_logic_vector(7 downto 0);                     --               .R
		vga_wire_G                 : out   std_logic_vector(7 downto 0);                     --               .G
		vga_wire_B                 : out   std_logic_vector(7 downto 0)                      --               .B
	);
END COMPONENT; 
 
BEGIN 
 nios : nios2 PORT MAP(
			clk_clk => CLOCK_50, 
			reset_reset_n => KEY(0), 
				
			switches_export => SW(17 DOWNTO 0), 
			leds_export => LEDR(17 DOWNTO 0),
			keys_export => KEY(3 downto 0),
				
			sdram_clk_clk => DRAM_CLK,
			sdram_wire_addr => DRAM_ADDR,
			sdram_wire_ba => DRAM_BA,
			sdram_wire_cas_n => DRAM_CAS_N,
			sdram_wire_cke => DRAM_CKE,
			sdram_wire_cs_n => DRAM_CS_N,
			sdram_wire_dq => DRAM_DQ,
			sdram_wire_dqm => DRAM_DQM,
			sdram_wire_ras_n => DRAM_RAS_N,
			sdram_wire_we_n => DRAM_WE_N,
			
			sram_wire_ADDR => SRAM_ADDR,
			sram_wire_DQ => SRAM_DQ,
			sram_wire_CE_N => SRAM_CE_N,
			sram_wire_LB_N => SRAM_LB_N,
			sram_wire_OE_N => SRAM_OE_N,
			sram_wire_UB_N => SRAM_UB_N,
			sram_wire_WE_N => SRAM_WE_N,
			
			decoder_wire_TD_CLK27 => TD_CLK27,
			decoder_wire_TD_DATA => TD_DATA,
			decoder_wire_TD_HS => TD_HS,
			decoder_wire_TD_VS => TD_VS,
			decoder_wire_TD_RESET => TD_RESET_N,
			
			vga_wire_CLK => VGA_CLK,
			vga_wire_HS => VGA_HS,
			vga_wire_VS => VGA_VS,
			vga_wire_BLANK => VGA_BLANK_N,
			vga_wire_SYNC => VGA_SYNC_N,
			vga_wire_R => VGA_R,
			vga_wire_G => VGA_G,
			vga_wire_B => VGA_B,
			
			av_config_wire_SDAT => I2C_SDAT,
			av_config_wire_SCLK => I2C_SCLK
			); 
 END NiosQsysDemo_rtl;