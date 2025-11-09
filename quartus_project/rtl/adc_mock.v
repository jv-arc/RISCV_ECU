module adc_mock #(
	parameter DELAY_DEPTH = 2,
	parameter WORD_SIZE = 12,
	parameter ADDR_DEPTH = 8
)(
	input               CLK,
	input               TRIGGER,
	output [width-1: 0] DATA,
	output              DVALID
);

	wire [WORD_SIZE-1:0]   data_rom2shift;

	simple_rom #(
		
		.WORD_SIZE(WORD_SIZE),
		.ADDR_DEPTH(ADDR_DEPTH),
		.memory_file("fake_data.hex")

	) fake_adc_data (
		
		.CLK(CLK),
		.ENABLE(1'b1),
		.ADDR(),
		.DATA(data_rom2shift)

	);

	shift_register_bank #(
		
		.LENGTH(DELAY_DEPTH),
		.WIDTH(WORD_SIZE)

	) shift_reg (
	
		.CLK(CLK),
		.RESET(),
		.DATA_IN(data_rom2shift),
		.DATA_OUT(DATA)

	);


endmodule
