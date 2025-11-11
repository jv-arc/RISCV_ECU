function automatic integer get_gpio_bank;
	input  integer gpio_number;
		begin

		if(gpio_number < 32) begin
			get_gpio_bank = 0;
		end else
		
		if(gpio_number < 36) begin
			get_gpio_bank = 2;
		end else

		if(gpio_number < 68) begin
			get_gpio_bank = 1;
		end else

		if(gpio_number < 72) begin
			get_gpio_bank = 2;
		end

		else begin
			get_gpio_bank = -1;
		end

	end
endfunction


function automatic integer get_gpio_pos;
	input  integer gpio_number;
	integer bank;
	begin

		bank = get_gpio_bank(gpio_number);

		if(bank == 0) begin
			get_gpio_pos = gpio_number;
		end else
	
		if(bank == 1) begin
			get_gpio_pos = gpio_number - 36;
		end else

		if(bank == 2) begin
			if(gpio_number < 36) begin
				get_gpio_pos = gpio_number - 32;
			end else begin
				get_gpio_pos = gpio_number - 64; // (+4 -68)
			end
		end

		else begin
			get_gpio_pos = -1;
		end
	end
endfunction
