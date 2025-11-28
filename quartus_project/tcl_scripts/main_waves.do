add wave -divider "DUT PORTS"
add wave -group "Basic Peripherals" -label "KEY"  sim:/tbench/dut/KEY
add wave -group "Basic Peripherals" -label "SW"   sim:/tbench/dut/SW
add wave -group "Basic Peripherals" -label "LEDR" sim:/tbench/dut/LEDR


add wave -divider "GPIO Banks"
add wave -group "GPIOs" -label "BANK 0" sim:/tbench/dut/GPIO_0
add wave -group "GPIOs" -label "BANK 1" sim:/tbench/dut/GPIO_1


add wave -divider "Memory Mapped Registers"
add wave -group "Bank A" -label "Read"  sim:/tbench/dut/u0/gpio_a_r_external_connection_export
add wave -group "Bank A" -label "Write"  sim:/tbench/dut/u0/gpio_a_w_external_connection_export
add wave -group "Bank A" -label "Select"  sim:/tbench/dut/u0/gpio_a_s_external_connection_export
add wave -group "Bank B" -label "Read"  sim:/tbench/dut/u0/gpio_b_r_external_connection_export
add wave -group "Bank B" -label "Write"  sim:/tbench/dut/u0/gpio_b_w_external_connection_export
add wave -group "Bank B" -label "Select"  sim:/tbench/dut/u0/gpio_b_s_external_connection_export
add wave -group "Bank C (shared)" -label "Read"  sim:/tbench/dut/u0/gpio_c_r_external_connection_export
add wave -group "Bank C (shared)" -label "Write 0" sim:/tbench/dut/u0/gpio_c0_w_external_connection_export
add wave -group "Bank C (shared)" -label "Write 1" sim:/tbench/dut/u0/gpio_c1_w_external_connection_export
add wave -group "Bank C (shared)" -label "Write 2" sim:/tbench/dut/u0/gpio_c2_w_external_connection_export


