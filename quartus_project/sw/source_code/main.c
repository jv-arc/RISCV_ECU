
#define  BUFFER_SIZE 64 //needs to be before "#include "custom_structs.h"
#include <stdint.h>
#include "io_helper.h"
#include "debugging.h"
#include "mem_map.h"


// ┌                                                         ┐
// │ Special types and casting macros are defined in         │
// │ the header file "io_helper.h"                           │
// │                                                         │
// │ Memory addresses are defined in "mem_map.h" and         │
// │ correspond to the ones defined using plataform designer │
// │ on the "sys.qsys" file                                  │
// │                                                         │
// │ Debugging macros such as DEBUG() and STATE() with       │
// │ respective enums and activations (ON_STATE ON_DEBUG)    │
// │ are defined in the header file "debugging.h"            │
// │ conditionallya and default to comments if "ENABLE_DEBUG"│
// │ macro is not defined.                                   │
// └                                                         ┘






//    ╔════════════════════╗
//    ║ ██╗    ██╗ ██████╗ ║
//    ║ ██║   ██╔╝██╔═══██╗║
//    ║ ██║  ██╔╝ ██║   ██║║
//    ║ ██║ ██╔╝  ██║   ██║║
//    ║ ██║██╔╝   ╚██████╔╝║
//    ║ ╚═╝╚═╝     ╚═════╝ ║
//    ╚════════════════════╝


// ╭───────────────────────────────────╮
// │ PLATAFORM DESIGN STD. PERIPHERALS │
// ╰───────────────────────────────────╯

#define TIMER         ( TIMER32_T    (TIMER_BASE) )
#define JTAG          ( JTAG_UART_T  (JTAG_BASE)  )
//#define PWM


//   ╭──────────────────────╮
//   │ SAFE GPIO/PIO ACCESS │
//   ╰──────────────────────╯

// ┌                                             ┐
// │ Since this is in a global scope we will not │
// │ have problems with dangling pointers.       │
// └                                             ┘


// ── GPIO ────────────────────────────────────────────────────────────

// Full Register
s_gpio_t *bank_a = &(s_gpio_t){
	.regs = (gpio_t*) BANK_A,
	.mask = 0xFFFFFFFF
};

// Full Register
s_gpio_t *bank_b = &(s_gpio_t){
	.regs = (gpio_t*) BANK_B,
	.mask = 0xFFFFFFFF
};


// 0b1111 0b1111 0b0000 0b0000 0b0000 0b0000 0b0000 0b0000
// mask: 0xFF000000
s_gpio_t *bank_c = &(s_gpio_t){
	.regs = (gpio_t*) BANK_C,
	.mask = 0xFF000000
};




// ── OTHER IO ────────────────────────────────────────────────────────


// 0b0000 0b0000 0b1111 0b0000 0b0000 0b0000 0b0000 0b0000
// mask=0x00F00000 ; offset=20
s_in_t *keys = &(s_in_t){
	.regs = (in_t*) PINS_C_R,
	.mask = 0x00F00000,
	.offset = 20
};


// 0b0000 0b0000 0b0000 0b1111 0b1111 0b1100 0b0000 0b0000
// mask=0x000FFC00 ; offset=10
s_in_t *switches = &(s_in_t){
	.regs = (in_t*) PINS_C_R ,
	.mask = 0x000FFC00,
	.offset = 10
};


// 0b1111 0b1111 0b1100 0b0000 0b0000 0b0000 0b0000 0b0000
// mask=0xFFC00000 ; offset=22
s_out_t *leds = &(s_out_t){
	.regs = (out_t*) PINS_C_0,
	.mask = 0xFFC00000,
	.offset = 22
};





//    ╭────────────────────╮
//    │ GLOBAL "VARIABLES" │
//    ╰────────────────────╯

// ┌                                                        ┐
// │ Since they are not symbols managed by the compiler     │
// │ they can't be variables.                               │
// │                                                        │
// │ I'm defining these with macros so they are easier to   │
// │ track on memory, this is a bad workaround, they should │
// │ be global variables and I should learn how to track    │
// │ them correctly by loopking up at the linkerscript.     │
// │                                                        │
// │ Not all of them are useful at the same time            │
// └                                                        ┘


