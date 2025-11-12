onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tbench/test_mode
add wave -noupdate /tbench/fetch_enable
add wave -noupdate /tbench/clock_gating
add wave -noupdate /tbench/tb_clk
add wave -noupdate /tbench/jtag_reset
add wave -noupdate /tbench/key_reset
add wave -noupdate /tbench/KEY_r
add wave -noupdate /tbench/sw_in
add wave -noupdate /tbench/ledr_out
add wave -noupdate /tbench/gpio_0
add wave -noupdate /tbench/gpio_0_drive
add wave -noupdate /tbench/gpio_0_en
add wave -noupdate /tbench/gpio_1
add wave -noupdate /tbench/gpio_1_drive
add wave -noupdate /tbench/gpio_1_en
add wave -noupdate /tbench/debug_wire
add wave -noupdate /tbench/dut/u0/gpio_0/bidir_port
add wave -noupdate /tbench/dut/u0/gpio_0/irq
add wave -noupdate /tbench/dut/u0/gpio_0/chipselect
add wave -noupdate /tbench/dut/u0/gpio_0/writedata
add wave -noupdate /tbench/dut/u0/gpio_0/data_dir
add wave -noupdate /tbench/dut/u0/gpio_0/irq_mask
add wave -noupdate /tbench/dut/u0/gpio_0/edge_detect
add wave -noupdate /tbench/dut/u0/gpio_0/edge_capture
add wave -noupdate /tbench/dut/u0/gpio_0/irq_mask
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {3270000 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 242
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {3146860 ps} {3358064 ps}
