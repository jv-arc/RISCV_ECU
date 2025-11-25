
#ifndef STRUCTS_GUARD //guard to avoid loading structs twice
#define STRUCTS_GUARD



// ╭─────────────╮
// │ CONVENTIONS │
// ╰─────────────╯

// ┌                                                    ┐
// │ - Structs and C related custom types use lowercase │
// │ name in snake case with a trailing '_t', '<type>_t'│
// │                                                    │
// │ - Macros for casting addresses to these types      │
// │ follow the same patter but with uppercase letters  │
// │ followed by '_T', like: '<TYPE>_T>'                │
// │                                                    │
// │ - Macros for getting a 'pointer' to a specific type│
// │ use the type's name in upper case followed by '_P' │
// │ like '<TYPE>_P'                                    │
// │                                                    │
// │ Example:                                           │
// │ type:    timer32_t                                 │
// │ casting: TIMER32_T                                 │
// │ pointer: TIMER32_P                                 │
// │                                                    │
// │ The only exceptions are the safety ones            │
// └                                                    ┘





// ╔═════════════════════════════════════╗
// ║ ██████╗  █████╗ ███████╗██╗ ██████╗ ║
// ║ ██╔══██╗██╔══██╗██╔════╝██║██╔════╝ ║
// ║ ██████╔╝███████║███████╗██║██║      ║
// ║ ██╔══██╗██╔══██║╚════██║██║██║      ║
// ║ ██████╔╝██║  ██║███████║██║╚██████╗ ║
// ║ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝ ╚═════╝ ║
// ╚═════════════════════════════════════╝
	
	// Standard Libraries have their own guards
	#include <stdint.h>

	// Simple address to uin32_t casting as a "register"
	#define REG_T(addr)      (*((volatile uint32_t*) (addr)))

	// Casts any "register" to a uin32_t pointer
  #define REG_P(REG)       ((volatile uint32_t*) (&(REG)))








  // ╭──────────────────╮
  // │ CHARACTER BUFFER │
  // ╰──────────────────╯

  // ┌                                                  ┐
  // │ Basically to avoid writing blocking code for the │
  // │ JTAG peripheral, we just write and read from the │
  // │ buffer and we let JTAG work based on it's ISR.   │
  // └                                                  ┘


	//Allows defining a different buffer_size on the code
	#ifndef BUFFER_SIZE
		#define BUFFER_SIZE 128
	#endif
	
	typedef struct{
		volatile uint8_t  DATA[BUFFER_SIZE]; //not struct size!
		volatile uint32_t HEAD;
		volatile uint32_t TAIL;
	} buffer_t;


	#define BUFFER_T(addr)         (*((volatile buffer_t*) (addr)))
	#define BUFFER_P(BUFFER)       ((volatile buffer_t*) (&(BUFFER)))

	// Size helpers
	#define BUFFER_OVERHEAD        (((BUFFER_SIZE)*(1)) + (4)*(2))
	#define BUFFER_OFFSET(base,n)  ((base)+((BUFFER_OVERHEAD)*(n)))





  // ╭──────────────────────────╮
  // │ INTERRUPT AND EVENT UNIT │
  // ╰──────────────────────────╯
 
  // ┌                                                  ┐
  // │ I created these because I thought it was being   │
  // │ used on the current pulpino configuration, it is │
  // │ not being used...                                │
  // │                                                  │
  // │ I'm leaving here if we need to use in the future │
  // └                                                  ┘

	// Sleep struct
	typedef struct{
		volatile uint32_t CONTROL;
		volatile uint32_t STATUS;
	} sleep_controller_t;


	// Struct for async controller (both interrupt and event controllers)
	typedef struct{
		volatile uint32_t ENABLE;
		volatile uint32_t PENDING_RW;
		volatile uint32_t PENDING_SET;
		volatile uint32_t PENDING_CLEAR;
	} async_controller_t;


	// Struct for the complete Event Unit
	typedef struct {
		async_controller_t INTER;
		async_controller_t EVENT;
		sleep_controller_t SLEEP;
	} event_unit_t;

	#define EVENT_UNIT_T(addr)       (*((volatile event_unit_t*) (addr)))
	#define EVENT_UNIT_P(EVENT_UNIT) ((volatile event_unit_t*) (&(EVENT_UNIT)))









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

	#define PIO_T(addr) ( *((volatile pio_t*) (addr)) )
	#define PIO_P(PIO) ( (volatile pio_t*) (&(PIO)) )


	// Only INPUT

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t _unused;
		volatile uint32_t INT_MASK;
		volatile uint32_t EDGE_CAPTURE;
	} in_t;

	#define INPUT_T(addr) (*((volatile in_t*) (addr)))
	#define INPUT_P(PIO)  ((volatile in_t*) (&(PIO)))


	// Only OUTPUT

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t _unused[3];
	} out_t;

	#define OUTPUT_T(addr) (*((volatile out_t*) (addr)))
	#define OUTPUT_P(PIO)  ((volatile out_t*) (&(PIO)))



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

	#define GPIO_T(addr)  (*((volatile gpio_t*) (addr)))
	#define GPIO_P(GPIO)  ((volatile gpio_t*) (&(GPIO)))








