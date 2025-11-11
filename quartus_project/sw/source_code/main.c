

#define  BUFFER_SIZE 64 //needs to be before "#include "custom_structs.h"
#include <stdint.h>
#include "custom_structs.h"
#include "debugging.h"
#include "mem_map.h"


/*
*====================================================
*     _ __ ___   __ _  ___ _ __ ___  ___
*    | '_ ` _ \ / _` |/ __| '__/ _ \/ __|
*    | | | | | | (_| | (__| | | (_) \__ \
*    |_| |_| |_|\__,_|\___|_|  \___/|___/
*
*=====================================================
*/


/*
*---------------peripheral access macros--------------------
*   - special types and casting macros are defined in
*  the header file "custom_structs.h"
*
*   - memory addresses are defined in "mem_map.h" and
*  correspond to the ones defined using plataform designer
*  on the "sys.qsys" file
*
*   - debugging macros such as debug() and flag() with
*  respective enums are defined in the header file
*  "debugging.h" conditionally and default to comments
*  if "debug_flag" macro is not defined.
*----------------------------------------------------------
*/

#define TIMER         ( TIMER32_T    (TIMER_BASE) )
#define JTAG          ( JTAG_UART_T  (JTAG_BASE) )
#define PIO_OUT       ( PIO_T        (PIO_OUT_BASE) )
#define PIO_IN        ( PIO_T        (PIO_IN_BASE) )
#define GPIO_0        ( PIO_T        (GPIO_0_BASE) )
#define GPIO_1        ( PIO_T        (GPIO_1_BASE) )
#define GPIO_E        ( PIO_T        (GPIO_E_BASE) )

/*
----------------GLOBAL "VARIABLES"----------------
*  WARNING!: This is a bad workaround I'm only
*  doing this while I'm not entirely sure how to
*  properly manipulate memory space in the linkerscript
*  file without breaking anything
*
*  VARIABLES is defined in "mem_map.h" into a
*  (supposedly) safe memory space
*-------------------------------------------------
*/

#define R_BUFFER_ADDR ( BUFFER_OFFSET (VARIABLES , 0) )
#define W_BUFFER_ADDR ( BUFFER_OFFSET (VARIABLES , 1) )
#define COUNT_ADDR    ( BUFFER_OFFSET (VARIABLES , 2) )

#define R_BUFFER      ( BUFFER_T (R_BUFFER_ADDR) )
#define W_BUFFER      ( BUFFER_T (W_BUFFER_ADDR) )
#define COUNT         ( REG_T    (COUNT_ADDR) )




/*
*=========================================
*           _   _   _ _
*     _   _| |_(_) (_) |_ _   _
*    | | | | __| | | | __| | | |
*    | |_| | |_| | | | |_| |_| |
*     \__,_|\__|_|_|_|\__|\__, |
*                         |___/
*=========================================
*/


/*
* ==========JTAG BASIC FUNCTIONS===========
*/
void jtag_put_char(char c){
	FLAG(NORMAL,FUNC,0x00,0x00);

	// Needs to go before the loop so we avoid
	// extra writing
	FLAG(NORMAL,WHILE,0x01,0x00);
	while((JTAG.CONTROL >> 16) == 0){};
	JTAG.DATA = c;

	FLAG(NORMAL,FUNC,0x00,0x01);
}
char jtag_get_char(){
	FLAG(NORMAL,FUNC,0x01,0x00);

	FLAG(NORMAL,WHILE,0x02,0x00);
	while(((JTAG.DATA >> 15) & 1) == 1){};

	// Before to not be cut by return
	FLAG(NORMAL,FUNC,0x01,0x01);
  return (char)(JTAG.DATA & 0xFF);
}



/*
* ==========SETTING GPIO AS INPUTS===========
*/
void set_gpio_zero_as_input(uint32_t mask){
	FLAG(NORMAL,SETUP,0x00,0x00);

	// inputs are '0'
	GPIO_0.DIRECTION &= ~mask;

	FLAG(NORMAL,SETUP,0x00,0x01);
}
void set_gpio_one_as_input(uint32_t mask){
	FLAG(NORMAL,SETUP,0x01,0x00);

	// inputs are '0'
	GPIO_1.DIRECTION &= ~mask;

	FLAG(NORMAL,SETUP,0x01,0x01);
}
void set_gpio_extra_as_input(uint32_t mask){
	FLAG(NORMAL,SETUP,0x02,0x00);

	// inputs are '0'
	GPIO_E.DIRECTION &= ~mask;

	FLAG(NORMAL,SETUP,0x02,0x01);
}



