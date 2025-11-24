//   ╔═══════════════════════════════════════════════════════════╗
//   ║                                                           ║
//   ║   ██████╗ ███████╗██████╗ ██╗   ██╗ ██████╗     ██████╗   ║
//   ║   ██╔══██╗██╔════╝██╔══██╗██║   ██║██╔════╝    ██╔════╝   ║
//   ║   ██║  ██║█████╗  ██████╔╝██║   ██║██║  ███╗   ██║        ║
//   ║   ██║  ██║██╔══╝  ██╔══██╗██║   ██║██║   ██║   ██║        ║
//   ║   ██████╔╝███████╗██████╔╝╚██████╔╝╚██████╔╝██╗╚██████╗   ║
//   ║   ╚═════╝ ╚══════╝╚═════╝  ╚═════╝  ╚═════╝ ╚═╝ ╚═════╝   ║
//   ║                                                           ║
//   ╚═══════════════════════════════════════════════════════════╝

// ┌                                                                     ┐
// │ Some macros to facilitate debugging in the C program, it uses a     │
// │ specific PIO to write data that can be easily read in a testbench   │
// │ or in a waveforms, it's usually best and more complete to use       │
// │ a JTAG debugger, but these macros are best for simpler settings.    │
// │                                                                     │
// │ The macros were made so that they work when ENABLE_DEBUG is defined │
// │ and they turn into comments and whitespace when ENABLE_DEBUG is     │
// │ not defined.                                                        │
// │                                                                     │
// │ These are not meant to be used in the final project.                │
// └                                                                     ┘


#define ENABLE_DEBUG // just comment this out or pass as arguments

#include "mem_map.h"
#include "io_helper.h"


// No-reloading guard
#ifndef DEBUG_GUARD
	#define DEBUG_GUARD

	typedef enum {
		NORMAL  = 0x00,
		SUCCESS = 0xF0,
		FAIL    = 0xFF
	} STATUS_T;

	typedef enum {
		MAIN  = 0x00,
		WHILE = 0x01,
		FUNC  = 0x02,
		SETUP = 0x03,
		ISR   = 0x04
	} SECTION_TYPE_T;


	#ifdef ENABLE_DEBUG

		static s_out_t debug_mode_bit = {
			.regs = (out_t*) PINS_C_0,
			.mask = 0x00000001,
			.offset = 0
		};
		
		static s_out_t debug_register = {
			.regs = (out_t*) PINS_C_1,
			.mask = 0xFFFFFFFF,
			.offset = 0
		};
		
		#define ON_VALUE             io_write(&debug_mode_bit, 0);
		#define OFF_VALUE            io_write(&debug_mode_bit, 1);
		#define write_debug_wire(n)  io_write(&debug_register, n);


		#define STATE(a,b,c,d) \
			write_debug_wire(((a)<<24)+((b)<<16)+((c)<<8)+((d)<<0))


		#define DEBUG(n) \
			ON_VALUE \
			write_debug_wire(n) \
			OFF_VALUE

	
		#define timer_conversion_factor     (10)
	

	#else

		#define ON_STATE  /*   */
		#define ON_DEBUG  /*   */

		#define DEBUG(n)  /* n */
		#define STATE(n)  /* n */

		#define timer_conversion_factor     (25000)


	#endif


	#define MS2CYCLES(n)   (((n)*(timer_conversion_factor))-1)

#endif // closing DEBUG_GUARD

