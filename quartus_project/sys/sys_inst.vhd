	component sys is
		port (
			clk_clk                          : in  std_logic                    := 'X'; -- clk
			master_0_master_reset_reset      : out std_logic;                           -- reset
			pio_0_external_connection_export : out std_logic_vector(7 downto 0);        -- export
			reset_reset_n                    : in  std_logic                    := 'X'  -- reset_n
		);
	end component sys;

	u0 : component sys
		port map (
			clk_clk                          => CONNECTED_TO_clk_clk,                          --                       clk.clk
			master_0_master_reset_reset      => CONNECTED_TO_master_0_master_reset_reset,      --     master_0_master_reset.reset
			pio_0_external_connection_export => CONNECTED_TO_pio_0_external_connection_export, -- pio_0_external_connection.export
			reset_reset_n                    => CONNECTED_TO_reset_reset_n                     --                     reset.reset_n
		);

