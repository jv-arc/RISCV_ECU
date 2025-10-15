# Folders

CURRENT_PATH = $(shell pwd)

TOOLCHAIN_DIR = /opt/riscv/bin
MEM_DIR = $(CURRENT_PATH)/mem_init
TOOL_DIR = $(CURRENT_PATH)/../../tools
SRC_DIR = $(CURRENT_PATH)/source_code

# USED PROGRAMS
CC = $(TOOLCHAIN_DIR)/riscv64-unknown-elf-gcc
ELFSIZE = $(TOOLCHAIN_DIR)/riscv64-unknown-elf-size
OBJCOPY = $(TOOLCHAIN_DIR)/riscv64-unknown-elf-objcopy
BIN2HEX = $(TOOL_DIR)/bin2hex


# FLAGS
CFLAGS = -Os -march=rv32imc_zicsr_zifencei -mabi=ilp32 -fdata-sections -ffunction-sections -fshort-enums -fgnu89-inline -Wall
LDFLAGS = -Wl,--gc-sections -Tlink.riscv.ld --specs=nano.specs -nostartfiles
LIBS =


# FILES
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.s) $(wildcard $(SRC_DIR)/*.S)
DEPS = $(wildcard $(SRC_DIR)/*.h)
ELF_INTERMEDIATE = ./bundle.elf
MEM = $(MEM_DIR)/sys_onchip_memory2_0



all: $(MEM).hex


# Compilacao
compile: $(ELF_INTERMEDIATE) 

$(ELF_INTERMEDIATE): $(SRCS) | $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	$(ELFSIZE) $@

_compile:
	$(CC) -o $(ELF_INTERMEDIATE) $(SRCS)  $(CFLAGS) $(LDFLAGS)
	$(ELFSIZE) $(ELF_INTERMEDIATE)


$(MEM).hex: $(ELF_INTERMEDIATE)
	$(OBJCOPY) --change-addresses -0x8000 -O binary --gap-fill 0 $< $(MEM).bin
	$(BIN2HEX) $(MEM).bin $(MEM).hex


# Clean
clean:
	rm -f *.elf $(MEM_DIR)/*.hex $(MEM_DIR)/*.bin



.PHONY: clean rv-reprogram mem_init_generate all compile _compile