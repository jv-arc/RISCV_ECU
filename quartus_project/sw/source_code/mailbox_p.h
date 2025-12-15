#include <stdint.h>

// ╔════════════════════════════════╗
// ║ ███╗   ███╗ █████╗ ██╗██╗      ║
// ║ ████╗ ████║██╔══██╗██║██║      ║
// ║ ██╔████╔██║███████║██║██║      ║
// ║ ██║╚██╔╝██║██╔══██║██║██║      ║
// ║ ██║ ╚═╝ ██║██║  ██║██║███████╗ ║
// ║ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚══════╝ ║
// ╚════════════════════════════════╝




#ifndef MAILBOX_H
#define MAILBOX_H


	// ── BASE STRUCT ─────────────────────────────────────────────────────

	typedef struct{
		volatile uint32_t COMMAND;
		volatile uint32_t POINTER;
		volatile uint32_t STATUS;
		volatile uint32_t MASK;
	} mailbox_t;





	// ── ACCESS FUNCTIONS ────────────────────────────────────────────────

	static inline void pending_bit_write(mailbox_t* device, uint32_t value){
		uint32_t old_value = device->STATUS & ~1;
		device->STATUS = old_value | (1 & value);
	}

	static inline uint32_t pending_bit_read(mailbox_t* device){
		return (device->STATUS & 1);
	}

	static inline void full_bit_write(mailbox_t* device, uint32_t value){
		uint32_t old_value = device->STATUS & ~(1<<1);
		device->STATUS = old_value | ((1<<1) & value);
	}

	static inline uint32_t full_bit_read(mailbox_t* device){
		return (device->STATUS & (1<<1));
	}

	static inline void pending_mask_bit_write(mailbox_t* device, uint32_t value){
		uint32_t old_value = device->MASK & ~1;
		device->MASK = old_value | (1 & value);
	}

	static inline uint32_t pending_mask_bit_read(mailbox_t* device){
		return (device->MASK & 1);
	}

	static inline void space_mask_bit_write(mailbox_t* device, uint32_t value){
		uint32_t old_value = device->MASK & ~(1<<1);
		device->MASK = old_value | ((1<<1) & value);
	}

	static inline uint32_t space_mask_bit_read(mailbox_t* device){
		return (device->MASK & (1<<1));
	}




#endif