#define R_BUFFER_ADDR      (BUFFER_OFFSET(VARIABLES, 0))
#define W_BUFFER_ADDR      (BUFFER_OFFSET(VARIABLES, 1))
#define COUNT_ADDR         (BUFFER_OFFSET(VARIABLES, 2))
#define TASKS_REG_ADDR     (COUNT_ADDR + 0x4)

#define R_BUFFER           (BUFFER_T (R_BUFFER_ADDR))
#define W_BUFFER           (BUFFER_T (W_BUFFER_ADDR))
#define COUNT              (REG_T    (COUNT_ADDR))
#define TASKS_REG          (REG_T    (TASKS_REG_ADDR))

// Task helpers
#define CHECK(task)        ((TASKS_REG) & (task))


//Task masks
#define ADC_TASK           (0x00000001)



//  ╭────────────╮
//  │ ADC MACROS │
//  ╰────────────╯

// ┌                                                                    ┐
// │ These are macros to access the Analog to digital converter defined │
// │ in Verilog to read data from the engine                            │
// └                                                                    ┘

#define ADC_OFFSET             (10)
#define ADC_DATA_SIZE          (8)
#define ADC_PINOUT_SIZE        (ADC_DATA_SIZE+4)

#define ADC_PINOUT_MASK        (((1<<ADC_PINOUT_SIZE)-1)<<(ADC_OFFSET))

#define ADC_BUSY_MASK          (1<<(ADC_OFFSET+0))
#define ADC_DVALID_MASK        (1<<(ADC_OFFSET+1))
#define ADC_DATA_MASK          (((1U<<ADC_DATA_SIZE)-1)<<(ADC_OFFSET+2))
#define ADC_RESET_MASK         (1<<(ADC_OFFSET+ADC_DATA_SIZE+2))
#define ADC_TRIGGER_MASK       (1<<(ADC_OFFSET+ADC_DATA_SIZE+3))






// ╔════════════════════════════════════════════════════╗
// ║ ██╗   ██╗████████╗██╗██╗     ██╗████████╗██╗   ██╗ ║
// ║ ██║   ██║╚══██╔══╝██║██║     ██║╚══██╔══╝╚██╗ ██╔╝ ║
// ║ ██║   ██║   ██║   ██║██║     ██║   ██║    ╚████╔╝  ║
// ║ ██║   ██║   ██║   ██║██║     ██║   ██║     ╚██╔╝   ║
// ║ ╚██████╔╝   ██║   ██║███████╗██║   ██║      ██║    ║
// ║  ╚═════╝    ╚═╝   ╚═╝╚══════╝╚═╝   ╚═╝      ╚═╝    ║
// ╚════════════════════════════════════════════════════╝



//  ╭────────────────╮
//  │ JTAG FUNCTIONS │
//  ╰────────────────╯

void jtag_put_char(char c){
	STATE(NORMAL,FUNC,0x00,0x00);

	// Needs to go before the loop so we avoid
	// extra writing
	STATE(NORMAL,WHILE,0x01,0x00);

	while((JTAG.CONTROL >> 16) == 0){};
	JTAG.DATA = c;

	STATE(NORMAL,FUNC,0x00,0x01);
}
char jtag_get_char(){
	STATE(NORMAL,FUNC,0x01,0x00);

	STATE(NORMAL,WHILE,0x02,0x00);
	while(((JTAG.DATA >> 15) & 1) == 1){};

	// Before to not be cut by return
	STATE(NORMAL,FUNC,0x01,0x01);
  return (char)(JTAG.DATA & 0xFF);
}



// ╭───────────────────────────────────╮
// │ QSYS COMPONENT INTERRUPTION SETUP │
// ╰───────────────────────────────────╯

void setup_timer_interruption(uint32_t counting_mode, uint32_t time){
	STATE(NORMAL,SETUP,0x0a, 0x00);

	// Stop counter
	TIMER.CONTROL |= (1U << 3);
	STATE(NORMAL,SETUP,0x0a,0x01);

	// set time period
	uint32_t period_full = MS2CYCLES(time);
	TIMER.PERIOD_L =  (  period_full & 0xFFFF );
	TIMER.PERIOD_H =  (( period_full >> 16 ) & 0xFFFF );
	STATE(NORMAL,SETUP,0x0a,0x02);

	// Clear old timer interrupts
	TIMER.STATUS &= ~(1);
	STATE(NORMAL,SETUP,0x0a,0x03);

	// Values for configuraing the Timer
	uint32_t setup = 0;
	setup |=                     (1U << 0); // (ITO) clean
	setup |= ((counting_mode & 1)    << 1); // (CONT) repeating or not
	setup |=                     (1U << 2); // (START) start now
	STATE(NORMAL,SETUP,0x0a,0x04);

	// Clean and write to register
	uint32_t cleaned_value = TIMER.CONTROL & (~ 5);
	TIMER.CONTROL = cleaned_value | setup;
	STATE(NORMAL,SETUP,0x0a,0x05);
}

