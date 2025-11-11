`timescale 1ns/10ps

module tbench;
	// `include "flag_values.svh"
	
	parameter NORMAL = 8'h00;
	parameter SUCCESS = 8'hF0;
	parameter FAILURE = 8'hFF;

	parameter MAIN = 8'h00;
	parameter WHILE = 8'h01;
	parameter FUNC = 8'h02;
	parameter SETUP = 8'h03;
	parameter ISR = 8'h04;

	//==================================================
	//                 BASIC SETUP
	//==================================================

	// fixed during simulation
	parameter BOOT_ADDR = 32'h00008000;
	parameter clk_period = 20; // 50MHz
	parameter cpu_period = clk_period*2;
	
	wire test_mode;
	wire fetch_enable;
	wire clock_gating;
	
	assign test_mode = 1'b0;
	assign fetch_enable = 1'b1;
	assign clock_gating = 1'b0;
	
	// changing during the simulation
	reg  tb_clk;
	reg  jtag_reset;
	reg  key_reset; //KEY[0]
	reg  [2:0] KEY_r; //remaining KEY[*]
	reg  [9:0] sw_in;
	wire [9:0] ledr_out;





	//==================================================
	//                TRI-STATE SETUP
	//==================================================

	//first bank declarations
	wire [35:0] gpio_0;
	reg  [35:0] gpio_0_drive;
	reg  [35:0] gpio_0_en;

	//second bank declarations
	wire [35:0] gpio_1;
	reg  [35:0] gpio_1_drive;
	reg  [35:0] gpio_1_en;

	// Tristate logic needs to be in this order to match Qsys convention
	// '1' ==> output ; '0' ==> input
	assign gpio_0 = gpio_0_en ? 36'hz : gpio_0_drive;
	assign gpio_1 = gpio_1_en ? 36'hz : gpio_1_drive;





	//==================================================
	//                   Instantiation
	//==================================================

	pulpino_qsys_test dut (
		.CLOCK_50  (tb_clk),
		.KEY       ({KEY_r[2:0], key_reset}),
		.SW        (sw_in),
		.LEDR      (ledr_out),
		.GPIO_0    (gpio_0),
		.GPIO_1    (gpio_1)
	);


	
	// wire [7:0] debug1;
	// wire [7:0] debug0;
	// wire [7:0] debug2;
	// wire [7:0] debug3;
	// assign {debug0, debug1, debug2, debug3} = dut.pulpino_qsys_test.debug_wire;
	
	wire [31:0] debug_wire;
	assign debug_wire = dut.pulpino_qsys_test.debug_wire;

	wire received_irq;
	assign received_irq = dut.pulpino_qsys_test.u0.pulpino_0.RISCV_CORE.id_stage_i.irq_i;
	wire [4:0] irq_id;
	assign irq_id = dut.pulpino_qsys_test.u0.pulpino_0.RISCV_CORE.id_stage_i.irq_id_i;
	//
	// wire [31:0] pc_id;
	// assign pc_if = dut.pulpino_qsys_test.u0.pulpino_0.RISCV_CORE.pc_if;
	//
	// wire [31:0] jump_target;
	// assign jump_target = dut.pulpino_qsys_test.u0.pulpino_0.RISCV_CORE.jump_target_ex;
	//
	// wire branch_decision;
	// assign jump_decidion = dut.pulpino_qsys_test.u0.pulpino_0.RISCV_CORE.branch_decision;

	//==================================================
	//                   SIMULATION
	//==================================================

	`include "debug_after_flag.svh"
	`include "gpio_helper.svh"
	initial begin

		integer i;
		integer pos;
		integer bank;
		integer timeout_limit;
		reg [31:0] expected_value;
	

		//==== Initial Conditions ====
		timeout_limit = 500;
		tb_clk = 0;
		key_reset = 1'b0;
		KEY_r = 3'b1;
		sw_in = 10'b0;

		// all gpios set as input
		gpio_0_en = 36'b0;
		gpio_1_en = 36'b0;

		// all gpios cleaned
		gpio_0_drive = 36'b0;
		gpio_1_drive = 36'b0;
 
 
		#100


		for(i=0; i<72; i++) begin
			$display("==============STARTING TEST FOR GPIO: %d  ==================", i);
			// Resetting the core
			key_reset = 1'b0;
			#cpu_period
			#cpu_period
			key_reset = 1'b1;

			// Confirm correct execution
			wait_for_stable_debug();
			wait_for_main();


			// Wait for the cpu to be idle
			wait_for_flag2({NORMAL, WHILE, 8'h00, 8'h00});

			// Trigger interrupt
			// those wires are 36 but long and are directly mapped
			if(i<36) begin
				gpio_0_drive[i] = 1'b1;
				#cpu_period
				gpio_0_drive[i] = 1'b0;
			end
			else if(i<72) begin
				gpio_1_drive[i-36] = 1'b1;
				#cpu_period
				gpio_1_drive[i-36] = 1'b0;
			end
			
			pos = get_gpio_pos(i);
			bank = get_gpio_bank(i);

			if( bank == 0 ) begin
				//Processor word is only 32 bit long
				expected_value = 32'h00000000;
				expected_value[pos] = 1'b1;
				
				//bank 0 is handler 3
				wait_for_flag2({NORMAL, ISR, 8'h03, 8'h00});
				assert_debug_after_flag({NORMAL, ISR, 8'h03, 8'h01}, 0, timeout_limit);
				assert_debug_after_flag({NORMAL, ISR, 8'h03, 8'h02}, expected_value, timeout_limit);
			end
			else if(bank == 1) begin
				//Processor word is only 32 bit long
				expected_value = 32'h00000000;
				expected_value[pos] = 1'b1;
				
				//bank 1 is handler 4
				wait_for_flag2({NORMAL, ISR, 8'h04, 8'h00});
				assert_debug_after_flag({NORMAL, ISR, 8'h04, 8'h01}, 0, timeout_limit);
				assert_debug_after_flag({NORMAL, ISR, 8'h04, 8'h02}, expected_value, timeout_limit);
			end
			else if(bank==2) begin
				//Processor word is only 32 bit long
				expected_value = 32'h00000000;
				expected_value[pos] = 1'b1;
				
				//bank 2 is handler 5
				wait_for_flag2({NORMAL, ISR, 8'h05, 8'h00});
				assert_debug_after_flag({NORMAL, ISR, 8'h05, 8'h01}, 0, timeout_limit);
				assert_debug_after_flag({NORMAL, ISR, 8'h05, 8'h02}, expected_value, timeout_limit);
			end


			// Wait CPU to become idle again
			wait_for_flag({NORMAL, WHILE, 8'h00, 8'h00});

		end



		//===== Finish the simulation ====
		#200
		$stop;
	end

	// Clock Generation
	always begin
		#(clk_period/2) tb_clk = ~tb_clk;
	end

endmodule
