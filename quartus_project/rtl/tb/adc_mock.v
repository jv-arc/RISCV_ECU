module adc_mock
#(

	parameter DELAY_DEPTH = 2,
	parameter WORD_SIZE = 12,
	parameter ADDR_DEPTH = 8

)(

	input                   CLK,
	input                   TRIGGER,
	input                   RESET,
	output [WORD_SIZE-1:0]  DATA,
	output                  DVALID,
	output                  BUSY,


	input                   TB_FORCE_ADDR,
	input                   TB_FORCE_DATA,
	input  [WORD_SIZE-1:0]  TB_DATA,
	input  [ADDR_DEPTH-1:0] TB_ADDR

);

	// State Machine Values
	localparam hold_cycles = 10;
	reg        [9:0] hold_count;

	localparam [1:0] idle = 2'b00;
	localparam [1:0] run  = 2'b01;
	localparam [1:0] hold = 2'b10;

	reg        [1:0] current_state;
	reg        [1:0] next_state;
	reg              trigger_prev;

	// Address Logic
	localparam max_value = ((2**ADDR_DEPTH) - 1);

	reg  [ADDR_DEPTH-1:0]  address_count;
	wire [ADDR_DEPTH-1:0]  address_rom;

	assign address_rom =  (TB_FORCE_ADDR) ? TB_ADDR : address_count;


	// Data Logic
	reg                    enable_shift;
	wire [WORD_SIZE-1:0]   data_rom;
	wire [WORD_SIZE-1:0]   data_shift;

	assign data_shift =  (TB_FORCE_DATA) ? TB_DATA : data_rom;
	assign BUSY = (current_state!=idle);

	simple_rom #(

		.WORD_SIZE(WORD_SIZE),
		.ADDR_DEPTH(ADDR_DEPTH),
		.MEMORY_FILE("fake_data.hex")

	) fake_adc_data (

		.CLK(CLK),
		.ENABLE(1'b1),
		.ADDR(address_rom),
		.DATA(data_rom)

	);

	shift_register_bank #(

		.LENGTH(DELAY_DEPTH),
		.WIDTH(WORD_SIZE)

	) shift_reg (

		.CLK(CLK),
		.ENABLE(enable_shift),
		.DATA_IN(data_shift),
		.DATA_OUT(DATA),
		.DVALID(DVALID)

	);


	initial begin
		current_state = idle;
		address_count = 0;
		hold_count    = 0;
	end

	// Turning signals on
	always @(posedge CLK) begin

		if (RESET) begin
			current_state <= idle;
			trigger_prev  <= 1'b0;
			enable_shift  <= 1'b0;
			address_count <= 0;
			hold_count    <= 0;

		end else begin

			trigger_prev <=TRIGGER;
			current_state <= next_state;

			case(current_state)

				idle: begin
					enable_shift <= 1'b0;
					hold_count   <= 0;
	
					if(address_count == max_value)begin
						address_count <= 0;

					end

				end

				run: begin
					enable_shift <= 1'b1;

				end

				hold: begin
					enable_shift <= 1'b1;

					if(hold_count < hold_cycles) begin
						hold_count <= hold_count + 1;

					end else begin
						address_count <= address_count + 1;
						hold_count <= 0;

					end
				end
				
			endcase
		end
	end

	// State Machine logic
	always @ ( * ) begin
		case(current_state)
			idle: next_state = (~trigger_prev && TRIGGER)  ? run  : current_state;
			run:  next_state = (DVALID)                    ? hold : current_state;
			hold: next_state = (hold_count == hold_cycles) ? idle : current_state;
		endcase
	end

endmodule
