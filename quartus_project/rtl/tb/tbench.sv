`timescale 1ns/10ps

module tbench(
	//so empty :3
);
	

//==================================================
//                     SETUP
//==================================================

	// fixed during simulation
	parameter BOOT_ADDR = 32'h00008000;
	parameter clk_period = 10; // 25MHz
	
	wire test_mode;
	wire fetch_enable;
	wire clock_gating;
	
	assign test_mode = 1'b0;
	assign fetch_enable = 1'b1;
	assign clock_gating = 1'b0;
	
	
	
	// changing during the simulation
	reg tb_clk;
	reg jtag_reset;
	reg reset_n;
	reg [31:0] gpio_in;
	wire [31:0] gpio_out;
	
	


// Instatiating core (no PLL)
pulpino_qsys_test dut (
    .CLOCK_50                            (tb_clk),
    .KEY                                 ({3'b0, ~reset_n}),
    .SW                                  (gpio_in[9:0]),
    .LEDR                                (gpio_out[9:0])
);



//==================================================
//                   SIMULATION
//==================================================

initial begin

   // Initial Conditions
   tb_clk = 0;
   reset_n = 1'b0;
   gpio_in = 32'b0000000000000000000000000001111; // [3:0] active low
   
	 
	 
  #50 //---------------------

	
	
	// Turning on the core
   reset_n = 1'b1;

	
	
   #500 //---------------------

	 
	 
   // End simulation
   $stop;

end


// Clock Generation
always begin
        #(clk_period/2) tb_clk = ~tb_clk;
end

endmodule
