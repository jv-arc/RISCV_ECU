#define  BUFFER_SIZE 64

#include <stdint.h>
#include "custom_structs.h"
#include "debugging.h"
#include "mem_map.h"


/*
=======================================================
     _ __ ___   __ _  ___ _ __ ___  ___
    | '_ ` _ \ / _` |/ __| '__/ _ \/ __|
    | | | | | | (_| | (__| | | (_) \__ \
    |_| |_| |_|\__,_|\___|_|  \___/|___/
 
=======================================================
*/
/*
---------------PERIPHERAL ACCESS MACROS--------------------
   - Special types and casting macros are defined in
	 the header file "custom_structs.h"

	 - Memory addresses are defined in "mem_map.h" and
	 correspond to the ones defined using Plataform Designer
	 on the "sys.qsys" file

	 - Debugging macros such as DEBUG() and FLAG() with
	 respective enums are defined in the header file
	 "debugging.h" conditionally and default to comments
	 if "DEBUG_FLAG" is not set
----------------------------------------------------------
*/
#define TIMER         ( TIMER32_T (TIMER_BASE) )
#define JTAG          ( JTAG_UART_T (JTAG_BASE) )
#define PIO_OUT       ( PIO_T (PIO_OUT_BASE) )
#define PIO_IN        ( PIO_T (PIO_IN_BASE) )
#define GPIO_0        ( PIO_T (GPIO_0_BASE) )
#define GPIO_1        ( PIO_T (GPIO_1_BASE) )
#define GPIO_E        ( PIO_T (GPIO_E_BASE) )
#define INTERRUPT     ( EVENT_T (EVENT_UNIT_BASE + 0x00) )
#define EVENT         ( EVENT_T (EVENT_UNIT_BASE + 0x10) )
#define SLEEP         ( SLEEP_T (EVENT_UNIT_BASE + 0x20) )

/*
----------------GLOBAL "VARIABLES"----------------
  WARNING!: This is a bad workaround I'm only
	doing this while I'm not entirelly sure how to
	properly do this using the linkerscript file

	- VARIABLES is defined in "mem_map.h" into a
	(suposedly) safe memory space

	- REG() casts any address into a uint32_t
	
--------------------------------------------------
*/

//used to count the amount of timing interrupts
#define COUNT         ( REG (VARIABLES) )






/*
=========================================
           _   _   _ _
     _   _| |_(_) (_) |_ _   _
    | | | | __| | | | __| | | |
    | |_| | |_| | | | |_| |_| |
     \__,_|\__|_|_|_|\__|\__, |
                         |___/
=========================================
*/
void jtag_put_char(char c){
	FLAG(NORMAL,FUNC,0x00,0x00);
	while((JTAG.CONTROL >> 16) == 0){};
	FLAG(NORMAL,FUNC,0x00,0x01);
	JTAG.DATA = c;
	FLAG(NORMAL,FUNC,0x00,0x02);
}

char jtag_get_char(){
	FLAG(NORMAL,FUNC,0x01,0x00);
	while(((JTAG.DATA >> 15) & 1) == 1){};
	FLAG(NORMAL,FUNC,0x01,0x01);
  return (char)(JTAG.DATA & 0xFF);
	FLAG(NORMAL,FUNC,0x01,0x02);
}

void set_GPIO_zero_direction(uint32_t mask){
	GPIO_0.DIRECTION = mask;
	FLAG(NORMAL,SETUP,0x00,0x00);
	GPIO_0.OUT_CLEAR = mask;
	FLAG(NORMAL,SETUP,0x00,0x01);
}

void set_GPIO_one_direction(uint32_t mask){
	GPIO_1.OUT_CLEAR = mask;
	FLAG(NORMAL,SETUP,0x01,0x00);
	GPIO_1.DIRECTION = mask;
	FLAG(NORMAL,SETUP,0x01,0x01);
}

void set_GPIO_extra_direction(uint32_t mask){
	GPIO_E.OUT_CLEAR = mask;
	FLAG(NORMAL,SETUP,0x02,0x00);
	GPIO_E.DIRECTION = mask;
	FLAG(NORMAL,SETUP,0x02,0x01);
}

