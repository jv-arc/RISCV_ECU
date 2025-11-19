	sys u0 (
		.clk_clk                               (<connected-to-clk_clk>),                               //                            clk.clk
		.gpio_a_dir_external_connection_export (<connected-to-gpio_a_dir_external_connection_export>), // gpio_a_dir_external_connection.export
		.gpio_a_r_external_connection_export   (<connected-to-gpio_a_r_external_connection_export>),   //   gpio_a_r_external_connection.export
		.gpio_a_w_external_connection_export   (<connected-to-gpio_a_w_external_connection_export>),   //   gpio_a_w_external_connection.export
		.gpio_b_dir_external_connection_export (<connected-to-gpio_b_dir_external_connection_export>), // gpio_b_dir_external_connection.export
		.gpio_b_r_external_connection_export   (<connected-to-gpio_b_r_external_connection_export>),   //   gpio_b_r_external_connection.export
		.gpio_b_w_external_connection_export   (<connected-to-gpio_b_w_external_connection_export>),   //   gpio_b_w_external_connection.export
		.master_0_master_reset_reset           (<connected-to-master_0_master_reset_reset>),           //          master_0_master_reset.reset
		.pio_r_one_external_connection_export  (<connected-to-pio_r_one_external_connection_export>),  //  pio_r_one_external_connection.export
		.pio_w_one_external_connection_export  (<connected-to-pio_w_one_external_connection_export>),  //  pio_w_one_external_connection.export
		.pio_w_two_external_connection_export  (<connected-to-pio_w_two_external_connection_export>),  //  pio_w_two_external_connection.export
		.pulpino_0_config_testmode_i           (<connected-to-pulpino_0_config_testmode_i>),           //               pulpino_0_config.testmode_i
		.pulpino_0_config_fetch_enable_i       (<connected-to-pulpino_0_config_fetch_enable_i>),       //                               .fetch_enable_i
		.pulpino_0_config_clock_gating_i       (<connected-to-pulpino_0_config_clock_gating_i>),       //                               .clock_gating_i
		.pulpino_0_config_boot_addr_i          (<connected-to-pulpino_0_config_boot_addr_i>),          //                               .boot_addr_i
		.reset_reset_n                         (<connected-to-reset_reset_n>)                          //                          reset.reset_n
	);

