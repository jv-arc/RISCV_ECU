
add wave -divider "CPU: Memory Access"
add wave -group "Load Store Unit" -label "Write Data" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/data_wdata_o
add wave -group "Load Store Unit" -label "Read Data" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/data_rdata_i
add wave -group "Load Store Unit" -label "Address" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/data_addr_o
add wave -group "Load Store Unit" -label "Write Enable" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/data_we_o
add wave -group "Instruction Load Unit" -label "Address" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/instr_addr_o
add wave -group "Instruction Load Unit" -label "Instruction" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/instr_rdata_i


add wave -divider "CPU: Instruction Fetch and Decode"
add wave  -label "Instruction Fetch" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/if_stage_i/pc_if_o
add wave  -label "Instruction Decode" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/if_stage_i/pc_id_o


add wave -divider "CPU:EX Stage"
add wave -group "ALU" -label "Operation" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/ex_block_i/alu_operator_i
add wave -group "ALU" -label "Operand A" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/ex_block_i/alu_operand_a_i
add wave -group "ALU" -label "Operand B" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/ex_block_i/alu_operand_b_i
add wave -group "ALU" -label "Comparison" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/ex_block_i/alu_cmp_result
add wave -group "ALU" -label "Is equal" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/ex_block_i/alu_is_equal_result
add wave -group "ALU" -label "Result" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/ex_block_i/alu_result
add wave -group "Branch" -label "Target" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/ex_block_i/jump_target_o
add wave -group "Branch" -label "Taken" sim:/tbench/dut/u0/pulpino_0/RISCV_CORE/ex_block_i/branch_decision_o
