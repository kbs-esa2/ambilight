
module MBlight_niosII (
	clk_clk,
	i2c_sda_in,
	i2c_scl_in,
	i2c_sda_oe,
	i2c_scl_oe,
	led_adress_export,
	led_control_export,
	led_data_export,
	reset_reset_n,
	sdram_io_addr,
	sdram_io_ba,
	sdram_io_cas_n,
	sdram_io_cke,
	sdram_io_cs_n,
	sdram_io_dq,
	sdram_io_dqm,
	sdram_io_ras_n,
	sdram_io_we_n,
	sdram_control_address,
	sdram_control_byteenable_n,
	sdram_control_chipselect,
	sdram_control_writedata,
	sdram_control_read_n,
	sdram_control_write_n,
	sdram_control_readdata,
	sdram_control_readdatavalid,
	sdram_control_waitrequest);	

	input		clk_clk;
	input		i2c_sda_in;
	input		i2c_scl_in;
	output		i2c_sda_oe;
	output		i2c_scl_oe;
	output	[7:0]	led_adress_export;
	input	[7:0]	led_control_export;
	output	[7:0]	led_data_export;
	input		reset_reset_n;
	output	[10:0]	sdram_io_addr;
	output	[1:0]	sdram_io_ba;
	output		sdram_io_cas_n;
	output		sdram_io_cke;
	output		sdram_io_cs_n;
	inout	[15:0]	sdram_io_dq;
	output	[1:0]	sdram_io_dqm;
	output		sdram_io_ras_n;
	output		sdram_io_we_n;
	input	[21:0]	sdram_control_address;
	input	[1:0]	sdram_control_byteenable_n;
	input		sdram_control_chipselect;
	input	[15:0]	sdram_control_writedata;
	input		sdram_control_read_n;
	input		sdram_control_write_n;
	output	[15:0]	sdram_control_readdata;
	output		sdram_control_readdatavalid;
	output		sdram_control_waitrequest;
endmodule