void enable_irq(){
	STATE(NORMAL,SETUP,0x0b,0x00);

	//Sets machine interrupts on
	//__asm__("csrs mie, 0x800");
	
	//Sets global interruptions on
	__asm__("csrs mstatus, 0x8");

	STATE(NORMAL,SETUP,0x0b,0x03);
}






// ╔════════════════════════════════════════════════════════════════════╗
// ║ ██╗  ██╗ █████╗ ███╗   ██╗██████╗ ██╗     ███████╗██████╗ ███████╗ ║
// ║ ██║  ██║██╔══██╗████╗  ██║██╔══██╗██║     ██╔════╝██╔══██╗██╔════╝ ║
// ║ ███████║███████║██╔██╗ ██║██║  ██║██║     █████╗  ██████╔╝███████╗ ║
// ║ ██╔══██║██╔══██║██║╚██╗██║██║  ██║██║     ██╔══╝  ██╔══██╗╚════██║ ║
// ║ ██║  ██║██║  ██║██║ ╚████║██████╔╝███████╗███████╗██║  ██║███████║ ║
// ║ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝ ║
// ╚════════════════════════════════════════════════════════════════════╝


// ╭─────────────────────────╮
// │ JTAG INTERRUPTION       │
// │ INTERRUPTION_NUMBER = 0 │
// ╰─────────────────────────╯

void __attribute__((interrupt)) jtag_interrupt_handler(void){
	STATE(NORMAL,ISR,0x00,0x00);
	
	// nothing, for now

	STATE(NORMAL,ISR,0x00,0x01);
}





// ╭─────────────────────────╮
// │ TIMER INTERRUPTION      │
// │ INTERRUPTION_NUMBER = 1 │
// ╰─────────────────────────╯

void __attribute__((interrupt)) timer_finished_handler(void){
	STATE(NORMAL,ISR,0x02,0x00);
	
	// clears interrupt on the interrupt controller
	TIMER.CONTROL |= ~1;
	STATE(NORMAL,ISR,0x02,0x01);
	
	// clears timeout bit in the timer
	TIMER.STATUS |= ~1;
	STATE(NORMAL,ISR,0x02,0x02);


	if(COUNT == 10){
		io_write(leds, COUNT);
		COUNT = 1;
	} else {
		COUNT ++;
	}

	STATE(NORMAL,ISR,0x02,0x04);
}





// ╭─────────────────────────╮
// │ BANK A INTERRUPTION     │
// │ INTERRUPTION_NUMBER = 2 │
// ╰─────────────────────────╯

void __attribute__((interrupt)) pins_a_r(void){
	STATE(NORMAL,ISR,0x01,0x00);
	//Just cleans for now
	gpio_edge_clear(bank_a, 0xFFFFFFFF);
	STATE(NORMAL,ISR,0x01,0x01);
}




// ╭─────────────────────────╮
// │ BANK B INTERRUPTION     │
// │ INTERRUPTION_NUMBER = 3 │
// ╰─────────────────────────╯

void __attribute__((interrupt)) pins_b_r(void){
	uint32_t int_read;
	uint32_t condition;
	uint32_t data_read;

	STATE(NORMAL,ISR,0x03,0x00);

	int_read = gpio_edge_read(bank_b);
	STATE(NORMAL,ISR,0x03,0x01);
	DEBUG(int_read);

	condition = int_read & ADC_DVALID_MASK;
	STATE(NORMAL,ISR,0x03,0x02);
	DEBUG(condition);


	// Tests if the interruption is correct
	if(condition){
		STATE(SUCCESS,ISR,0x03,0x03);
		data_read = gpio_read(bank_b);
		data_read &= ADC_DATA_MASK;
		data_read = (data_read >> ADC_OFFSET);

		STATE(SUCCESS,ISR,0x03,0x04);
		DEBUG(data_read);
	} else {
		STATE(FAIL, ISR, 0x03, 0x5);
	}

	// Cleans interrupt
	gpio_edge_clear(bank_b, ADC_DVALID_MASK);
	STATE(NORMAL,ISR,0x03,0x06);
}




