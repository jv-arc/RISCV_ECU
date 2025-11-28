add wave -group "IRQ Sources"
add wave -group "BANK_A" -label "Capture" sim:/tbench/dut/u0/gpio_a_r/edge_capture
add wave -group "BANK_A" -label "Mask" sim:/tbench/dut/u0/gpio_a_r/irq_mask
add wave -group "BANK_B" -label "Capture" sim:/tbench/dut/u0/gpio_b_r/edge_capture
add wave -group "BANK_B" -label "Mask" sim:/tbench/dut/u0/gpio_b_r/irq_mask
add wave -group "BANK_C" -label "Capture" sim:/tbench/dut/u0/gpio_c_r/edge_capture
add wave -group "BANK_C" -label "Mask" sim:/tbench/dut/u0/gpio_c_r/irq_mask
add wave -group "TIMER"  -label "Finished IRQ" sim:/tbench/dut/u0/timer_0/irq
