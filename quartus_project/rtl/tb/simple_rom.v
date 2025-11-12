module simple_rom #(
	parameter WORD_SIZE,
	parameter ADDR_DEPTH,
	parameter MEMORY_FILE
)(
	input                       CLK,
	input                       ENABLE,
	input      [ADDR_DEPTH-1:0] ADDR,
	output reg [WORD_SIZE-1:0]  DATA
);

	reg [WORD_SIZE-1:0] memory[ADDR_DEPTH-1:0];

	initial begin
		$redmemh(MEMORY_FILE, memory);
	end

	always @(posedge CLK) begin
		DATA = (ENABLE == 1'b1) ? memory[ADDR] : 1'b0;
	end

endmodule