void set_GPIO_zero_interruptions(uint32_t mask){
	GPIO_0.EDGE_CAPTURE = mask;
	FLAG(NORMAL,SETUP,0x03,0x00);
	GPIO_0.INT_MASK = mask;
	FLAG(NORMAL,SETUP,0x03,0x01);
}

void set_GPIO_one_interruptions(uint32_t mask){
	GPIO_1.EDGE_CAPTURE = mask;
	FLAG(NORMAL,SETUP,0x04,0x00);
	GPIO_1.INT_MASK = mask;
	FLAG(NORMAL,SETUP,0x04,0x01);
}

void set_GPIO_extra_interruptions(uint32_t mask){
	GPIO_E.EDGE_CAPTURE = mask;
	FLAG(NORMAL,SETUP,0x05,0x00);
	GPIO_E.INT_MASK = mask;
	FLAG(NORMAL,SETUP,0x05,0x01);
}

void set_PIO_IN_interruptions(uint32_t mask){
	PIO_IN.EDGE_CAPTURE = 1;
	FLAG(NORMAL,SETUP,0x06,0x00);
	PIO_IN.INT_MASK = mask;
	FLAG(NORMAL,SETUP,0x06,0x01);
}



void setup_JTAG(void){
}

void setup_timer_interruption(uint32_t counting_mode, uint32_t time){
 
	// Stop counter
	TIMER.CONTROL |= (1<<3);
	FLAG(NORMAL,SETUP,0x08,0x00);

	// set time period
	uint32_t period_full = MS2CYCLES(time);
	TIMER.PERIOD_L =  (  period_full & 0xFFFF );
	TIMER.PERIOD_H =  (( period_full >> 16 ) & 0xFFFF );
	FLAG(NORMAL,SETUP,0x08,0x01);

	// Clear old timer interrupts
	TIMER.STATUS &= ~(1);
	FLAG(NORMAL,SETUP,0x08,0x02);

	// Values for configuraing the Timer
	uint32_t setup = 0;
	setup |=                     (1<<0); // (ITO) clean
	setup |= ((counting_mode & 1) << 1); // (CONT) repeating or not
	setup |=                     (1<<2); // (START) start now
	FLAG(NORMAL,SETUP,0x08,0x03);

	// Clean and write to register
	uint32_t cleaned_value = TIMER.CONTROL & (~ 5);
	TIMER.CONTROL = cleaned_value | setup;
	FLAG(NORMAL,SETUP,0x08,0x04);
}


void enable_irq(uint32_t mask){
	FLAG(NORMAL,SETUP,0x09,0x00);

	// Clear all pending interruptions
	INTERRUPT.PENDING_CLEAR = 0xFFFFFFFF;
	FLAG(NORMAL,SETUP,0x09,0x01);

	// Set mask to enabble interrupts
	INTERRUPT.ENABLE = mask;
	FLAG(NORMAL,SETUP,0x09,0x02);

	// Set mstatus to 8
	__asm__(
		"li x6, 0x00000008\n"
		"csrs mstatus, x6"
	);
	FLAG(NORMAL,SETUP,0x09,0x03);
}



/*
==================================================
     _                     _ _
    | |__   __ _ _ __   __| | | ___ _ __ ___
    | '_ \ / _` | '_ \ / _` | |/ _ \ '__/ __|
    | | | | (_| | | | | (_| | |  __/ |  \__ \
    |_| |_|\__,_|_| |_|\__,_|_|\___|_|  |___/

==================================================
*/
/*
	Interrupt handler for JTAG, cleans the JTAG interrupt signal 
	INT_NUM = 0
*/
void __attribute__((interrupt)) jtag_interrupt_handler(void){
	FLAG(NORMAL,ISR,0x00,0x00);
	INTERRUPT.PENDING_CLEAR = (1 << 0);
	FLAG(NORMAL,ISR,0x00,0x01);
}



/*
	Interrupt handler for PIO_IN
	INT_NUM = 1
*/
void __attribute__((interrupt)) board_input_handler(void){
	FLAG(NORMAL,ISR,0x01,0x00);
	INTERRUPT.PENDING_CLEAR = (1 << 1);
	FLAG(NORMAL,ISR,0x01,0x01);
}



