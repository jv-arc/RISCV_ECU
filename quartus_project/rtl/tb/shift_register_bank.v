module shift_register_bank
#(

	parameter LENGTH,
	parameter WIDTH

)(

	input                  CLK,
	input                  ENABLE,
	input      [WIDTH-1:0] DATA_IN,
	output reg [WIDTH-1:0] DATA_OUT,
	output reg             DVALID

);


reg [LENGTH-2:0] register_bank [WIDTH-1:0];
reg [LENGTH-2:0] valid_shift;


integer i, j;


always @(posedge CLK) begin


	// Loop trough all rows of the shift register
	for(i=0; i< WIDTH; i++) begin

		register_bank[i][0] <= (ENABLE) ? DATA_IN[i] : 1'b0;
		DATA_OUT[i] <= (ENABLE) ? register_bank[i][LENGTH-2]: 1'b0;

		// Loop trough all columns of the shift register
		for(j=1; j < LENGTH-1; j++) begin
			register_bank[i][j] <= (ENABLE) ? register_bank[i][j-1] : 1'b0;

		end
	end

	valid_shift[0] <= ENABLE;
	DVALID <= (ENABLE) ? valid_shift[LENGTH-2] : 1'b0;

	for(j=1;j<LENGTH-1; j++) begin
		valid_shift[j] <= (ENABLE) ? valid_shift[j-1] : 1'b0;

	end
end
endmodule
