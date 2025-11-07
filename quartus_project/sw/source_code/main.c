#include "acess_structs.h"
#include "debbuging.h"
#include "mem_map.h"
#include <stdint.h>


// Memory Addresses used
#define ICP           ( REG (ICP_ADDR) )
#define IRP           ( REG (IRP_ADDR) )
#define TIMER         ( TIMER32_T (TIMER_BASE) )
#define JTAG          ( JTAG_UART_T (JTAG_BASE) )
#define PIO_OUT       ( PIO_T (PIO_OUT_BASE) )
#define PIO_IN        ( PIO_T (PIO_IN_BASE) )
#define GPIO_0        ( PIO_T (GPIO_0_BASE) )
#define GPIO_1        ( PIO_T (GPIO_1_BASE) )
#define GPIO_E        ( PIO_T (GPIO_E_BASE) )

// memory region for counting "variable"
#define COUNT         ( REG (VARIABLES) )

/*
 ==== Debugging Convention ====
If DEBUG_FLAG is defined the DEBUG() macro
will be defined in the "debbuging.h" header

There are enum definition and conventions
are in the debbugging file
 ==============================
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

//======Functions for PIO Control============
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

// Functions for General Setup

void setup_IO(void){

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


void enable_irq(void){
	FLAG(NORMAL,SETUP,0x09,0x00);


	// Clear enabled interruptions
	ICP = 0xFFFFFFFF;
	FLAG(NORMAL,SETUP,0x09,0x01);


	// Set IRP mask for interrupt 2
	IRP = (1<< 2);
	FLAG(NORMAL,SETUP,0x09,0x02);


	// Set mstatus to 8
	__asm__(
		"li x6, 0x00000008\n"
		"csrs mstatus, x6"
	);
	FLAG(NORMAL,SETUP,0x09,0x03);
}




/*
	Interrupt handler for JTAG, cleans the JTAG interrupt signal 
	INT_NUM = 0 
*/
void __attribute__((interrupt)) jtag_interrupt_handler(void){
	FLAG(NORMAL,ISR,0x00,0x00);
	ICP = (1 << 0);
	FLAG(NORMAL,ISR,0x00,0x01);
}

/*
	Interrupt handler for PIO_IN
	INT_NUM = 0
*/
void __attribute__((interrupt)) board_input_handler(void){
	FLAG(NORMAL,ISR,0x01,0x00);
	ICP = (1 << 1);
	FLAG(NORMAL,ISR,0x01,0x01);
}

/*
	Debbuging LEDs format : 0x20-
	(Leading 2 and 3 turns LEDR[9] on, so it's easy to see in waveform)
*/
void __attribute__((interrupt)) timer_finished_handler(void){
	FLAG(NORMAL,ISR,0x02,0x00);
	
	// clears interrupt on the interrupt controler
	ICP = (1 << 2);
	TIMER.CONTROL |= ~1;
	FLAG(NORMAL,ISR,0x02,0x01);
	
	// clears timeout bit in the timer
	TIMER.STATUS |= ~1;
	FLAG(NORMAL,ISR,0x02,0x02);

	PIO_OUT.DATA = COUNT;
	
	if(COUNT==10){
		COUNT = 0;
	} else {
		COUNT ++;

		FLAG(SUCCESS,ISR,0x02,0x03);
	}

	FLAG(NORMAL,ISR,0x02,0x04);
}


/*
	Interrupt handler for JTAG, cleans the JTAG interrupt signal 
	INT_NUM = 3
*/
void __attribute__((interrupt)) gpio_zero_handler(void){
	FLAG(NORMAL,ISR,0x03,0x00);
	PIO_OUT.DATA = 3;
	uint32_t interrupt = GPIO_1.DATA;
	PIO_OUT.DATA = interrupt;
	GPIO_1.EDGE_CAPTURE = interrupt;
	ICP = (1 << 3);
	FLAG(NORMAL,ISR,0x03,0x01);
}


/*
	Interrupt handler for JTAG, cleans the JTAG interrupt signal 
	INT_NUM = 4
*/
void __attribute__((interrupt)) gpio_one_handler(void){
	FLAG(NORMAL,ISR,0x04,0x00);
	PIO_OUT.DATA = 4;
	uint32_t interrupt = GPIO_0.DATA;
	PIO_OUT.DATA = interrupt;
	GPIO_0.EDGE_CAPTURE = interrupt;
	ICP = (1 << 4);
	FLAG(NORMAL,ISR,0x04,0x01);
}


/*
	Interrupt handler for JTAG, cleans the JTAG interrupt signal 
	INT_NUM = 5
*/
void __attribute__((interrupt)) gpio_extra_handler(void){
	FLAG(NORMAL,ISR,0x05,0x00);
	PIO_OUT.DATA = 5;
	uint32_t interrupt = GPIO_E.DATA;
	PIO_OUT.DATA = interrupt;
	GPIO_E.EDGE_CAPTURE = interrupt;
	ICP = (1 << 5);
	FLAG(NORMAL,ISR,0x05,0x01);
}

void __attribute__ ((interrupt)) default_exc_handler(void){
	FLAG(FAILURE,ISR,0x06,0x00);
	default_exc_handler();
}

int main(int argc, char **argv){
	// Setup process
	COUNT = 0;
	FLAG(NORMAL,MAIN,0x00,0x00);

	enable_irq();
	FLAG(NORMAL,MAIN,0x00,0x01);
	setup_timer_interruption(1, 1); // 1ms repeating
	FLAG(NORMAL,MAIN,0x00,0x02);
	setup_IO();
	FLAG(NORMAL,MAIN,0x00,0x03);

	while (1){
		FLAG(NORMAL,WHILE,0x00,0x00);
	}
	return 0;
}
