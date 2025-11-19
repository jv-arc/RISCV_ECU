// ╔═══════════════════════════════════════════════════╗
// ║                                                   ║
// ║  ██████╗  ██████╗ ██╗     ██╗███╗   ██╗ ██████╗   ║
// ║  ██╔══██╗██╔═══██╗██║     ██║████╗  ██║██╔═══██╗  ║
// ║  ██████╔╝██║   ██║██║     ██║██╔██╗ ██║██║   ██║  ║
// ║  ██╔═══╝ ██║   ██║██║     ██║██║╚██╗██║██║   ██║  ║
// ║  ██║     ╚██████╔╝███████╗██║██║ ╚████║╚██████╔╝  ║
// ║  ╚═╝      ╚═════╝ ╚══════╝╚═╝╚═╝  ╚═══╝ ╚═════╝   ║
// ║                                                   ║
// ╚═══════════════════════════════════════════════════╝



module pulpino_qsys_test (
	input CLOCK_50,
	input  [3:0]  KEY,
	input  [9:0]  SW,
	output [9:0]  LEDR,
	inout  [35:0] GPIO_0,
	inout  [35:0] GPIO_1,
);




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
	.gpio_a_dir_external_connection_export    (gpio_a_s),
	.gpio_a_r_external_connection_export      (gpio_a_r),
	.gpio_a_w_external_connection_export      (gpio_a_w),
	.gpio_b_dir_external_connection_export    (gpio_b_s),
	.gpio_b_r_external_connection_export      (gpio_b_r),
	.gpio_b_w_external_connection_export      (gpio_b_w),
	.pio_r_one_external_connection_export     ({gpio_c_r, pio_r}),
	.pio_w_one_external_connection_export     ({gpio_c_w, pio_w_one}),
	.pio_w_two_external_connection_export     ({gpio_c_s, pio_w_two})
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

// PLL Instantiation
pll clock_conversion(
	.refclk   (CLOCK_50),
	.rst      (~reset_n),
	.outclk_0 (clk25)
);

// Supporting wires
wire clk25;
wire jtag_reset;
wire reset_n;
assign reset_n = KEY[0] & ~jtag_reset;






//   ╭─────────────────────────────╮
//   │ GENERAL GPIO REPRESENTATION │
//   ╰─────────────────────────────╯

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
wire [BANK_A_SIZE-1:0] gpio_a_w;
wire [BANK_A_SIZE-1:0] gpio_a_r;
wire [BANK_A_SIZE-1:0] gpio_a_s;


// wires for gpio bank B
localparam BANK_B_SIZE = 32;
wire [BANK_B_SIZE-1:0] gpio_b_w;
wire [BANK_B_SIZE-1:0] gpio_b_r;
wire [BANK_B_SIZE-1:0] gpio_ab_s;


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
wire [PIN_NUMBER-1:0] gpio_w;
wire [PIN_NUMBER-1:0] gpio_r;
wire [PIN_NUMBER-1:0] gpio_s;

assign gpio_w = {gpio_a_w, gpio_b_w, gpio_c_w};
assign gpio_r = {gpio_a_r, gpio_b_r, gpio_c_r};
assign gpio_s = {gpio_a_s, gpio_b_s, gpio_c_s};








// ╭─────────────────────╮
// │ TRISTATE GPIO LOGIC │
// ╰─────────────────────╯


wire [PIN_NUMBER-1:0] gpio_shared;
wire [PIN_NUMBER-1:0] gpio_drive;
wire [PIN_NUMBER-1:0] gpio_connect;


// Generation of the per wire tristate behavior
genvar i;
generate
	for(i=0; i < PIN_NUMBER; i ++) begin
		assign gpio_shared[i] = gpio_connect[i] ? gpio_drive[i] : 1'bz;
		assign f_gpio[i] = gpio_shared[i];
	end
endgenerate


// Per wire selection behavior
always @(*) begin
	for(genvar j=0; j < PIN_NUMBER ; j++) begin
		case(gpio_s[j])

			1'b1: begin
				gpio_drive[j] = gpio_r[j];
				gpio_connect[j] = 1'b1;
			end

			1'b0: begin
				gpio_drive[j] = gpio_w[j];
				gpio_connect[j] = 1'b1;
			end

			default: gpio_connect[j] = 1'b0;

		endcase
	end
end





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


// Empty PIO bits
localparam PIO_SIZE = 32 - BANK_C_SIZE;

wire [PIO_SIZE-1:0] pio_r;
wire [PIO_SIZE-1:0] pio_w_one;
wire [PIO_SIZE-1:0] pio_w_two;

localparam r_i_size = 14;
localparam r_o1_size = 10;
localparam r_o2_size = PIO_SIZE; //nothing (yet)

wire [r_i_size-1:0]  r_i;
wire [r_o1_size-1:0] r_o1;
wire [r_o2_size-1:0] r_o2;


// PIO signal concatenation
assign r_i  = {r_i_size{1'b0}};
assign r_o1 = {r_o1_size{1'bz}};
assign r_o2 = {r_o2_size{1'bz}};

assign pio_r = {keys, switches, r_i};
assign pio_w_one = {leds, r_o1};
assign pio_w_two = r_o2};




endmodule
