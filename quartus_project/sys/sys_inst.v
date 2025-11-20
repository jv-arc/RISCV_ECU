	sys u0 (
		.clk_clk                              (<connected-to-clk_clk>),                              //                           clk.clk
		.gpio_a_r_external_connection_export  (<connected-to-gpio_a_r_external_connection_export>),  //  gpio_a_r_external_connection.export
		.gpio_a_s_external_connection_export  (<connected-to-gpio_a_s_external_connection_export>),  //  gpio_a_s_external_connection.export
		.gpio_a_w_external_connection_export  (<connected-to-gpio_a_w_external_connection_export>),  //  gpio_a_w_external_connection.export
		.gpio_b_r_external_connection_export  (<connected-to-gpio_b_r_external_connection_export>),  //  gpio_b_r_external_connection.export
		.gpio_b_s_external_connection_export  (<connected-to-gpio_b_s_external_connection_export>),  //  gpio_b_s_external_connection.export
		.gpio_b_w_external_connection_export  (<connected-to-gpio_b_w_external_connection_export>),  //  gpio_b_w_external_connection.export
		.gpio_c0_w_external_connection_export (<connected-to-gpio_c0_w_external_connection_export>), // gpio_c0_w_external_connection.export
		.gpio_c1_w_external_connection_export (<connected-to-gpio_c1_w_external_connection_export>), // gpio_c1_w_external_connection.export
		.gpio_c2_w_external_connection_export (<connected-to-gpio_c2_w_external_connection_export>), // gpio_c2_w_external_connection.export
		.gpio_c_r_external_connection_export  (<connected-to-gpio_c_r_external_connection_export>),  //  gpio_c_r_external_connection.export
		.master_0_master_reset_reset          (<connected-to-master_0_master_reset_reset>),          //         master_0_master_reset.reset
		.pulpino_0_config_testmode_i          (<connected-to-pulpino_0_config_testmode_i>),          //              pulpino_0_config.testmode_i
		.pulpino_0_config_fetch_enable_i      (<connected-to-pulpino_0_config_fetch_enable_i>),      //                              .fetch_enable_i
		.pulpino_0_config_clock_gating_i      (<connected-to-pulpino_0_config_clock_gating_i>),      //                              .clock_gating_i
		.pulpino_0_config_boot_addr_i         (<connected-to-pulpino_0_config_boot_addr_i>),         //                              .boot_addr_i
		.reset_reset_n                        (<connected-to-reset_reset_n>)                         //                         reset.reset_n
	);

