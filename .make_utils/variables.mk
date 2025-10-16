# ============================================
#  -----------VARIABLES FRAGMENT-------------
# ============================================
#  Make fragment with all definitions for
#  folders, paths and variables
# ============================================


Q_DIR=$(PROJECT_DIR)/quartus_project
STAMPS_DIR=$(PROJECT_DIR)/.make_utils/stamps
COMPILER_SOURCE_DIR=$(PROJECT_DIR)/riscv-gnu-toolchain
TOOLCHAIN_DIR=$(PROJECT_DIR)/toolchain
TOOLS_DIR=$(PROJECT_DIR)/tools
QUESTA_DIR=$(Q_DIR)/simulation/questa
OUTPUT_DIR=$(Q_DIR)/output_files

SW_DIR=$(Q_DIR)/sw
MEM_DIR=$(SW_DIR)/mem_init
SRC_DIR=$(SW_DIR)/source_code


LINKER_SCRIPT=$(SW_DIR)/link.riscv.ld
VERILOG_SOURCES := $(shell find $(Q_DIR) -name '*.v' -name '*.sv')
QIP_FILES := $(wildcard $(Q_DIR)/*.qip)

test_bench_script=$(QUESTA_DIR)/$(PROJECT_NAME)_run_msim_rtl_verilog.do
END_SOF= $(OUTPUT_DIR)/$(PROJECT_NAME).sof
END_QSF= $(Q_DIR)/$(PROJECT_NAME).qsf
ELF_INTERMEDIATE = $(SW_DIR)/bundle.elf
BIN2HEX = $(TOOLS_DIR)/bin2hex

SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.s) $(wildcard $(SRC_DIR)/*.S)
DEPS = $(wildcard $(SRC_DIR)/*.h)
OBJS = $(wildcard $(SRC_DIR)/*.o)

CC = $(TOOLCHAIN_DIR)/bin/riscv64-unknown-elf-gcc
ELFSIZE = $(TOOLCHAIN_DIR)/bin/riscv64-unknown-elf-size
OBJCOPY = $(TOOLCHAIN_DIR)/bin/riscv64-unknown-elf-objcopy

MEM = $(MEM_DIR)/sys_onchip_memory2_0

TESTBENCH=$(Q_DIR)/rtl/pulpino_qsys_test.v



DESIGN_COMPILE_STAMP = $(STAMPS_DIR)/quartus_compiled
TB_COMPILE_STAMP=$(STAMPS_DIR)/testbench_compiled
MEM_STAMP= $(STAMPS_DIR)/memory_stamp
SYNTHESIS_STAMP = $(STAMPS_DIR)/synthesis_stamp
ASSEMBLY_STAMP = $(STAMPS_DIR)/assembly_stamp
FITTING_STAMP = $(STAMPS_DIR)/fitting_stamp
TOOLCHAIN_COMPILATION_STAMP=$(STAMPS_DIR)/toolchain_built

QSYS_FILE=$(Q_DIR)/$(QSYS_NAME)/synthesis/$(QSYS_NAME).qip
QSYS_SRC=$(Q_DIR)/$(QSYS_NAME).qsys