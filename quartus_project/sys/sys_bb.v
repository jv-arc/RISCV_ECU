
module sys (
	clk_clk,
	gpio_a_r_external_connection_export,
	gpio_a_s_external_connection_export,
	gpio_a_w_external_connection_export,
	gpio_b_r_external_connection_export,
	gpio_b_s_external_connection_export,
	gpio_b_w_external_connection_export,
	gpio_c0_w_external_connection_export,
	gpio_c1_w_external_connection_export,
	gpio_c2_w_external_connection_export,
	gpio_c_r_external_connection_export,
	master_0_master_reset_reset,
	pulpino_0_config_testmode_i,
	pulpino_0_config_fetch_enable_i,
	pulpino_0_config_clock_gating_i,
	pulpino_0_config_boot_addr_i,
	reset_reset_n,
	i2cslave_to_avlmm_bridge_0_conduit_end_conduit_data_in,
	i2cslave_to_avlmm_bridge_0_conduit_end_conduit_clk_in,
	i2cslave_to_avlmm_bridge_0_conduit_end_conduit_data_oe,
	i2cslave_to_avlmm_bridge_0_conduit_end_conduit_clk_oe);	

	input		clk_clk;
	input	[31:0]	gpio_a_r_external_connection_export;
	output	[31:0]	gpio_a_s_external_connection_export;
	output	[31:0]	gpio_a_w_external_connection_export;
	input	[31:0]	gpio_b_r_external_connection_export;
	output	[31:0]	gpio_b_s_external_connection_export;
	output	[31:0]	gpio_b_w_external_connection_export;
	output	[31:0]	gpio_c0_w_external_connection_export;
	output	[31:0]	gpio_c1_w_external_connection_export;
	output	[31:0]	gpio_c2_w_external_connection_export;
	input	[31:0]	gpio_c_r_external_connection_export;
	output		master_0_master_reset_reset;
	input		pulpino_0_config_testmode_i;
	input		pulpino_0_config_fetch_enable_i;
	input		pulpino_0_config_clock_gating_i;
	input	[31:0]	pulpino_0_config_boot_addr_i;
	input		reset_reset_n;
	input		i2cslave_to_avlmm_bridge_0_conduit_end_conduit_data_in;
	input		i2cslave_to_avlmm_bridge_0_conduit_end_conduit_clk_in;
	output		i2cslave_to_avlmm_bridge_0_conduit_end_conduit_data_oe;
	output		i2cslave_to_avlmm_bridge_0_conduit_end_conduit_clk_oe;
endmodule