/*
* ==========SETTING GPIO AS OUTPUTS===========
*/
void set_gpio_zero_as_output(uint32_t mask){
	FLAG(NORMAL,SETUP,0x03,0x00);

	// It's necessary to clean the gpio before
	//otherwise it can be stuck at high.
	GPIO_0.OUT_CLEAR = mask;

	// inputs are '1'
	GPIO_0.DIRECTION |= mask;

	FLAG(NORMAL,SETUP,0x03,0x01);
}
void set_gpio_one_as_output(uint32_t mask){
	FLAG(NORMAL,SETUP,0x04,0x00);

	// It's necessary to clean the gpio before
	//otherwise it can be stuck at high.
	GPIO_1.OUT_CLEAR = mask;

	// inputs are '1'
	GPIO_1.DIRECTION |= mask;
	FLAG(NORMAL,SETUP,0x04,0x01);
}
void set_gpio_extra_as_output(uint32_t mask){
	FLAG(NORMAL,SETUP,0x05,0x00);

	// It's necessary to clean the gpio before
	//otherwise it can be stuck at high.
	GPIO_E.OUT_CLEAR = mask;

	// inputs are '1'
	GPIO_E.DIRECTION |= mask;
	FLAG(NORMAL,SETUP,0x05,0x01);
}



/*
* ==========SETTING GPIO INTERRUPTIONS===========
*/
void set_gpio_zero_interruptions(uint32_t mask){
	FLAG(NORMAL,SETUP,0x06, 0x00);

	// Cleans pending interrupts before enabling them
	// otherwise we might get interruptions that were
	// triggered begore enabling them
	GPIO_0.EDGE_CAPTURE = mask;
	GPIO_0.INT_MASK = mask;

	FLAG(NORMAL,SETUP,0x06,0x01);
}
void set_gpio_one_interruptions(uint32_t mask){
	FLAG(NORMAL,SETUP,0x07, 0x00);

	// Cleans pending interrupts before enabling them
	// otherwise we might get interruptions that were
	// triggered begore enabling them
	GPIO_1.EDGE_CAPTURE = mask;
	GPIO_1.INT_MASK = mask;

	FLAG(NORMAL,SETUP,0x07,0x01);
}
void set_gpio_extra_interruptions(uint32_t mask){
	FLAG(NORMAL,SETUP,0x07, 0x00);

	// Cleans pending interrupts before enabling them
	// otherwise we might get interruptions that were
	// triggered begore enabling them
	GPIO_E.EDGE_CAPTURE = mask;
	GPIO_E.INT_MASK = mask;

	FLAG(NORMAL,SETUP,0x07,0x01);
}



/*
* ==========SETTING OTHER INTERRUPTIONS===========
*/
void set_pio_in_interruptions(uint32_t mask){
	FLAG(NORMAL,SETUP,0x08, 0x00);

	PIO_IN.EDGE_CAPTURE = 1;
	PIO_IN.INT_MASK = mask;

	FLAG(NORMAL,SETUP,0x08,0x01);
}
void setup_timer_interruption(uint32_t counting_mode, uint32_t time){
	FLAG(NORMAL,SETUP,0x09, 0x00);

	// Stop counter
	TIMER.CONTROL |= (1U << 3);
	FLAG(NORMAL,SETUP,0x09,0x01);

	// set time period
	uint32_t period_full = MS2CYCLES(time);
	TIMER.PERIOD_L =  (  period_full & 0xFFFF );
	TIMER.PERIOD_H =  (( period_full >> 16 ) & 0xFFFF );
	FLAG(NORMAL,SETUP,0x09,0x02);

	// Clear old timer interrupts
	TIMER.STATUS &= ~(1);
	FLAG(NORMAL,SETUP,0x09,0x03);

	// Values for configuraing the Timer
	uint32_t setup = 0;
	setup |=                     (1U << 0); // (ITO) clean
	setup |= ((counting_mode & 1)    << 1); // (CONT) repeating or not
	setup |=                     (1U << 2); // (START) start now
	FLAG(NORMAL,SETUP,0x09,0x04);

	// Clean and write to register
	uint32_t cleaned_value = TIMER.CONTROL & (~ 5);
	TIMER.CONTROL = cleaned_value | setup;
	FLAG(NORMAL,SETUP,0x09,0x05);
}
void enable_irq(){
	FLAG(NORMAL,SETUP,0x0a,0x00);

	//Sets machine interrupts on
	//__asm__("csrs mie, 0x800");
	
	//Sets global interruptions on
	__asm__("csrs mstatus, 0x8");

	FLAG(NORMAL,SETUP,0x0a,0x03);
}



/*
*==================================================
*     _                     _ _
*    | |__   __ _ _ __   __| | | ___ _ __ ___
*    | '_ \ / _` | '_ \ / _` | |/ _ \ '__/ __|
*    | | | | (_| | | | | (_| | |  __/ |  \__ \
*    |_| |_|\__,_|_| |_|\__,_|_|\___|_|  |___/
*
*==================================================
*/

