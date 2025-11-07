
#ifndef STRUCTS_GUARD //guard to avoid loading structs twice
#define STRUCTS_GUARD


	
	//=================BASIC DEFINITIONS==================

	//Avoid loading stdint.h twice
	#ifndef stdint
	 #include <stdint.h>
	#endif

	// Simple address to uin32_t casting as a register
	#define REG(addr) (*((volatile uint32_t*) (addr)))




	//=============CHARACTER BUFFER (FOR JTAG)=============

	// allows defining a different buffer_size elsewhere
	#ifndef BUFFER_SIZE
		#define BUFFER_SIZE 128
	#endif
	
	// The data, and 2 addresses, for the beggining and end
	typedef struct{
		volatile uint8_t DATA[BUFFER_SIZE];
		volatile uint32_t HEAD;
		volatile uint32_t TAIL;
	} buffer_t;

	// Casts addr to buffr_t struct
	#define BUFFER_T(addr) (*((volatile buffer_t*) (addr)))




	//=================BASIC DEFINITIONS==================
	
	typedef struct{
		volatile uint32_t CONTROL;
		volatile uint32_t STATUS;
	} sleep_t;

	// Casts addr to sleep_t struct
	#define SLEEP_T(addr) (*((volatile sleep_t*) (addr)))





	//=================BASIC DEFINITIONS==================

	typedef struct{
		volatile uint32_t ENABLE;
		volatile uint32_t PENDING_RW;
		volatile uint32_t PENDING_SET;
		volatile uint32_t PENDING_CLEAR;
	} event_t;

	// Casts addr to event_t struct
	#define EVENT_T(addr) (*((volatile event_t*) (addr)))





	//=================BASIC DEFINITIONS==================
	
	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t DIRECTION;
		volatile uint32_t INT_MASK;
		volatile uint32_t EDGE_CAPTURE;
		volatile uint32_t OUT_SET;
		volatile uint32_t OUT_CLEAR;
	} pio_t;

	// Casts addr to pio_t struct
	#define PIO_T(addr) (*((volatile pio_t*) (addr)))





	//=================BASIC DEFINITIONS==================

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t CONTROL;
	} jtag_uart_t;

	// Casts addr to jtag_uart_t struct
	#define JTAG_UART_T(addr) (*((volatile jtag_uart_t*) (addr)))





	//=================BASIC DEFINITIONS==================

	typedef struct {
		volatile uint32_t STATUS;
		volatile uint32_t CONTROL;
		volatile uint32_t PERIOD_L;
		volatile uint32_t PERIOD_H;
		volatile uint32_t SNAP_L;
		volatile uint32_t SNAP_H;
	} interval_timer32_t;

	// Casts addr to interval_timer32_t struct
	#define TIMER32_T(addr) (*((volatile interval_timer32_t*) (addr)))





	//=================BASIC DEFINITIONS==================

	typedef struct {
		volatile uint32_t STATUS;
		volatile uint32_t CONTROL;
		volatile uint32_t PERIOD_0;
		volatile uint32_t PERIOD_1;
		volatile uint32_t PERIOD_2;
		volatile uint32_t PERIOD_3;
		volatile uint32_t SNAP_0;
		volatile uint32_t SNAP_1;
	} interval_timer64_t;

	// Casts addr to interval_timer64_t struct
	#define TIMER64_T(addr) (*((volatile interval_timer64_t*) (addr)))

#endif // STRUCTS_GUARD
