
#ifndef STRUCTS_GUARD //guard to avoid loading structs twice
#define STRUCTS_GUARD


	/*
	*=======================================================
	*                     CONVENTIONS
	*------------------------------------------------------
	*  - Structs and C related custom types use lowercase
	*  name in snake case with a trailing '_t', '<type>_t'
	*
	*  - Macros for casting addresses to these types
	*  follow the same patter but with uppercase letters
	*  followed by '_T', like: '<TYPE>_T>'
	*
	*  - Macros for getting a 'pointer' to a specific type
	*  use the type's name in upper case followed by '_P'
	*  like '<TYPE>_P'
	*
	*  Example:
	*    type:    timer32_t
	*    casting: TIMER32_T
	*    pointer: TIMER32_P
	*
	*
	*-------------------------------------------------------
	*
	*  QUESTION: Why pointer macros???
	*
	*    The best recommended practice is to use absolute
	*  well defined, addresses in macros, I agree with that!
	*   A specific device should use an address defined in a
	*  macro with the same name followed by '_ADDRESS'.
	*
	*    So "LED_BANK_A" should be defined as:
	*  "#define LED_BANK_A  (PIO_T(LED_BANK_A_ADDRESS))"
	*  The point is that this is not always easy to find or to
	*  to access. Can you guarantee, the best practices
	*  will always be followed??? I don't think so.
	*  the <TYPE>_P is a convenient way of solving this.
	*
	*    But the usage of "<DEVICE>_ADDRESS" is totally
	*  encouraged!
	*
	*
	*  REASONING: why not bitfields?
	*
	*     Because they are hard to control in the compiler
	*  and in different architectures, just read the manuals
	*  and use bitwise and bit shift operations.
	*========================================================
	*/




	
	//=================BASIC DEFINITIONS==================

	// Standard Libraries have their own guards
	#include <stdint.h>

	// Simple address to uin32_t casting as a "register"
	#define REG_T(addr)      (*((volatile uint32_t*) (addr)))

	// Casts any "register" to a uin32_t pointer
  #define REG_P(REG)       ((volatile uint32_t*) (&(REG)))








	//==================CHARACTER BUFFER===================
	// Basically to avoid writing blocking code for the 
	// JTAG peripheral, we just write and read from the
	// buffer and we let JTAG work based on it's ISR.
  //
  // This doesn't correspond to any hardware device
	//


	// allows defining a different buffer_size on the code
	#ifndef BUFFER_SIZE
		#define BUFFER_SIZE 128
	#endif
	
	// The data, and 2 addresses, for the beginning and the end
	typedef struct{
		volatile uint8_t  DATA[BUFFER_SIZE]; //not struct size!
		volatile uint32_t HEAD;
		volatile uint32_t TAIL;
	} buffer_t;


	// Casts addr to buffer_t struct
	#define BUFFER_T(addr)         (*((volatile buffer_t*) (addr)))

	// Returns the address of a buffer
	#define BUFFER_P(BUFFER)       ((volatile buffer_t*) (&(BUFFER)))

	// Get memory addresses for any amount of buffers
	#define BUFFER_OVERHEAD        (((BUFFER_SIZE)*(1)) + (4)*(2))
	#define BUFFER_OFFSET(base,n)  ((base)+((BUFFER_OVERHEAD)*(n)))








	//====================================================================
	// MY LIFE WAS A LIE, THERE IS NO EVENT UNIT IN THE PROJECT!!!!!!!!!!!
	// THIS IS JUST GHOST CODE!!!!!!!
	//
	// I WAS WRITING CONTROL DATA INTO UNMAPPED MEMORY THINKING IT WAS
  // CRUCIAL, BUT IT WAS USELESS
	//
	// Let this code here be a testment to my lack of debugging skills, and
	// also to be used if one day I use the Pulpino event unit
	//
	//=================STRUCTS AND MACROS FOR EVENT UNIT==================
	// The event unit controls both sleep, interruptions and events. This
  // is a bit different from traditional RISC-V implementations, and was	     <== Pure baloney
	// not defined in this project, but it's actually inherited from the
  // 'zero-riscy' implementation. As far as I know it's one of the only
  // peripherals being used on the project that can be found on the
  // original PULPino datasheet.
	//


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









	//====================PIO DEFINITIONS=====================
	// This is based on the Altera/Interl IP for the 'pio'
	// component in Qsys, it is used for many different I/O.
	// The register mapping is always the same, but some registers
  // might not be available.
	//
	// Example: if DIRECTION isn't available at a specific config
	// INT_MASK stays in the same place, not moving to take
	// DIRECTION's memory address on the mapping.
	//
	// The DATA register can be used to read and write anything but
	// the write operation must provide the full value. Meanwhile
	// the OUT_SET and OUT_CLEAR ones cannot be read but they don't
	// need the whole value. It only clears or sets bits you write
	// as 1. This is useful for atomic operations.
	//

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t DIRECTION;
		volatile uint32_t INT_MASK;
		volatile uint32_t EDGE_CAPTURE;
		volatile uint32_t OUT_SET;
		volatile uint32_t OUT_CLEAR;
	} pio_max_t;

	#define PIO_MAX_T(addr) ( *((volatile pio_max_t*) (addr)) )
	#define PIO_MAX_P(PIO) ( (volatile pio_max_t*) (&(PIO)) )





	//=================PIO MINIMAL DEFINITION==================

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t DIRECTION;
		volatile uint32_t INT_MASK;
		volatile uint32_t EDGE_CAPTURE;
	} pio_min_t;

	#define PIO_MIN_T(addr) (*((volatile pio_min_t*) (addr)))
	#define PIO_MIN_P(PIO)  ((volatile pio_min_t*) (&(PIO)))



	

	//=================GPIO_BANK==================

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



	//=================JTAG UART DEFINITIONS==================

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t CONTROL;
	} jtag_uart_t;

	#define JTAG_UART_T(addr)       (*((volatile jtag_uart_t*) (addr)))
	#define JTAG_UART_P(JTAG_UART)  ((volatile jtag_uart_t*) (&(JTAG_UART)))








	//=================TIMER32 DEFINITIONS==================

	typedef struct {
		volatile uint32_t STATUS;
		volatile uint32_t CONTROL;
		volatile uint32_t PERIOD_L;
		volatile uint32_t PERIOD_H;
		volatile uint32_t SNAP_L;
		volatile uint32_t SNAP_H;
	} timer32_t;

	// Casts addr to timer32_t struct
	#define TIMER32_T(addr)    (*((volatile timer32_t*) (addr)))

	// Get's a "pointer" to a timer32_t struct
	#define TIMER32_P(TIMER32) ((volatile timer32_t*) (&(TIMER32)))




	//=================TIMER64 DEFINITIONS==================

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

	// Casts addr to timer64_t struct
	#define TIMER64_T(addr)    (*((volatile timer64_t*) (addr)))

	// Get's a "pointer" to a timer64 struct
	#define TIMER64_P(TIMER64) ((volatile timer64_t*) (&(TIMER64)))





#endif // STRUCTS_GUARD
