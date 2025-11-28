// This is a series of tasks for making reading and checking
// processor behavior more easily. If this is not enough you
// probably need JTAG debugging.



// Change this between declaration and simulation to change 
// time limit of tasks
integer _max_task_time;



//  ╭───────────────╮
//  │ TASKS / ERROR │
//  ╰───────────────╯

integer _task_count  = 0;
integer _error_count = 0;

task task_inc;
	begin
		_task_count  = _task_count  + 1;
	end
endtask

task error_inc;
	begin
		_error_count = _error_count + 1;
	end
endtask

task results;
	begin
		$display("\n\n");
		$display("============================");
		$display("      SIMULATION ENDED");
		$display("============================");
		$display("Number of tasks: %d", _task_count);
		$display("Number of errors: %d", _error_count);
		$display("FInal time: %t", $time);
		$display("\n\n");
	end
endtask






// ╭────────────────────╮
// │ SYNC BOUNDED TESTS │
// ╰────────────────────╯

// ┌                                            ┐
// │ These tasks are timebounded and blocking,  │
// │ that is, they are limited to the _max_time │
// │ variable and they block execution of the   │
// │ simulation.                                │
// └                                            ┘

task automatic wait_match;
	ref var signal;
	input var reference;

	integer time_count;

	fork
		begin
			task_inc;
			time_count = 0;

			$display("\n");
			$display("[START-%d] Waiting started at %t", _task_count, $time);

			while (signal != reference && time_count < _max_task_time) begin
				@(posedge tb_clk);
				time_count = time_count + 1;
			end

			if(time_count <  _max_task_time) begin
				$display("[MATCH-%d] Signals with value %h matched at %t",_task_count, reference, $time);
			end else begin
				$display("[MISS-%d] Signals do not match in the timeframe", _task_count);
				$display("[INFO-%d] Target: %h, Reference: %h, time: %t", _task_count, signal, reference, $time);
				error_inc;
			end
		end
	join_none
	#0;
endtask



task wait_for_state;
	input [31:0] expected;

	integer      time_count;

	begin
		task_inc;
		time_count = 0;

		while (db_wire != expected && time_count < _max_task_time) begin
			@(posedge tb_clk);
			time_count = time_count + 1;
		end

		if(time_count < _max_task_time) begin
			$display("[MATCH] flag 0x%h reached at %t", expected, $time);
		end else begin
			$display("[MISS] flag 0x%h not reached - current time: %t", expected, $time);
			error_inc;
		end
	end
endtask




task assert_debug;
	input   [31:0] expected;

	integer        saved;
	integer        time_count;

	begin
		task_inc;
		time_count = 0;

		while (db_mode != 1'b1 && time_count < _max_task_time) begin
			@(posedge tb_clk);
			time_count = time_count + 1;
		end

		if(time_count < _max_task_time) begin
			#cpu_period
			saved = expected;
			time_count = 0;

			while (db_wire == saved && time_count < _max_task_time) begin
				@(posedge tb_clk);
				time_count = time_count + 1;
			end

			if(time_count < _max_task_time) begin
				#cpu_period
				#cpu_period
				time_count = 0;

				while (db_wire != expected && time_count < _max_task_time) begin
					@(posedge tb_clk);
					time_count = time_count + 1;
				end
				
				if(time_count < _max_task_time) begin
					time_count = 0;

					while (db_mode != 1'b0 && time_count < _max_task_time) begin
						@(posedge tb_clk);
						time_count = time_count + 1;
					end

					if(time_count < _max_task_time) begin
						$display("[MATCH-%d] Debug completed at %t", _task_count, $time);
					end else begin
						$display("[MISS] Debug mode not finished correctly, time: %t", $time);
						error_inc;
					end

				end else begin
					$display("[MISS] Debug value is not correct, time: %t", $time);
					error_inc;
				end

			end else begin
				$display("[MISS] Debug value didn't change in the time frame, time: %t", $time);
				error_inc;
			end

		end else begin
			$display("[MISS] Debug mode did not start correctly in the time frame, time: %t", $time);
			error_inc;
		end

	end
endtask






// ╭──────────────────────╮
// │ UNBOUNDED SYNC TASKS │
// ╰──────────────────────╯

// ┌                                              ┐
// │ These tasks stop simulation execution but    │
// │ they don't have a time limit, so they won't  │
// │ check for errors and can trap the simulation │
// │ indefinetily                                 │
// └                                              ┘

task sync_state;
	input [31:0] expected;

	begin
		task_inc;
		while (db_wire != expected) begin
			@(posedge tb_clk);
		end
		$display("[INFO] state 0x%h reached at %t", expected, $time);
	end
endtask


task sync_main;

	begin
		task_inc;
		while(db_wire != 32'h55555555) begin
			@(posedge tb_clk);
		end

		$display("[MATCH]: MAIN REACHED AT %t", $time);

		while(db_wire == 32'h55555555) begin
			@(posedge tb_clk);
		end
	end
endtask

//Any signal matches any reference
task sync_signal;
	ref var signal;
	input var reference;

	begin
		task_inc;
		while (signal != reference) begin
			@(posedge tb_clk);
		end
		$display("[MATCH] Signal equaled 0x%h at %t", reference, $time);
	end
endtask


task sync_readable_debug;
	begin
		task_inc;

		// Wait while debug_wire contains X or Z (unstable/uninitialized)
		while (^db_wire === 1'bx) begin
			@(posedge tb_clk);
		end
		$display("[INFO] db_wire stabilized at %t with value 0x%h", $time, db_wire);

	end
endtask












 // ╭─────────────────────╮
 // │ BOUNDED ASYNC TESTS │
 // ╰─────────────────────╯

task automatic assert_signal_after_signal;
	ref var first_watch;
	ref var second_watch;

	input var first_reference;
	input var second_reference;

	integer time_count;

	fork
		begin
			task_inc;

			time_count = 0;
			while (first_watch != first_reference && time_count < _max_task_time) begin
				@(posedge tb_clk);
				time_count = time_count + 1;
			end

			if(time_count >= _max_task_time) begin
				$display("[MISS-%d] First value not reached in: %t", _task_count, $time);
				error_inc;
			end else begin
				$display("[INFO-%d] First value reached at %t, waiting for the second", _task_count, $time);
				time_count = 0;
				while (second_watch != second_reference && time_count < _max_task_time) begin
					@(posedge tb_clk);
					time_count = time_count + 1;
				end

				if(time_count >= _max_task_time) begin
					$display("[MISS-%d] Second value not reached in: %t", _task_count, $time);
					error_inc;
				end else begin
					$display("[MATCH-%d] Second value reached at %t", _task_count, $time);
				end
			end
		end
	join_none

	#0;

endtask


