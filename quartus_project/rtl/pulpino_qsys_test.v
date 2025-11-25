

// These match the DE1-SoC names precisely
// So we do not need to make pin assignments
module pulpino_qsys_test (
	input CLOCK_50,
	input  [3:0]  KEY,
	input  [9:0]  SW,
	output [9:0]  LEDR,
	inout  [35:0] GPIO_0,
	inout  [35:0] GPIO_1
);





// ╭────────────────────────╮
// │ ZERORISCVY BASE CONFIG │
// ╰────────────────────────╯

parameter BOOT_ADDR = 32'h00008000;

wire test_mode;
wire fetch_enable;
wire clock_gating;

assign test_mode = 1'b0;
assign fetch_enable = 1'b1;
assign clock_gating = 1'b0;







// ╭─────────────────╮
// │ SYNCHRONIZATION │
// ╰─────────────────╯

// Supporting wires
wire clk25;
wire jtag_reset;
wire reset_n;
assign reset_n = KEY[0] & ~jtag_reset;

// PLL Instantiation
pll clock_conversion(
	.refclk   (CLOCK_50),
	.rst      (~reset_n),
	.outclk_0 (clk25)
);








// ╭─────────────────────────────╮
// │ GENERAL GPIO REPRESENTATION │
// ╰─────────────────────────────╯

// ┌                                                          ┐
// │ Instead of using "input", "output" etc. to name these    │
// │ I'm using the CPU perspective on dataflow, that is, if   │
// │ the CPU will mainly write (w) to the wire, or if it will │
// │ mainly read from the wire.                               │
// │                                                          │
// │ I try to keep this consistent in all context the signal  │
// │ interacts in some way with lsu operations, because they  │
// │ are all consistent since they are based on a single pov. │
// │                                                          │
// │ Names in Qsys and other in other contexts might not be   │
// │ the same, unfortunately.                                 │
// └                                                          ┘

// wires for gpio bank A
localparam BANK_A_SIZE = 32;
wire [BANK_A_SIZE-1:0] gpio_a_r;
wire [BANK_A_SIZE-1:0] gpio_a_w;
wire [BANK_A_SIZE-1:0] gpio_a_s;


// wires for gpio bank B
localparam BANK_B_SIZE = 32;
wire [BANK_B_SIZE-1:0] gpio_b_w;
wire [BANK_B_SIZE-1:0] gpio_b_r;
wire [BANK_B_SIZE-1:0] gpio_b_s;


// wires for gpio bank C
localparam BANK_C_SIZE = 8;
wire [BANK_C_SIZE-1:0] gpio_c_w;
wire [BANK_C_SIZE-1:0] gpio_c_r;
wire [BANK_C_SIZE-1:0] gpio_c_s;

localparam PIN_NUMBER = BANK_A_SIZE + BANK_B_SIZE + BANK_C_SIZE;


// Connection to ports
wire [PIN_NUMBER-1:0] f_gpio;
assign f_gpio = {GPIO_1, GPIO_0};


// Connection to PIO components
wire [PIN_NUMBER-1:0] gpio_r;
wire [PIN_NUMBER-1:0] gpio_w;
wire [PIN_NUMBER-1:0] gpio_s;

assign gpio_r = {gpio_a_r, gpio_b_r, gpio_c_r};
assign gpio_w = {gpio_a_w, gpio_b_w, gpio_c_w};
assign gpio_s = {gpio_a_s, gpio_b_s, gpio_c_s};






//  ╭────────────────╮
//  │ GPIO TRI-STATE │
//  ╰────────────────╯

// Generation of granular per wire tristate control
genvar i;
generate
	for(i=0; i < PIN_NUMBER; i++) begin : gen_tstate_block
		assign f_gpio[i] = (gpio_s[i] == 1'b0) ? gpio_w[i] : 1'bz;
	end
endgenerate

// Reading from port happens when f_gpio[i] = 1'bz
assign gpio_r = f_gpio;






//  ╭───────────╮
//  │ DEBUGGING │
//  ╰───────────╯

// ┌                                                  ┐
// │ This is only meant for development, I just think │
// │ it's easier to debug simple behavior in the wave │
// │ forms and asserts in verilog than always using   │
// │ JTAG, so this are peripherals only for the CPU   │
// │ to write to and check on the testbench           │
// └                                                  ┘

// Shows if the wire is for flags or if it is for 
// readding 32 bit words
wire db_mode;


// the flag* signals are the same as db_output
// they are just separated in the same way I
// do on the C code
wire [31:0] db_output;

wire [7:0] flag0;
wire [7:0] flag1;
wire [7:0] flag2;
wire [7:0] flag3;

assign db_output = {flag3, flag2, flag1, flag0};






//   ╭─────────────────────────╮
//   │ OTHER PIO CONFIGURATION │
//   ╰─────────────────────────╯

// ┌                                                     ┐
// │ Some of the PIO peripherals are shared between      │
// │ GPIO pins (bank C) and other input and output pins, │
// │ this section makes sure the wires are correctly     │
// │ concatenated and that nonused bits are setup in a   │
// │ safe manner                                         │
// └                                                     ┘

// internal wires
wire [3:0] keys;
wire [9:0] switches;
wire [9:0] leds;


// ports
assign LEDR     = leds;
assign keys     = KEY;
assign switches = SW;


// Remaining PIO bits
localparam PIO_SIZE = 32 - BANK_C_SIZE;

wire [PIO_SIZE-1:0] pio_r;
wire [PIO_SIZE-1:0] pio_0_w;
wire [PIO_SIZE-1:0] pio_1_w;

localparam r_r_size   = PIO_SIZE - 14;
localparam r_0_w_size = PIO_SIZE - 10;
localparam r_1_w_size = PIO_SIZE - 1;

wire [r_r_size-1:0]   r_r;
wire [r_0_w_size-1:0] r_0_w;
wire [r_1_w_size-1:0] r_1_w;


// PIO signal concatenation
assign r_r   = {r_r_size{1'b0}};
assign r_0_w = {r_0_w_size{1'bz}};
assign r_1_w = {r_1_w_size{1'bz}};

assign pio_r   = {keys, switches, r_r};
assign pio_0_w = {leds, r_0_w};
assign pio_1_w = {r_1_w, db_mode};












// ╭───────────────────────────╮
// │ MAIN SYSTEM INSTANTIATION │
// ╰───────────────────────────╯

sys u0 (
	// Basic pins
	.clk_clk                                  (clk25),
	.reset_reset_n                            (reset_n),
	.master_0_master_reset_reset              (jtag_reset),

	// Main config
	.pulpino_0_config_fetch_enable_i          (fetch_enable),
	.pulpino_0_config_clock_gating_i          (clock_gating),
	.pulpino_0_config_boot_addr_i             (BOOT_ADDR),
	.pulpino_0_config_testmode_i              (test_mode),

	// Peripherals
	.gpio_a_r_external_connection_export      (gpio_a_r),
	.gpio_a_w_external_connection_export      (gpio_a_w),
	.gpio_a_s_external_connection_export      (gpio_a_s),
	.gpio_b_r_external_connection_export      (gpio_b_r),
	.gpio_b_w_external_connection_export      (gpio_b_w),
	.gpio_b_s_external_connection_export      (gpio_b_s),
	.gpio_c_r_external_connection_export      ({gpio_c_r, pio_r}),
	.gpio_c0_w_external_connection_export     ({gpio_c_w, pio_0_w}),
	.gpio_c1_w_external_connection_export     ({gpio_c_s, pio_1_w}),
	.gpio_c2_w_external_connection_export     ({flag3, flag2, flag1, flag0})
);



endmodule
