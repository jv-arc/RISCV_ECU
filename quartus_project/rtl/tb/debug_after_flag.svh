 // ╔═════════════════════════════════════════════════════════════╗
 // ║                                                             ║
 // ║    ██████╗ ███████╗██████╗ ██╗   ██╗ ██████╗   ██╗   ██╗    ║
 // ║    ██╔══██╗██╔════╝██╔══██╗██║   ██║██╔════╝   ██║   ██║    ║
 // ║    ██║  ██║█████╗  ██████╔╝██║   ██║██║  ███╗  ██║   ██║    ║
 // ║    ██║  ██║██╔══╝  ██╔══██╗██║   ██║██║   ██║  ╚██╗ ██╔╝    ║
 // ║    ██████╔╝███████╗██████╔╝╚██████╔╝╚██████╔╝██╗╚████╔╝     ║
 // ║    ╚═════╝ ╚══════╝╚═════╝  ╚═════╝  ╚═════╝ ╚═╝ ╚═══╝      ║
 // ║                                                             ║
 // ╚═════════════════════════════════════════════════════════════╝

 //  ┌                                                           ┐
 //  │ This is a series of tasks for making reading and checking │
 //  │ processor behavior more easily. If this is not enough you │
 //  │ probably need JTAG debugging                              │
 //  └                                                           ┘


integer _task_count  = 0;
integer _error_count = 0;

task automatic wait_match;
	input signal_read_in;
	input signal_reference_in;
	input integer timeout_in;

	automatic logic signal_read = signal_read_in;
	automatic logic signal_reference = signal_reference_in;
	automatic integer timeout = timeout_in;

	fork
		begin
			_task_count = _task_count + 1;

			integer time_count = 0;

			$display("\n\n");
			$display("[START-%d] Waiting started at %t", _task_count, $time);

			if(timeout < 0) begin // if negative runs forever
			
				while (signal_read != signal_reference) begin
					@(posedge tb_clk);
				end

				$display("[MATCH-%d] Signals with value %h match at %t", _task_count, signal_reference, $time);
			

			end else begin
			
				while (signal_read != signal_reference && time_count < timeout) begin
					@(posedge tb_clk);
					time_count = time_count + 1;
				end

				if(time_count < timeout) begin
					$display("[MATCH-%d] Signals with value %h match at %t", signal_reference, $time);
				end else begin

					_error_count = _error_count + 1;

					$display("[MISS-%d] Signals do not match in the timeframe", _task_count);
					$display("[INFO-%d] Target: %h, Reference %h, Timestamp: %t", _task_count, signal_read, signal_reference, $time);
					$display("[INFO-%d] Error count: %d", _task_count, _error_count);
				end
			end
		end
	join_none
	#0;
endtask

task sync_state;
	input [31:0] expected_state;

	begin
		while (debug_wire != expected_state) begin
			@(posedge tb_clk);

		end
		$display("[INFO] state 0x%h reached at %t", expected_state, $time);
	end
endtask


task sync_signal;
	input signal;
	input reference;

	begin
		while (signal != reference) begin
			@(posedge tb_clk);

		end
		$display("[MATCH] Signal equaled 0x%h at %t", reference, $time);
	end
endtask


task wait_for_flag2;
	input [31:0] expected_flag;
	integer      max_time;
	integer      current_time;


	begin
		max_time = 5000;
		current_time = 0;

		while (debug_wire != expected_flag && current_time < max_time) begin
			@(posedge tb_clk);
			current_time = current_time + 1;

		end

		if(current_time < max_time) begin
			$display("[MATCH] flag 0x%h reached at %t", expected_flag, $time);

		end else begin
			$display("[MISS] flag 0x%h not reached - current time: %t", expected_flag, $time);

		end

	end
endtask


task wait_for_stable_debug;
	begin

		// Wait while debug_wire contains X or Z (unstable/uninitialized)
		while (^debug_wire === 1'bx) begin
			@(posedge tb_clk);

		end
		$display("[INFO] debug_wire stabilized at %t with value 0x%h", $time, debug_wire);

	end
endtask

task sync_main;
	begin

		while(debug_wire != 32'h55555555) begin
			@(posedge tb_clk);

		end

		$display("[MATCH]: MAIN REACHED AT %t", $time);

		while(debug_wire == 32'h55555555) begin
			@(posedge tb_clk);

		end

	end
endtask

task assert_signal_after_signal;
	input first_watch_in;
	input first_reference_in;
	input second_watch_in;
	input second_reference_in;
	input timeout_in;

	automatic logic first_watch;
	automatic logic first_reference;
	automatic logic second_watch;
	automatic logic second_reference;
	automatic logic timeout;

	fork
		begin
			integer time_count = 0;
			_task_count = _task_count + 1;

			if(timeout < 0)begin

				while(first_watch != first_reference) begin
					@(posedge tb_clk);
				end
				
				$display("[MATCH-%d]", _task_count);
				$display("[INFO-%d]", _task_count);

				while(second_watch != second_reference) begin
					@(posedge tb_clk);
				end

				$display("[MATCH-%d]", _task_count);
				$display("[INFO-%d]", _task_count);

			end else begin
				
				while (first_watch != first_reference && time_count < timeout) begin
					@(posedge tb_clk);
					time_count = time_count + 1;
				end

				if(time_count >= timeout)begin
					$display("[MISS-%d]", _task_count);
					$display("[INFO-%d]", _task_count);
				else end begin

					time_count = 0;
					while (second_watch != second_reference && time_count < timeout) begin
						@(posedge tb_clk);
						time_count = time_count + 1;
					end

					if(time_count >= timeout) begin
						$display("[MISS-%d]", _task_count);
						$display("[INFO-%d]", _task_count);
					end else begin
						$display("[MATCH-%d]", _task_count);
						$display("[INFO-%d]", _task_count);
					end
				end
			end
		end
	join_none
	#0;
endtask



task assert_debug_after_flag;
	input [31:0]  expected_flag;
	input [31:0]  expected_debug;
	input integer max_time;

	integer       time_count;
	integer       flag_wait;
	integer       debug_wait;
	reg   [31:0]  captured_value;

	begin
		time_count = 0;

		// Exists if timed out or if flag is found
		while (debug_wire != expected_flag && time_count < max_time) begin
			@(posedge tb_clk);
			time_count = time_count + 1;
		end

		

		// Checks if we timed out
		if(time_count >= max_time) begin
			$display("[MISS] Wait for flag 0x%h took too long - current time: %t", expected_flag, $time);

		end


		// If we didn't timed out, we found the flag!
		else begin
			
			// waits for the flag signal to end
			// Only exists if we find the flag or we time_out
			while(debug_wire == expected_flag  && time_count < max_time) begin
				@(posedge tb_clk);
				time_count = time_count + 1;

			end

			// Checks again if we timed out
			if(time_count >= max_time) begin
				$display("[MISS] Waited too long for debug value! - current time: %t", $time);

			end

			// If we didn't timeout, that means the debug_wire changed
			else begin
				
				// capture value in debug_wire
				captured_value = debug_wire;

				// Test value
				if(captured_value == expected_debug) begin
					$display("[MATCH] wire_debug is 0x%h - current time: %t", captured_value, $time);

				end else begin
					$display("[MISS] the expected value was 0x%h", expected_debug);
					$display("[INFO] wire_debug contains 0x%h at time %t", captured_value, $time);

				end

			end

		end
	end
endtask





