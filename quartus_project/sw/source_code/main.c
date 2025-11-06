#include "acess_structs.h"
#include "debbuging.h"
#include "mem_map.h"
#include <stdint.h>


// Memory Addresses used
#define ICP           ( REG (ICP_ADDR) )
#define IRP           ( REG (IRP_ADDR) )
#define TIMER         ( TIMER32_T (TIMER_BASE) )
#define JTAG          ( JTAG_UART_T (JTAG_BASE) )
#define PIO_OUT       ( PIO1_T (PIO_OUT_BASE) )
#define PIO_IN        ( PIO1_T (PIO_IN_BASE) )

// memory region for counting "variable"
#define COUNT         ( REG (VARIABLES) )

/* 
  ==== Debugging Convention ====
If DEBUG_FLAG is defined the DEBUG() macro
will be defined in the "debbuging.h" header

I'm using the follow convention:
 
 * byte 0 - Program State:
   00 for normal function
	 F0 for success
	 FF for error

 * byte 1 - type of section
	 00 for regular
	 01 for handler
	
 * byte 2 - section identifier
 * byte 3 - step count
 
  ==============================
*/


void setup_timer_interruption(void){
	DEBUG(0x00000000);

	// Stop counter
	TIMER.CONTROL |= (1<<3);
	DEBUG(0x00000001);

	// set time period
	uint32_t period_full = MS2CYCLES(1);
	TIMER.PERIOD_L =  (  period_full & 0xFFFF );
	TIMER.PERIOD_H =  (( period_full >> 16 ) & 0xFFFF );
	DEBUG(0x00000002);

	// Clear old timer interrupts
	TIMER.STATUS &= ~(1);
	DEBUG(0x00000003);

	// Activate counting in repeating mode
	// (START = 1 ; CONT = 1 ; ITO =1) => 5
	uint32_t cleaned_value = TIMER.CONTROL & (~ 5);
	TIMER.CONTROL = cleaned_value | 5;
	DEBUG(0x00000004);
}


/* 
  Function to enable interruptions, all, in the interrupt peripheral:
	- uses the bit 3 in the CONRTOL register (offset 0x04)
	- writes time to 16 bit regions PERIODL (0x8) and PERIODH (0xC)
	- cleans first bit of CONTROL register to clean interrupts in the peripheral
	- Activates counting (START=1), in the single shot mode (CONT=0) and (ITO=1)
	
	Debugging LED format: 0x0A-
*/
void enable_irq(void){

	DEBUG(0x00000200);


	// Clear enabled interruptions
	ICP = 0xFFFFFFFF;
	DEBUG(0x00000201);


	// Set IRP mask for interrupt 2
	IRP = (1<< 2);
	DEBUG(0x00000202);


	// Set mstatus to 8
	__asm__(
		"li x6, 0x00000008\n"
		"csrs mstatus, x6"
	);
	DEBUG(0x00000203);
}




/*
	Interrupt handler for JTAG, cleans the JTAG interrupt signal 
	INT_NUM = 0 
*/
void __attribute__((interrupt)) jtag_interrupt_handler(void){
	// Clean the interruprt
	ICP = (1 << 0);
  DEBUG(0x00010000);
	/*
	Do Something here...
	*/
}

/*
	Interrupt handler for PIO_IN
	INT_NUM = 0 
*/
void __attribute__((interrupt)) board_input_handler(void){
	// Clean the interruprt
	ICP = (1 << 1);
  DEBUG(0x00010100);
}

/*
	Debbuging LEDs format : 0x20-
	(Leading 2 and 3 turns LEDR[9] on, so it's easy to see in waveform)
*/
void __attribute__((interrupt)) timer_finished_handler(void){
  DEBUG(0x00010200);
	
	// clears interrupt on the interrupt controler
	ICP = (1 << 2);
	TIMER.CONTROL |= ~1;
  DEBUG(0x00010201);
	
	// clears timeout bit in the timer
	TIMER.STATUS |= ~1;
  DEBUG(0x00010202);

	PIO_OUT.DATA = COUNT;
	
	if(COUNT==10){
		COUNT = 0;
	} else {
		COUNT ++;
	}
  DEBUG(0x00010203);
}


/*
	Interrupt handler for JTAG, cleans the JTAG interrupt signal 
	INT_NUM = 3
*/
void __attribute__((interrupt)) gpio_zero_handler(void){
	// Clean the interruprt
	ICP = (1 << 3);
  DEBUG(0x00010300);
	/*
	Do Something here...
	*/
}


/*
	Interrupt handler for JTAG, cleans the JTAG interrupt signal 
	INT_NUM = 4
*/
void __attribute__((interrupt)) gpio_one_handler(void){
	// Clean the interruprt
	ICP = (1 << 4);
  DEBUG(0x00010400);
	/*
	Do Something here...
	*/
}


/*
	Interrupt handler for JTAG, cleans the JTAG interrupt signal 
	INT_NUM = 5
*/
void __attribute__((interrupt)) gpio_extra_handler(void){
	// Clean the interruprt
	ICP = (1 << 5);
  DEBUG(0x00010500);
	/*
	Do Something here...
	*/
}

void __attribute__ ((interrupt)) default_exc_handler(void){
	DEBUG(0xFF010000);
	default_exc_handler();
}

int main(int argc, char **argv){
	// Setup process
	COUNT = 0;
	DEBUG(0x00000300);

	enable_irq();
	DEBUG(0x00000301);
	setup_timer_interruption();
	
	while (1){
		DEBUG(0x00000400);
	}
	return 0;
}
