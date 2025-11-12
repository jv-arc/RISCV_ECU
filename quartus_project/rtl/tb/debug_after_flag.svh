// === WARNING !!!! ===
// This will run indefinitely if flag is not reached
task wait_for_flag;
	input [31:0] expected_flag;

	begin
		while (debug_wire != expected_flag) begin
			@(posedge tb_clk);

		end
		$display("[FLAG-LOG] flag 0x%h reached at %t", expected_flag, $time);

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
			$display("[FLAG-LOG] flag 0x%h reached at %t", expected_flag, $time);

		end else begin
			$display("[FLAG-ERROR] flag 0x%h not reached - current time: %t", expected_flag, $time);

		end

	end
endtask


task wait_for_stable_debug;
	begin

		// Wait while debug_wire contains X or Z (unstable/uninitialized)
		while (^debug_wire === 1'bx) begin
			@(posedge tb_clk);

		end
		$display("[INIT-LOG]: debug_wire stabilized at %t with value 0x%h", $time, debug_wire);

	end
endtask

task wait_for_main;
	begin

		while(debug_wire != 32'h55555555) begin
			@(posedge tb_clk);

		end

		$display("[FLAG-LOG]: MAIN REACHED AT %t", $time);

		while(debug_wire == 32'h55555555) begin
			@(posedge tb_clk);

		end

	end
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
			$display("ERROR: Wait for flag 0x%h took too long - current time: %t", expected_flag, $time);

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
				$display("ERROR: Waited too long for debug value! - current time: %t", $time);

			end

			// If we didn't timeout, that means the debug_wire changed
			else begin
				
				// capture value in debug_wire
				captured_value = debug_wire;

				// Test value
				if(captured_value == expected_debug) begin
					$display("ASSERTION COMPLETED, wire_debug is 0x%h - current time: %t", captured_value, $time);

				end else begin
					$display("ASSERTION FAILED: the expected value was 0x%h", expected_debug);
					$display("wire_debug contains 0x%h at time %t", captured_value, $time);

				end

			end

		end
	end
endtask

task wait_match;
	input signal_read;
	input signal_reference;
	input integer timeout;


	integer time_count;

	begin
		time_count = 0;
	
		while (signal_read != signal_reference && time_count < timeout) begin
			@(posedge tb_clk);
			time_count = time_count + 1;
		end

		if(time_count < timeout) begin
			$display("SUCCESS! signals match at %t", $time);
		end else begin
			$display("ERROR! signals do not match");
			$display("Signal being watched %h, Signal expected %h", signal_read, signal_reference);
		end
		
	end
endtask





