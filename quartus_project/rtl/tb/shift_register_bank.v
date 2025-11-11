module shift_register_bank #(
	parameter LENGTH,
	parameter WIDTH
)(
	input                  CLK,
	input                  RESET,
	input                  TRIGGER,
	input      [WIDTH-1:0] DATA_IN,
	output reg [WIDTH-1:0] DATA_OUT,
	output reg             DVALID
);
	/*
		The register bank consists of an WIDTH amount of shift registers,
		each of them with a LENGTH amount of bits.
	*/
	reg [LENGTH-1:0] register_bank [WIDTH-1:0];
	reg [LENGTH-1:0] valid_shift;

	// needed for the access later
	integer i,j;



	always(@posedge CLK) begin

		// Behavior for Resetting Component
		if (RESET == 1'b1) begin


			for(i = 0; i < WIDTH ; i++) begin
				for(j = 0; j < LENGTH ; j++) begin
					register_bank[i][j] <= 1'b0;
				end
			end

			for(j = 0; j < LENGTH; j++) begin
				valid_shift[j] <= 1'b0;
			end


		end else
		begin // Regular Component Behavior
			
			// Only while TRIGGER is held
			if (TRIGGER == 1'b1) begin
				
				// Loop torugh all registers from 0 to WIDTH
				for(i = 0; i < WIDTH ; i++) begin
				
					// each DATA_IN goes to the first bit of it's respective register_bank
					register_bank[i][0] <= DATA_IN[i];

				end

				// the valid_register receives 1 at it's entrance
				valid_shift[0] <= 1'b1;
			end else begin

			// Only while TRIGGER is not held
				
				for(i = 0; i < WIDTH ; i++) begin
					register_bank[i][0] <= 1'b0;
				end
				valid_shift[0] <= 1'b0;

			end

			// Regardless of the TRIGGER
			

				// Loop torugh all registers from 0 to WIDTH
			for(i = 0; i < WIDTH ; i++) begin
		
				// For each register, a loop trough all bits, from 0 to LENGTH
				for(j = 1; j < LENGTH-1 ; j++) begin
					register_bank[i][j] <= register_bank[i][j-1];
				end

				// the last bit from each register_bank goes to it's respective DATA_OUT
				DATA_OUT[i] <= register_bank[i][LENGTH-1];
			end

			for(j = 0; j <LENGTH-1 ; j++) begin
				valid_shift[j] <= valid_shift[j-1];
			end
			
			DVALID <= valid_shift[LENGTH-1];

		end
	end
endmodule



