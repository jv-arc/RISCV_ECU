#include <stdint.h>


// ╔═════════════════════╗
// ║ ██████╗ ██╗ ██████╗ ║
// ║ ██╔══██╗██║██╔═══██╗║
// ║ ██████╔╝██║██║   ██║║
// ║ ██╔═══╝ ██║██║   ██║║
// ║ ██║     ██║╚██████╔╝║
// ║ ╚═╝     ╚═╝ ╚═════╝ ║
// ╚═════════════════════╝


// ┌                                                       ┐
// │ These are structs and functions to safelly access     │
// │ peripherals, since some GPIOs share memory space      │
// │ with peripherals there are some abstraction functions │
// │ to access these peripherals safely                    │
// └                                                       ┘


#ifndef PIO_H
#define PIO_H

// ╭─────────────────╮
// │ PIO DEFINITIONS │
// ╰─────────────────╯

  // GENERAL

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t DIRECTION;
		volatile uint32_t INT_MASK;
		volatile uint32_t EDGE_CAPTURE;
		volatile uint32_t OUT_SET;
		volatile uint32_t OUT_CLEAR;
	} pio_t;


	// Only INPUT

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t _unused;
		volatile uint32_t INT_MASK;
		volatile uint32_t EDGE_CAPTURE;
	} in_t;


	// Only OUTPUT

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t _unused[3];
	} out_t;


	//GPIO BANK

	typedef struct{
		volatile uint32_t READ;
		volatile uint32_t _unused0;
		volatile uint32_t INT_MASK;
		volatile uint32_t EDGE_CAPTURE;
		volatile uint32_t WRITE;
		volatile uint32_t _unused1[3];
		volatile uint32_t DIRECTION;
		volatile uint32_t _unused2[3];
	} gpio_t;




	// ╭──────────────╮
	// │ SAFE STRUCTS │
	// ╰──────────────╯

	// GPIO
	typedef struct {
		volatile gpio_t* regs;
		uint32_t mask;
	} s_gpio_t;

	//Input
	typedef struct{
		volatile in_t* regs;
		uint32_t mask;
		uint32_t offset;
	} s_in_t;
	

	//Output
	typedef struct{
		volatile out_t* regs;
		uint32_t mask;
		uint32_t offset;
	} s_out_t;



	
	// ╭────────────────╮
	// │ GPIO FUNCTIONS │
	// ╰────────────────╯

	static inline void gpio_write(const s_gpio_t* bank, uint32_t value){
		uint32_t old_masked_value = (bank->regs->WRITE & ~(bank->mask));
		bank->regs->WRITE = (old_masked_value | (bank->mask & value));
	}
	static inline void gpio_int_mask(const s_gpio_t* bank, uint32_t value){
		uint32_t old_masked_value = (bank->regs->INT_MASK & ~(bank->mask));
		bank->regs->INT_MASK = (old_masked_value | (bank->mask & value));
	}
	static inline void gpio_direction(const s_gpio_t* bank, uint32_t value){
		uint32_t old_masked_value = (bank->regs->DIRECTION & ~(bank->mask));
		bank->regs->DIRECTION = (old_masked_value |(bank->mask & value));
	}
	static inline void gpio_edge_clear(const s_gpio_t* bank, uint32_t value){
		// Writing 0 does nothing
		bank->regs->EDGE_CAPTURE = (bank->mask & value);
	}
	static inline void gpio_intconfig_set(const s_gpio_t* bank, uint32_t pattern){
		gpio_edge_clear(bank, pattern); //So we don't enable pending interruptions
		uint32_t masked_pattern = (bank->mask & pattern);
		bank->regs->INT_MASK |= masked_pattern;
	}
	static inline void gpio_intconfig_clr(const s_gpio_t* bank, uint32_t pattern){
		uint32_t masked_pattern = (bank->mask & pattern);
		bank->regs->INT_MASK &= ~masked_pattern;
	}
	static inline uint32_t gpio_read(const s_gpio_t* bank){
		return (bank->regs->READ & bank->mask);
	}
	static inline uint32_t gpio_edge_read(const s_gpio_t* bank){
		return (bank->regs->EDGE_CAPTURE & bank->mask);
	}
	static inline void gpio_set_input(const s_gpio_t* bank, uint32_t pattern){
		uint32_t masked_pattern = (bank->mask & pattern);
		bank->regs->DIRECTION |= masked_pattern;
	}
	static inline void gpio_set_output(const s_gpio_t* bank, uint32_t pattern){
		uint32_t masked_pattern = (bank->mask & pattern);
		bank->regs->DIRECTION &= ~masked_pattern;
	}




// ╭──────────────────────╮
// │ GENERAL IO FUNCTIONS │
// ╰──────────────────────╯

static inline void io_int_mask(const s_in_t* bank, uint32_t value){
	uint32_t old_masked_value = (bank->regs->INT_MASK & ~(bank->mask));
	uint32_t new_masked_value = ((value << bank->offset) & bank->mask);
	bank->regs->INT_MASK = (old_masked_value | new_masked_value);
}

static inline uint32_t io_read(const s_in_t* bank){
	uint32_t masked_register_value = (bank->regs->DATA & bank->mask);
	return (masked_register_value >> bank->offset);
}

static inline uint32_t io_edge_read(const s_in_t* bank){
	uint32_t masked_register_value = (bank->regs->EDGE_CAPTURE & bank->mask);
	return (masked_register_value >> bank->offset);
}

static inline void io_edge_clear(const s_in_t* bank){
	//Writing 0 does nothing
	bank->regs->EDGE_CAPTURE = (bank->mask);
}
static inline void io_intconfig_set(const s_in_t* bank, uint32_t pattern){
	io_edge_clear(bank);
	uint32_t masked_pattern = (bank->mask & (pattern<<bank->offset));
	bank->regs->INT_MASK |= masked_pattern;
}

static inline void io_intconfig_clr(const s_in_t* bank, uint32_t pattern){
	uint32_t masked_pattern = (bank->mask & (pattern<<bank->offset));
	bank->regs->INT_MASK &= ~masked_pattern;
}

static inline void io_write(const s_out_t* bank, uint32_t value){
	uint32_t old_masked_value = (bank->regs->DATA & ~(bank->mask));
	uint32_t new_masked_value = ((value << bank->offset) & bank->mask);
	bank->regs->DATA = (old_masked_value | new_masked_value);
}



#endif
