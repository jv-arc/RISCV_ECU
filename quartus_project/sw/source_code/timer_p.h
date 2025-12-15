#include <stdint.h>


// ╔═════════════════════════════════════════════════╗
// ║ ████████╗██╗███╗   ███╗███████╗██████╗ ███████╗ ║
// ║ ╚══██╔══╝██║████╗ ████║██╔════╝██╔══██╗██╔════╝ ║
// ║    ██║   ██║██╔████╔██║█████╗  ██████╔╝███████╗ ║
// ║    ██║   ██║██║╚██╔╝██║██╔══╝  ██╔══██╗╚════██║ ║
// ║    ██║   ██║██║ ╚═╝ ██║███████╗██║  ██║███████║ ║
// ║    ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝ ║
// ╚═════════════════════════════════════════════════╝




#ifndef TIMER_H
#define TIMER_H

	//Allows redefining for debugging
	#ifndef TIME_FACTOR_MS_TO_CYCLES
		#define TIME_FACTOR_MS_TO_CYCLES (25000)
	#endif

	//Converts an amount in milliseconds to time constant for timers
	#define MS_TIMER(n)   (((n)*(TIME_FACTOR_MS_TO_CYCLES))-1)



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



	static inline uint32_t check_running_32timer(timer32_t* device){
		uint32_t reading = device->STATUS;
		reading &= 2;
		return (reading >> 1);
	}

	static inline uint32_t check_timeout_32timer(timer32_t* device){
		uint32_t reading = device->STATUS;
		return (reading & 1);
	}

	static inline void clear_timeout_32timer(timer32_t* device){
		//Writing to the rest of the register has no effect
		device->STATUS = 1;
	}

	static inline void int_on_32timer(timer32_t* device){
		uint32_t saved_control = (device->CONTROL & (~(1<<0)));
		device->CONTROL = (saved_control | (1<<0));
	}

	static inline void int_off_32timer(timer32_t* device){
		uint32_t saved_control = (device->CONTROL & (~(1<<0)));
		device->CONTROL = saved_control; // writes 0 at bit 0
	}

	static inline void single_run_mode_32timer(timer32_t* device){
		uint32_t saved_control = (device->CONTROL & (~(1<<1)));
		device->CONTROL = saved_control; // writes 0 at bit 1
	}

	static inline void multi_run_mode_32timer(timer32_t* device){
		uint32_t saved_control = (device->CONTROL & (~(1<<1)));
		device->CONTROL = (saved_control | (1<<1));
	}

	static inline uint32_t which_mode_32timer(timer32_t* device){
		return ((device->CONTROL & (1<<1)) >> 1);
	}

	//the bit 2 of the register can only start counting, writing 0 does nothing
	static inline void start_32timer(timer32_t* device){
		uint32_t saved_control = (device->CONTROL & (~(1<<2)));
		device->CONTROL = (saved_control | (1<<2));
	}

	//the bit 3 of the register can only stop counting, writing 0 does nothing
	static inline void stop_32timer(timer32_t* device){
		uint32_t saved_control = (device->CONTROL & (~(1<<3)));
		device->CONTROL = (saved_control | (1<<3));
	}

	static inline void write_period_32timer(timer32_t* device, uint32_t value){
		device->PERIOD_L = (value & 0x0000FFFF);
		device->PERIOD_H = (value & 0xFFFF0000);
	}

	static inline uint32_t get_snapshot_32timer(timer32_t* device){
		//any write will request snapshot
		device->SNAP_L = 1;

		// after request snapshot is loaded into registers
		uint32_t high_reading = device->SNAP_H;
		uint32_t low_reading = device->SNAP_L;
		return ((high_reading & 0xFFFF0000) | (low_reading & 0x0000FFFF));
	}





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
		volatile uint32_t SNAP_2;
		volatile uint32_t SNAP_3;
	} timer64_t;

	typedef struct {
		volatile uint32_t TIME_63_32;
		volatile uint32_t TIME_31_0;
	} period_64_t;


	static inline uint32_t check_running_64timer(timer64_t* device){
		return check_running_32timer((timer32_t*) device);
	}

	static inline uint32_t check_timeout_64timer(timer64_t* device){
		return check_timeout_32timer((timer32_t*) device);
	}

	static inline void clear_timeout_64timer(timer64_t* device){
		clear_timeout_32timer((timer32_t*) device);
	}

	static inline void int_on_64timer(timer64_t* device){
		int_on_32timer((timer32_t*) device);
	}

	static inline void int_off_64timer(timer64_t* device){
		int_off_32timer((timer32_t*) device);
	}

	static inline void single_run_mode_64timer(timer64_t* device){
		single_run_mode_32timer((timer32_t*) device);
	}

	static inline void multi_run_mode_64timer(timer64_t* device){
		multi_run_mode_32timer((timer32_t*) device);
	}

	static inline uint32_t which_mode_64timer(timer64_t* device){
		return which_mode_32timer((timer32_t*) device);
	}

	static inline void start_64timer(timer64_t* device){
		start_32timer((timer32_t*) device);
	}

	static inline void stop_64timer(timer64_t* device){
		stop_32timer((timer32_t*) device);
	}

	static inline void write_period_64timer(timer64_t* device, period_64_t period){
		device->PERIOD_0 = (period.TIME_31_0 & 0x0000FFFF);
		device->PERIOD_1 = (period.TIME_31_0 & 0xFFFF0000);
		device->PERIOD_2 = (period.TIME_63_32 & 0x0000FFFF);
		device->PERIOD_3 = (period.TIME_63_32 & 0xFFFF0000);
	}

	static inline period_64_t get_snapshot_64timer(timer64_t* device){
		device->SNAP_0 = 1;
		
		uint32_t reading_0 = device->SNAP_0;
		uint32_t reading_1 = device->SNAP_1;
		uint32_t reading_2 = device->SNAP_2;
		uint32_t reading_3 = device->SNAP_3;

		period_64_t result;

		result.TIME_63_32 = ((reading_3 & 0xFFFF0000) | (reading_2 & 0x0000FFFF));
		result.TIME_31_0 = ((reading_1 & 0xFFFF0000) | (reading_0 & 0x0000FFFF));
		return result;
	}



#endif
