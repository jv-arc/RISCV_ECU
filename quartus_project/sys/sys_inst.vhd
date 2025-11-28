	component sys is
		port (
			clk_clk                                                : in  std_logic                     := 'X';             -- clk
			gpio_a_r_external_connection_export                    : in  std_logic_vector(31 downto 0) := (others => 'X'); -- export
			gpio_a_s_external_connection_export                    : out std_logic_vector(31 downto 0);                    -- export
			gpio_a_w_external_connection_export                    : out std_logic_vector(31 downto 0);                    -- export
			gpio_b_r_external_connection_export                    : in  std_logic_vector(31 downto 0) := (others => 'X'); -- export
			gpio_b_s_external_connection_export                    : out std_logic_vector(31 downto 0);                    -- export
			gpio_b_w_external_connection_export                    : out std_logic_vector(31 downto 0);                    -- export
			gpio_c0_w_external_connection_export                   : out std_logic_vector(31 downto 0);                    -- export
			gpio_c1_w_external_connection_export                   : out std_logic_vector(31 downto 0);                    -- export
			gpio_c2_w_external_connection_export                   : out std_logic_vector(31 downto 0);                    -- export
			gpio_c_r_external_connection_export                    : in  std_logic_vector(31 downto 0) := (others => 'X'); -- export
			master_0_master_reset_reset                            : out std_logic;                                        -- reset
			pulpino_0_config_testmode_i                            : in  std_logic                     := 'X';             -- testmode_i
			pulpino_0_config_fetch_enable_i                        : in  std_logic                     := 'X';             -- fetch_enable_i
			pulpino_0_config_clock_gating_i                        : in  std_logic                     := 'X';             -- clock_gating_i
			pulpino_0_config_boot_addr_i                           : in  std_logic_vector(31 downto 0) := (others => 'X'); -- boot_addr_i
			reset_reset_n                                          : in  std_logic                     := 'X';             -- reset_n
			i2cslave_to_avlmm_bridge_0_conduit_end_conduit_data_in : in  std_logic                     := 'X';             -- conduit_data_in
			i2cslave_to_avlmm_bridge_0_conduit_end_conduit_clk_in  : in  std_logic                     := 'X';             -- conduit_clk_in
			i2cslave_to_avlmm_bridge_0_conduit_end_conduit_data_oe : out std_logic;                                        -- conduit_data_oe
			i2cslave_to_avlmm_bridge_0_conduit_end_conduit_clk_oe  : out std_logic                                         -- conduit_clk_oe
		);
	end component sys;

	u0 : component sys
		port map (
			clk_clk                                                => CONNECTED_TO_clk_clk,                                                --                                    clk.clk
			gpio_a_r_external_connection_export                    => CONNECTED_TO_gpio_a_r_external_connection_export,                    --           gpio_a_r_external_connection.export
			gpio_a_s_external_connection_export                    => CONNECTED_TO_gpio_a_s_external_connection_export,                    --           gpio_a_s_external_connection.export
			gpio_a_w_external_connection_export                    => CONNECTED_TO_gpio_a_w_external_connection_export,                    --           gpio_a_w_external_connection.export
			gpio_b_r_external_connection_export                    => CONNECTED_TO_gpio_b_r_external_connection_export,                    --           gpio_b_r_external_connection.export
			gpio_b_s_external_connection_export                    => CONNECTED_TO_gpio_b_s_external_connection_export,                    --           gpio_b_s_external_connection.export
			gpio_b_w_external_connection_export                    => CONNECTED_TO_gpio_b_w_external_connection_export,                    --           gpio_b_w_external_connection.export
			gpio_c0_w_external_connection_export                   => CONNECTED_TO_gpio_c0_w_external_connection_export,                   --          gpio_c0_w_external_connection.export
			gpio_c1_w_external_connection_export                   => CONNECTED_TO_gpio_c1_w_external_connection_export,                   --          gpio_c1_w_external_connection.export
			gpio_c2_w_external_connection_export                   => CONNECTED_TO_gpio_c2_w_external_connection_export,                   --          gpio_c2_w_external_connection.export
			gpio_c_r_external_connection_export                    => CONNECTED_TO_gpio_c_r_external_connection_export,                    --           gpio_c_r_external_connection.export
			master_0_master_reset_reset                            => CONNECTED_TO_master_0_master_reset_reset,                            --                  master_0_master_reset.reset
			pulpino_0_config_testmode_i                            => CONNECTED_TO_pulpino_0_config_testmode_i,                            --                       pulpino_0_config.testmode_i
			pulpino_0_config_fetch_enable_i                        => CONNECTED_TO_pulpino_0_config_fetch_enable_i,                        --                                       .fetch_enable_i
			pulpino_0_config_clock_gating_i                        => CONNECTED_TO_pulpino_0_config_clock_gating_i,                        --                                       .clock_gating_i
			pulpino_0_config_boot_addr_i                           => CONNECTED_TO_pulpino_0_config_boot_addr_i,                           --                                       .boot_addr_i
			reset_reset_n                                          => CONNECTED_TO_reset_reset_n,                                          --                                  reset.reset_n
			i2cslave_to_avlmm_bridge_0_conduit_end_conduit_data_in => CONNECTED_TO_i2cslave_to_avlmm_bridge_0_conduit_end_conduit_data_in, -- i2cslave_to_avlmm_bridge_0_conduit_end.conduit_data_in
			i2cslave_to_avlmm_bridge_0_conduit_end_conduit_clk_in  => CONNECTED_TO_i2cslave_to_avlmm_bridge_0_conduit_end_conduit_clk_in,  --                                       .conduit_clk_in
			i2cslave_to_avlmm_bridge_0_conduit_end_conduit_data_oe => CONNECTED_TO_i2cslave_to_avlmm_bridge_0_conduit_end_conduit_data_oe, --                                       .conduit_data_oe
			i2cslave_to_avlmm_bridge_0_conduit_end_conduit_clk_oe  => CONNECTED_TO_i2cslave_to_avlmm_bridge_0_conduit_end_conduit_clk_oe   --                                       .conduit_clk_oe
		);