// ╭────────────────────────────────╮
// │ PINS_C_R INT                   │
// │ INTERRUPTION_NUMBER = 4        │
// │   ────────────────────────     │
// │ Shared: bank_C, keys, switches │
// ╰────────────────────────────────╯


void __attribute__((interrupt)) pins_c_r(void){
	STATE(NORMAL, ISR, 0x0, 0x0);
	uint32_t test = gpio_edge_read(bank_c);
	if(test){
		gpio_edge_clear(bank_c, test);
		//bank_c_handler(test);	
	}

	test = io_edge_read(keys);
	if(test){
		io_edge_clear(keys);
		//key_handler(test);
	}

	test = io_edge_read(switches);
	if(test){
		io_edge_clear(switches);
		//switches_handler(test);
	}

	STATE(FAIL,ISR,0x04,0x02);
}



//  ╭────────────────────────────────╮
//  │ FALLBACK INTERRUPT HANDLER     │
//  │ INTERRUPTION_NUMBER in [5, 19] │
//  ╰────────────────────────────────╯
// ┌                                                            ┐
// │ Fallback interrupt handler, asserts FLAG FAIL and loops    │
// │ it should only trigger if an unnexpected interrupt happens │
// └                                                            ┘
void __attribute__ ((interrupt)) default_exc_handler(void){
	STATE(FAIL,ISR,0x06,0x00);
	while(1){};
}






//    ╔══════════════════════════════════╗
//    ║ ███╗   ███╗ █████╗ ██╗███╗   ██╗ ║
//    ║ ████╗ ████║██╔══██╗██║████╗  ██║ ║
//    ║ ██╔████╔██║███████║██║██╔██╗ ██║ ║
//    ║ ██║╚██╔╝██║██╔══██║██║██║╚██╗██║ ║
//    ║ ██║ ╚═╝ ██║██║  ██║██║██║ ╚████║ ║
//    ║ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ║
//    ╚══════════════════════════════════╝

// ┌                                                             ┐
// │ Button in PIO_IN triggers interrupt that sets a flag,       │
// │ if flag is set,trigger_adc in event_loop starts and sets    │
// │ ADC trigger on.                                             │
// │                                                             │
// │ ADC should send a hardware signal to interrupt processor to │
// │ read interrupt data                                         │
// └                                                             ┘


void trigger_adc(){
	STATE(NORMAL, FUNC, 0x2, 0x00);

	//"quick" on-off because device expects pulse
	gpio_write(bank_a, ADC_TRIGGER_MASK);
	gpio_write(bank_a, ~ADC_TRIGGER_MASK);

	TASKS_REG = 0;

	STATE(NORMAL, FUNC, 0x2, 0x01);
}

int event_loop(){
	STATE(NORMAL,FUNC,0x0F,0x00);
	while (1){
		STATE(NORMAL,WHILE,0x00,0x00);

		if(CHECK(ADC_TASK)){
    	trigger_adc();
		}
	}
	return 1;
}

int main(int argc, char **argv){
	STATE(0x55, 0x55, 0x55, 0x55);
	TASKS_REG = 0;

	STATE(NORMAL, MAIN, 0x1, 0);
	DEBUG(ADC_PINOUT_MASK);

	STATE(NORMAL, MAIN, 0x1, 0x1);
	DEBUG(ADC_DVALID_MASK);

	STATE(NORMAL,MAIN, 0x1, 0x2);
	io_intconfig_set(keys, 0x2);
	io_edge_clear(keys);

	STATE(NORMAL,MAIN, 0x1, 0x3);
	gpio_write(bank_a, ADC_PINOUT_MASK);
	gpio_edge_clear(bank_a, ADC_PINOUT_MASK);

	STATE(NORMAL,MAIN, 0x1, 0x4);
	gpio_set_input(bank_a, ADC_DVALID_MASK);
	gpio_intconfig_set(bank_a, ADC_DVALID_MASK);

	STATE(NORMAL,MAIN, 0x1, 0x5);
	enable_irq();

	STATE(NORMAL,MAIN, 0x1, 0x6);
	return event_loop();
}

