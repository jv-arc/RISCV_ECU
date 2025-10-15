# ========================================
# Makefile to facilitate quartus workflow
# ========================================

PROJECT_NAME=pulpino_qsys_test

compile: 
	quartus_sh --flow compile $(PROJECT_NAME)

reload_memory: compile 
	quartus_cdb $(PROJECT_NAME) -c $(PROJECT_NAME) --update_mif  


run_tb: reload_memory compile
	vsim -do simulation/questa/pulpino_qsys_test_run_msim_rtl_verilog.do   


.PHONY: run_tb reload_memory compile