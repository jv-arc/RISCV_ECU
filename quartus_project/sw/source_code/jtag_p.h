#include <stdint.h>

// ╔════════════════════════════════════╗
// ║      ██╗████████╗ █████╗  ██████╗  ║
// ║      ██║╚══██╔══╝██╔══██╗██╔════╝  ║
// ║      ██║   ██║   ███████║██║  ███╗ ║
// ║ ██   ██║   ██║   ██╔══██║██║   ██║ ║
// ║ ╚█████╔╝   ██║   ██║  ██║╚██████╔╝ ║
// ║  ╚════╝    ╚═╝   ╚═╝  ╚═╝ ╚═════╝  ║
// ╚════════════════════════════════════╝



#ifndef JTAG_H
#define JTAG_H



	// ╭──────╮
	// │ JTAG │
	// ╰──────╯


	// ── BASE STRUCT ─────────────────────────────────────────────────────

	typedef struct{
		volatile uint32_t DATA;
		volatile uint32_t CONTROL;
	} jtag_uart_t;




	// ── ACCESS FUNCTIONS ────────────────────────────────────────────────


	static inline char get_jtag_data(jtag_uart_t* device){
		return (device->DATA & 0xFF);
	}

	static inline void put_jtag_data(jtag_uart_t* device, char char_in){
		// Everything other than what we are trying to write is read only
		device->DATA = char_in;
	}

	static inline uint32_t get_jtag_rvalid(jtag_uart_t* device){
		return ((device->DATA >> 15) & 1);
	}

	static inline uint32_t get_jtag_ravail(jtag_uart_t* device){
		return (device->DATA >> 16);
	}

	static inline uint32_t get_jtag_re(jtag_uart_t* device){
		return (device->CONTROL & 0b1);
	}

	static inline void set_jtag_re_on(jtag_uart_t* device){
		device->CONTROL |= 0b1;
	}

	static inline void set_jtag_re_off(jtag_uart_t* device){
		device->CONTROL &= ~0b1;
	}

	static inline uint32_t get_jtag_we(jtag_uart_t* device){
		return (device->CONTROL & 0b10);
	}

	static inline void set_jtag_we_on(jtag_uart_t* device){
		device->CONTROL |= 0b10;
	}

	static inline void set_jtag_we_off(jtag_uart_t* device){
		device->CONTROL &= ~0b10;
	}

	static inline uint32_t get_jtag_wspace(jtag_uart_t* device){
		return (device->CONTROL >> 16);
	}

	static inline uint32_t get_jtag_ri(jtag_uart_t* device){
		return (device->CONTROL & (1<<8));
	}

	static inline uint32_t get_jtag_wi(jtag_uart_t* device){
		return (device->CONTROL & (1<<9));
	}

	static inline uint32_t get_jtag_ac(jtag_uart_t* device){
		return (device->CONTROL & (1<<10));
	}








  // ╭──────────────────╮
  // │ CHARACTER BUFFER │
  // ╰──────────────────╯

  // ┌                                                  ┐
  // │ Basically to avoid writing blocking code for the │
  // │ JTAG peripheral, we just write and read from the │
  // │ buffer and we let JTAG work based on it's ISR.   │
  // └                                                  ┘


	//Allows defining a different buffer_size on the code
	#ifndef BUFFER_SIZE
		#define BUFFER_SIZE 128
	#endif



	// ── BASE STRUCT ─────────────────────────────────────────────────────
	typedef struct{
		volatile uint8_t  DATA[BUFFER_SIZE]; //not struct size!
		volatile uint32_t HEAD;
		volatile uint32_t TAIL;
	} buffer_t;




	// ── ACCESS FUNCTIONS ────────────────────────────────────────────────
	static inline uint32_t write_char_to_buffer(buffer_t* buffer, char c){
		uint32_t next_tail = (buffer->TAIL +1) % BUFFER_SIZE;
		if(next_tail == buffer->HEAD){
			return ~0;
		}
		
		buffer->DATA[buffer->TAIL] = c;
		buffer->TAIL = next_tail;
		return 0;
	}

	//uint32_t to return something that isn't a char in case of error
	static inline uint32_t read_char_from_buffer(buffer_t* buffer){
		if(buffer->HEAD == buffer->TAIL){
			return ~0;
		}

		uint32_t char_read = buffer->DATA[buffer->HEAD];
		buffer->HEAD = (buffer->HEAD +1) % BUFFER_SIZE;
		return (char_read & 0xFF);
	}

	static inline buffer_t* buffer_init(uint32_t address){
		buffer_t* newBuffer = ((buffer_t*) (address));
		newBuffer->HEAD = 0;
		newBuffer->TAIL = 0;
		return newBuffer;
	}






#endif
