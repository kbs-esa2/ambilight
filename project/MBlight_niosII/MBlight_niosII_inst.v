	MBlight_niosII u0 (
		.clk_clk                     (<connected-to-clk_clk>),                     //           clk.clk
		.i2c_sda_in                  (<connected-to-i2c_sda_in>),                  //           i2c.sda_in
		.i2c_scl_in                  (<connected-to-i2c_scl_in>),                  //              .scl_in
		.i2c_sda_oe                  (<connected-to-i2c_sda_oe>),                  //              .sda_oe
		.i2c_scl_oe                  (<connected-to-i2c_scl_oe>),                  //              .scl_oe
		.led_adress_export           (<connected-to-led_adress_export>),           //    led_adress.export
		.led_control_export          (<connected-to-led_control_export>),          //   led_control.export
		.led_data_export             (<connected-to-led_data_export>),             //      led_data.export
		.reset_reset_n               (<connected-to-reset_reset_n>),               //         reset.reset_n
		.sdram_io_addr               (<connected-to-sdram_io_addr>),               //      sdram_io.addr
		.sdram_io_ba                 (<connected-to-sdram_io_ba>),                 //              .ba
		.sdram_io_cas_n              (<connected-to-sdram_io_cas_n>),              //              .cas_n
		.sdram_io_cke                (<connected-to-sdram_io_cke>),                //              .cke
		.sdram_io_cs_n               (<connected-to-sdram_io_cs_n>),               //              .cs_n
		.sdram_io_dq                 (<connected-to-sdram_io_dq>),                 //              .dq
		.sdram_io_dqm                (<connected-to-sdram_io_dqm>),                //              .dqm
		.sdram_io_ras_n              (<connected-to-sdram_io_ras_n>),              //              .ras_n
		.sdram_io_we_n               (<connected-to-sdram_io_we_n>),               //              .we_n
		.sdram_control_address       (<connected-to-sdram_control_address>),       // sdram_control.address
		.sdram_control_byteenable_n  (<connected-to-sdram_control_byteenable_n>),  //              .byteenable_n
		.sdram_control_chipselect    (<connected-to-sdram_control_chipselect>),    //              .chipselect
		.sdram_control_writedata     (<connected-to-sdram_control_writedata>),     //              .writedata
		.sdram_control_read_n        (<connected-to-sdram_control_read_n>),        //              .read_n
		.sdram_control_write_n       (<connected-to-sdram_control_write_n>),       //              .write_n
		.sdram_control_readdata      (<connected-to-sdram_control_readdata>),      //              .readdata
		.sdram_control_readdatavalid (<connected-to-sdram_control_readdatavalid>), //              .readdatavalid
		.sdram_control_waitrequest   (<connected-to-sdram_control_waitrequest>)    //              .waitrequest
	);

