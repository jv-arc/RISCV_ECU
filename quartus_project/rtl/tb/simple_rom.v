module simple_rom #(
	parameter WORD_SIZE,
	parameter ADDR_DEPTH,
	parameter memory_file
)(
	input                   CLK,
	input                   ENABLE,
	input  [ADDR_DEPTH-1:0] ADDR,
	output [WORD_SIZE-1:0]  DATA
);

	reg [WORD_SIZE-1:0] memory[ROM_SIZE-1:0];

	initial begin
		$redmemh(memory_file, memory);
	end

	always(@posedge CLK) begin
		if (ENABLE == 1'b1) begin
			DATA = memory[ADDR];
	end

endmodule

