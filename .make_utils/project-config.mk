# ============================================
#  --------------GLOBAL FRAGMENT-------------
# ============================================
#  Make fragment with all main definitions for
#  project, paths, names, values...
# ============================================



#...................Names....................

PROJECT_NAME=pulpino_qsys_test
QSYS_NAME=sys



#..................Compilation................

BOOT_ADDRESS=0x8000
CFLAGS = -Os -march=rv32imc_zicsr_zifencei -mabi=ilp32 -fdata-sections -ffunction-sections -fshort-enums -fgnu89-inline -Wall
LDFLAGS = -Wl,--gc-sections --specs=nano.specs -nostartfiles
LIBS =
TOOLCHAIN_COMPILATION_FLAGS= --enable-multilib

#...................Quartus....................

#Set to "true" if quartus can't reload memory normally (ie. Quartus 24.1)
ALTERNATIVE_MEM_RELOAD=true 