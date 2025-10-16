# ============================================
#  ----------SOURCE CODE FRAGMENTS-----------
# ============================================
#  Make fragment for compiling, linking and 
#  assemblying all source code files and
#  generating the necessary memory file 
# ============================================


generate-memory: $(MEM).hex

$(MEM).hex: $(MEM).bin
	$(BIN2HEX) $(MEM).bin $(MEM).hex


compile-source-code: $(OBJS) 

$(OBJS): $(SRCS) $(DEPS)
	cd $(SW_DIR) && \
	$(CC) -c $(CFLAGS) -o $@ $<


link-executable: $(ELF_INTERMEDIATE)

$(ELF_INTERMEDIATE): $(OBJS)
	cd $(SW_DIR) && \
	$(CC) -o $@ $(OBJS) -T $(LINKER_SCRIPT) $(LDFLAGS) && \
	$(ELFSIZE) $@




generate-binary: $(MEM).bin

$(MEM).bin: $(ELF_INTERMEDIATE)
	$(OBJCOPY) --change-addresses -$(BOOT_ADDRESS) -O binary --gap-fill 0 $(ELF_INTERMEDIATE) $(MEM).bin


clean-memory-files:
	rm -f $(ELF_INTERMEDIATE) $(MEM_DIR)/*.hex $(MEM_DIR)/*.bin && \
	rm $(MEM_STAMP)



.PHONY: clean-memory-files compile-source-code link-executable generate-memory generate-binary