// ╭───────────╮
// │ JTAG UART │
// ╰───────────╯

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t CONTROL;
	} jtag_uart_t;

	#define JTAG_UART_T(addr)       (*((volatile jtag_uart_t*) (addr)))
	#define JTAG_UART_P(JTAG_UART)  ((volatile jtag_uart_t*) (&(JTAG_UART)))





// ╭─────────╮
// │ TIMER32 │
// ╰─────────╯

	typedef struct {
		volatile uint32_t STATUS;
		volatile uint32_t CONTROL;
		volatile uint32_t PERIOD_L;
		volatile uint32_t PERIOD_H;
		volatile uint32_t SNAP_L;
		volatile uint32_t SNAP_H;
	} timer32_t;

	#define TIMER32_T(addr)    (*((volatile timer32_t*) (addr)))
	#define TIMER32_P(TIMER32) ((volatile timer32_t*) (&(TIMER32)))




// ╭─────────╮
// │ TIMER64 │
// ╰─────────╯

	typedef struct {
		volatile uint32_t STATUS;
		volatile uint32_t CONTROL;
		volatile uint32_t PERIOD_0;
		volatile uint32_t PERIOD_1;
		volatile uint32_t PERIOD_2;
		volatile uint32_t PERIOD_3;
		volatile uint32_t SNAP_0;
		volatile uint32_t SNAP_1;
	} timer64_t;

	#define TIMER64_T(addr)    (*((volatile timer64_t*) (addr)))
	#define TIMER64_P(TIMER64) ((volatile timer64_t*) (&(TIMER64)))










//    ╔══════════════════════════════════╗
//    ║ ███████╗ █████╗ ███████╗███████╗ ║
//    ║ ██╔════╝██╔══██╗██╔════╝██╔════╝ ║
//    ║ ███████╗███████║█████╗  █████╗   ║
//    ║ ╚════██║██╔══██║██╔══╝  ██╔══╝   ║
//    ║ ███████║██║  ██║██║     ███████╗ ║
//    ║ ╚══════╝╚═╝  ╚═╝╚═╝     ╚══════╝ ║
//    ╚══════════════════════════════════╝

// ┌                                                       ┐
// │ These are structs and functions to safelly access     │
// │ peripherals, since some GPIOs share memory space      │
// │ with peripherals there are some abstraction functions │
// │ to access these peripherals safely                    │
// └                                                       ┘



  // ╭─────────╮
  // │ STRUCTS │
  // ╰─────────╯

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







// ╭────────────╮
// │ GENERAL IO │
// ╰────────────╯

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




#endif // STRUCTS_GUARD