/*
* Interrupt handler for JTAG, cleans the JTAG interrupt signal 
* INT_NUM = 0
*/
void __attribute__((interrupt)) jtag_interrupt_handler(void){
	FLAG(NORMAL,ISR,0x00,0x00);


	FLAG(NORMAL,ISR,0x00,0x01);
}



/*
* Interrupt handler for PIO_IN
* INT_NUM = 1
*/
void __attribute__((interrupt)) board_input_handler(void){
	FLAG(NORMAL,ISR,0x01,0x00);


	FLAG(NORMAL,ISR,0x01,0x01);
}



/*
* Interrupt handler for when the timer finish it's count.
* INT_NUM = 2
*/
void __attribute__((interrupt)) timer_finished_handler(void){
	FLAG(NORMAL,ISR,0x02,0x00);
	
	// clears interrupt on the interrupt controller
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
* Interrupt handler for GPIOs from bank 0
* INT_NUM = 3
*/
void __attribute__((interrupt)) gpio_zero_handler(void){
	FLAG(NORMAL,ISR,0x03,0x00);

	// Debug GPIO_0.DATA
	uint32_t read_data = GPIO_0.DATA;
	FLAG(NORMAL,ISR,0x03,0x01);
	DEBUG(read_data);


	// Confirms GPIO_0.EDGE_CAPTURE
	uint32_t interrupt_in_peripheral = GPIO_0.EDGE_CAPTURE;
	FLAG(NORMAL,ISR,0x03,0x02);
	DEBUG(interrupt_in_peripheral);


	// Cleans interrupt
	GPIO_0.EDGE_CAPTURE = interrupt_in_peripheral;


	FLAG(NORMAL,ISR,0x03,0x04);
}



/*
* Interrupt handler for for GPIOs from bank 1
* INT_NUM = 4
*/
void __attribute__((interrupt)) gpio_one_handler(void){
	FLAG(NORMAL,ISR,0x04,0x00);

	// Debug GPIO_0.DATA
	uint32_t read_data = GPIO_1.DATA;
	FLAG(NORMAL,ISR,0x04,0x01);
	DEBUG(read_data);


	// Confirms GPIO_0.EDGE_CAPTURE
	uint32_t interrupt_in_peripheral = GPIO_1.EDGE_CAPTURE;
	FLAG(NORMAL,ISR,0x04,0x02);
	DEBUG(interrupt_in_peripheral);


	// Cleans interrupt
	GPIO_1.EDGE_CAPTURE = interrupt_in_peripheral;


	FLAG(NORMAL,ISR,0x04,0x04);
}



/*
* Interrupt handler for extra GPIOs that won't fit in the ones before
* INT_NUM = 5
*/
void __attribute__((interrupt)) gpio_extra_handler(void){
	FLAG(NORMAL,ISR,0x05,0x00);

	// Debug GPIO_0.DATA
	uint32_t read_data = GPIO_E.DATA;
	FLAG(NORMAL,ISR,0x05,0x01);
	DEBUG(read_data);


	// Confirms GPIO_0.EDGE_CAPTURE
	uint32_t interrupt_in_peripheral = GPIO_E.EDGE_CAPTURE;
	FLAG(NORMAL,ISR,0x05,0x02);
	DEBUG(interrupt_in_peripheral);


	// Cleans interrupt
	GPIO_E.EDGE_CAPTURE = interrupt_in_peripheral;

	FLAG(NORMAL,ISR,0x05,0x04);
}



/*
*  Fallback interrupt handler, asserts FLAG FAILURE and loops
*  it should only trigger if an unnexpected interrupt happens
*
*  INT_NUM = anything after 5 and before 20
*/
void __attribute__ ((interrupt)) default_exc_handler(void){
	FLAG(FAILURE,ISR,0x06,0x00);
	while(1){};
}







/*
*============================================
*                     _
*     _ __ ___   __ _(_)_ __
*    | '_ ` _ \ / _` | | '_ \
*    | | | | | | (_| | | | | |
*    |_| |_| |_|\__,_|_|_| |_|
*
*============================================
*/

int main(int argc, char **argv){
	DEBUG(0x55555555);

	// Set GPIO_0 as input
	set_gpio_zero_as_input(0xFFFFFFFF);
	set_gpio_one_as_input(0xFFFFFFFF);
	set_gpio_extra_as_input(0xFFFFFFFF);
	FLAG(NORMAL,MAIN,0x00,0x01);
	
	// Enable GPIO_0 interruptions
	set_gpio_zero_interruptions(0xFFFFFFFF);
	set_gpio_one_interruptions(0xFFFFFFFF);
	set_gpio_extra_interruptions(0xFFFFFFFF);
	FLAG(NORMAL,MAIN,0x00,0x02);

	enable_irq();
	FLAG(NORMAL,MAIN,0x00,0x03);

	while (1){
		//Needs to be inside to reassert after ISR
		FLAG(NORMAL,WHILE,0x00,0x00);
	}
	return 0;
}