/*
	Interrupt handler for when the timer finish it's count.
	INT_NUM = 2
*/
void __attribute__((interrupt)) timer_finished_handler(void){
	FLAG(NORMAL,ISR,0x02,0x00);
	
	// clears interrupt on the interrupt controler
	INTERRUPT.PENDING_CLEAR = (1 << 2);
	TIMER.CONTROL |= ~1;
	FLAG(NORMAL,ISR,0x02,0x01);
	
	// clears timeout bit in the timer
	TIMER.STATUS |= ~1;
	FLAG(NORMAL,ISR,0x02,0x02);

	PIO_OUT.DATA = COUNT;
  if (PIO_OUT.DATA == 10){
		FLAG(SUCCESS,ISR,0x02,0x03);
	}
	if(COUNT == 10){
		COUNT = 1;
	} else {
		COUNT ++;
	}

	FLAG(NORMAL,ISR,0x02,0x04);
}



/*
	Interrupt handler for GPIOs from bank 0
	INT_NUM = 3
*/
void __attribute__((interrupt)) gpio_zero_handler(void){
	FLAG(NORMAL,ISR,0x03,0x00);
	PIO_OUT.DATA = 3;
	
	// Cleans interrupts on the peripheral
	uint32_t interrupt = GPIO_1.DATA;
	PIO_OUT.DATA = interrupt;
	GPIO_1.EDGE_CAPTURE = interrupt;

	// Cleans interrupts on the manager
	INTERRUPT.PENDING_CLEAR = (1 << 3);
	FLAG(NORMAL,ISR,0x03,0x01);
}



/*
	Interrupt handler for for GPIOs from bank 1
	INT_NUM = 4
*/
void __attribute__((interrupt)) gpio_one_handler(void){
	FLAG(NORMAL,ISR,0x04,0x00);
	PIO_OUT.DATA = 4;

	// Cleans interrupts on the peripheral
	uint32_t interrupt = GPIO_0.DATA;
	PIO_OUT.DATA = interrupt;
	GPIO_0.EDGE_CAPTURE = interrupt;

	// Cleans interrupts on the manager
	INTERRUPT.PENDING_CLEAR = (1 << 4);
	FLAG(NORMAL,ISR,0x04,0x01);
}



/*
	Interrupt handler for extra GPIOs that won't fit in the ones before
	INT_NUM = 5
*/
void __attribute__((interrupt)) gpio_extra_handler(void){
	FLAG(NORMAL,ISR,0x05,0x00);
	PIO_OUT.DATA = 5;

	// Cleans interrupts on the peripheral
	uint32_t interrupt = GPIO_E.DATA;
	PIO_OUT.DATA = interrupt;
	GPIO_E.EDGE_CAPTURE = interrupt;

	// Cleans interrupts on the manager
	INTERRUPT.PENDING_CLEAR = (1 << 5);
	FLAG(NORMAL,ISR,0x05,0x01);
}



/*
  Fallback interrupt handler, asserts error debug and loops
	it should only trigger if an unnexpected interrupt happens

	INT_NUM = anything after 5 and before 20
*/
void __attribute__ ((interrupt)) default_exc_handler(void){
	FLAG(FAILURE,ISR,0x06,0x00);
	default_exc_handler();
}







/*
============================================
                     _
     _ __ ___   __ _(_)_ __
    | '_ ` _ \ / _` | | '_ \
    | | | | | | (_| | | | | |
    |_| |_| |_|\__,_|_|_| |_|

============================================
*/

int main(int argc, char **argv){
	COUNT = 0;
	FLAG(NORMAL,MAIN,0x00,0x00);
	enable_irq(0xFFFFFFFF);
	FLAG(NORMAL,MAIN,0x00,0x01);
	setup_timer_interruption(1, 1); // 1ms repeating
	FLAG(NORMAL,MAIN,0x00,0x02);
//	setup_IO();
	FLAG(NORMAL,MAIN,0x00,0x03);

	while (1){
		FLAG(NORMAL,WHILE,0x00,0x00);
	}
	return 0;
}
