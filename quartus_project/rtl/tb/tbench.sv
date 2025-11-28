`timescale 1ns/10ps

module tbench;







 // ╔═══════════════════════════════════════════╗
 // ║ ███████╗███████╗████████╗██╗   ██╗██████╗ ║
 // ║ ██╔════╝██╔════╝╚══██╔══╝██║   ██║██╔══██╗║
 // ║ ███████╗█████╗     ██║   ██║   ██║██████╔╝║
 // ║ ╚════██║██╔══╝     ██║   ██║   ██║██╔═══╝ ║
 // ║ ███████║███████╗   ██║   ╚██████╔╝██║     ║
 // ║ ╚══════╝╚══════╝   ╚═╝    ╚═════╝ ╚═╝     ║
 // ╚═══════════════════════════════════════════╝


	// fixed during simulation
	parameter BOOT_ADDR  = 32'h00008000;
	parameter clk_period = 20; // 50MHz
	parameter cpu_period = clk_period*2;
	
	wire test_mode;
	wire fetch_enable;
	wire clock_gating;
	
	assign test_mode    = 1'b0;
	assign fetch_enable = 1'b1;
	assign clock_gating = 1'b0;
	
	// changing during the simulation
	reg         tb_clk;
	reg         jtag_reset;
	reg         key_reset; //KEY[0]
	reg  [2:0]  KEY_r;     //remaining KEY[*]
	reg  [9:0]  sw_in;
	wire [9:0]  ledr_out;
	wire [35:0] gpio_0;
	wire [35:0] gpio_1;






 // ╭────────────╮
 // │ ADC CONFIG │
 // ╰────────────╯
	
	localparam data_size = 8;
	localparam pinout_size = data_size + 4;

  wire adc_trigger;
  wire adc_reset;
  wire adc_dvalid;
  wire adc_busy;

	wire [data_size-1:0]   adc_data;
	wire [pinout_size-1:0] adc_pinout;


	// WARNING: In Verilog concatenation the left most bit goes into MSB
	assign adc_pinout={adc_trigger, adc_reset, adc_data, adc_dvalid, adc_busy};
	localparam offset = 10;

	// The expression (2**pinout_size)-1) creates a  word of 1 that is
	// pinout_size long, multiplying it by 2*offset move the word to the
	// correct position
	integer adc_pinout_mask  = (((2**pinout_size)-1)*(2**(offset)));
	integer adc_trigger_mask = (2**(offset+pinout_size));
	
	assign gpio_0[pinout_size + offset - 1:offset] = adc_pinout;






 // ╭───────────────╮
 // │ INSTANTIATION │
 // ╰───────────────╯

	pulpino_qsys_test dut (
		.CLOCK_50  (tb_clk),
		.KEY       ({KEY_r[2:0], key_reset}),

		.SW        (sw_in),
		.LEDR      (ledr_out),

		.GPIO_0    (gpio_0),
		.GPIO_1    (gpio_1)
	);

	adc_mock #(

		.DELAY_DEPTH(5),
		.WORD_SIZE(data_size),
		.ADDR_DEPTH()

	)adc(

		.CLK(tb_clk),
		.TRIGGER(adc_trigger),
		.RESET(adc_reset),

		.DATA(adc_data),
		.DVALID(adc_dvalid),
		.BUSY(adc_busy),

		.TB_FORCE_ADDR(1'b0),
		.TB_FORCE_DATA(1'b0),
		.TB_DATA(),
		.TB_ADDR()

	);





 // ╭────────────────────────────────╮
 // │ AXULIAR SIMULATION DEFINITIONS │
 // ╰────────────────────────────────╯

	wire db_mode;
	assign db_mode = dut.pulpino_qsys_test.pio_1_w[0];

	wire [31:0] db_wire;
	assign db_wire = dut.pulpino_qsys_test.db_output;

	wire received_irq;
	assign received_irq = dut.pulpino_qsys_test.u0.pulpino_0.RISCV_CORE.id_stage_i.irq_i;

	wire [4:0] irq_id;
	assign irq_id = dut.pulpino_qsys_test.u0.pulpino_0.RISCV_CORE.id_stage_i.irq_id_i;
	
	wire [data_size-1:0] memory_value;
	assign memory_value = adc.fake_adc_data.ADDR;
	reg [data_size-1:0] data_read;










 // ╔════════════════════════════════════════════════════════════════════════════════╗
 // ║ ███████╗██╗███╗   ███╗██╗   ██╗██╗      █████╗ ████████╗██╗ ██████╗ ███╗   ██╗ ║
 // ║ ██╔════╝██║████╗ ████║██║   ██║██║     ██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║ ║
 // ║ ███████╗██║██╔████╔██║██║   ██║██║     ███████║   ██║   ██║██║   ██║██╔██╗ ██║ ║
 // ║ ╚════██║██║██║╚██╔╝██║██║   ██║██║     ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║ ║
 // ║ ███████║██║██║ ╚═╝ ██║╚██████╔╝███████╗██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║ ║
 // ║ ╚══════╝╚═╝╚═╝     ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝ ║
 // ╚════════════════════════════════════════════════════════════════════════════════╝



//`include "gpio_helper.svh"
`include "debug_after_flag.svh"

	parameter NORMAL = 8'h00;
	parameter SUCCESS = 8'hF0;
	parameter FAILURE = 8'hFF;

	parameter MAIN = 8'h00;
	parameter WHILE = 8'h01;
	parameter FUNC = 8'h02;
	parameter SETUP = 8'h03;
	parameter ISR = 8'h04;




 // ╭──────────────────╮
 // │ CLOCK GENERATION │
 // ╰──────────────────╯

	always begin
		#(clk_period/2) tb_clk = ~tb_clk;
	end






 // ╭───────────────╮
 // │ MAIN SEQUENCE │
 // ╰───────────────╯

	initial begin

		integer i;
		integer timeout_limit;


		// Initial Values
		timeout_limit = 500;
		tb_clk = 0;
		key_reset = 1'b0;
		KEY_r = 3'b1;
		sw_in = 10'b0;




		$display("\n\n\n");
		$display("=====================================");
		$display("             ADC MOC TEST");
		$display("=====================================");


		#100


		// Resetting the core
		key_reset = 1'b0;
		#cpu_period
		#cpu_period
		#cpu_period
		key_reset = 1'b1;


		// Confirm correct execution
		// sync_readable_debug();
		// sync_main();


		// Main simulation loop
		//for(i=0; i < ((2**data_size) -1); i++) begin  //
		for(i=0; i < 2; i++) begin
			$display("\n\n");
			$display("- - - - - - - - - - - - - - - - -");
			$display("TESTING MEMORY ADDRESS: %d", i);
			$display("- - - - - - - - - - - - - - - - -");
			$display("\n");


			// Trigger input
			KEY_r[0] = 1'b0;
			$display("Button pressed");
			#cpu_period
			KEY_r[0] = 1'b1;
			$display("Button released");
			$display("\n");

			repeat (125) begin
				$display("-");
				#cpu_period
				$display("|");
			end
			
			$display("\n");
			$display("\n");
			$display("Test case %d ended", i);
			$display("\n");
		end

		#200

		results;
		$stop;

	end


endmodule
