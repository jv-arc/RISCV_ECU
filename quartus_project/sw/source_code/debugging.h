// ╔═══════════════════════════════════════════════════════════╗
// ║                                                           ║
// ║   ██████╗ ███████╗██████╗ ██╗   ██╗ ██████╗     ██████╗   ║
// ║   ██╔══██╗██╔════╝██╔══██╗██║   ██║██╔════╝    ██╔════╝   ║
// ║   ██║  ██║█████╗  ██████╔╝██║   ██║██║  ███╗   ██║        ║
// ║   ██║  ██║██╔══╝  ██╔══██╗██║   ██║██║   ██║   ██║        ║
// ║   ██████╔╝███████╗██████╔╝╚██████╔╝╚██████╔╝██╗╚██████╗   ║
// ║   ╚═════╝ ╚══════╝╚═════╝  ╚═════╝  ╚═════╝ ╚═╝ ╚═════╝   ║
// ║                                                           ║
// ╚═══════════════════════════════════════════════════════════╝

// ┌                                                                    ┐
// │ Some macros to facilitate debugging in the C program, it uses a    │
// │ specific PIO to write data that can be easily read in a testbench  │
// │ or in a waveforms, it's usually best and more complete to use      │
// │ a JTAG debugger, but these macros are best for simpler settings.   │
// │                                                                    │
// │ The macros were made so that they work when DEBUG_FLAG is          │
// │ defined and they turn into comments and whitespace when DEBUG_FLAG │
// │ is not defined.                                                    │
// │                                                                    │
// │ These are not meant to be used in the final project.               │
// └                                                                    ┘


#define DEBUG_FLAG // just comment this or pass it

#include "mem_map.h"

// No-reloading guard
#ifndef DEBUG_GUARD
	#define DEBUG_GUARD



	#ifdef DEBUG_FLAG

		// Addresses
		#define dbg_mode_addr               (GPIO_C_1)
		#define dbg_mode_mask               (0x00000001)
		#define dbg_addr                    (GPIO_C_2)


		// Debug Control
		#define db_st_toggle                ((*((volatile uint32_t*) (dbg_mode_addr))
		#define ON_STATE                    (db_st_toggle |= dbg_mode_mask)
		#define ON_DEBUG                    (db_st_toggle &= (~dbg_mode_mask))
		#define DEBUG(n)                    ((*((volatile uint32_t*) (dbg_mode_addr)) = (n))


		// Timer
		#define timer_conversion_factor     (10)
	

	#else


		// Addresses
		#define dbg_addr                    /*n*/
		#define dbg_mode_addr               /*n*/
		#define dbg_mode_mask               /*n*/


		// Debug Control
		#define db_st_toggle                /*n*/
		#define ON_STATE                    /*n*/
		#define ON_DEBUG                    /*n*/
		#define DEBUG(n)                    /*n*/


		// Timer
		#define timer_conversion_factor     (25000)


	#endif


	// Independent of DEBUG_FLAG:
	#define MS2CYCLES(n)                   (((n)*(timer_conversion_factor))-1)

	typedef enum {
		NORMAL  = 0x00,
		SUCCESS = 0xF0,
		FAILURE = 0xFF
	} STATUS_T;

	typedef enum {
		MAIN  = 0x00,
		WHILE = 0x01,
		FUNC  = 0x02,
		SETUP = 0x03,
		ISR   = 0x04
	} SECTION_TYPE_T;

	#define STATE(a,b,c,d)                DEBUG( ((a)<<24) + ((b)<<16) + ((c)<<8) + ((d)<<0) )


#endif // closing DEBUG_GUARD

