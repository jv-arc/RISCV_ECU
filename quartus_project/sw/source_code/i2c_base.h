#include <stdint.h>

typedef struct{
	volatile uint32_t COMMAND;
	volatile uint32_t POINTER;
	volatile uint32_t STATUS;
	volatile uint32_t MASK;
} mailbox_generic_t;

typedef struct{
	volatile uint32_t DATA;
} mailbox_simple_t;


static inline void pending_bit_write(mailbox_generic_t* device, uint32_t value){
	uint32_t old_value = device->STATUS & ~1;
	device->STATUS = old_value | (1 & value);
}

static inline uint32_t pending_bit_read(mailbox_generic_t* device){
	return (device->STATUS & 1);
}

static inline void full_bit_write(mailbox_generic_t* device, uint32_t value){
	uint32_t old_value = device->STATUS & ~(1<<1);
	device->STATUS = old_value | ((1<<1) & value);
}

static inline uint32_t full_bit_read(mailbox_generic_t* device){
	return (device->STATUS & (1<<1));
}

static inline void pending_mask_bit_write(mailbox_generic_t* device, uint32_t value){
	uint32_t old_value = device->MASK & ~1;
	device->MASK = old_value | (1 & value);
}

static inline uint32_t pending_mask_bit_read(mailbox_generic_t* device){
	return (device->MASK & 1);
}

static inline void space_mask_bit_write(mailbox_generic_t* device, uint32_t value){
	uint32_t old_value = device->MASK & ~(1<<1);
	device->MASK = old_value | ((1<<1) & value);
}

static inline uint32_t space_mask_bit_read(mailbox_generic_t* device){
	return (device->MASK & (1<<1));
}

static inline uint32_t read_mailbox(mailbox_simple_t* device){
	return device->DATA;
}

static inline void write_mailbox(mailbox_simple_t* device, uint32_t value){
	device->DATA = value;
}




typedef struct{
	union {
		volatile uint32_t RAW;
    struct {
			volatile uint8_t a0;
			volatile uint8_t a1;
			volatile uint8_t a2;
			volatile uint8_t a3;
		} bytes;
	};
} i2c_atom_t;

typedef struct{
	volatile uint32_t SIZE;
	volatile uint32_t START;
} i2c_message_t;

typedef struct{
  volatile uint32_t HEAD;
  volatile uint32_t TAIL;
  volatile i2c_atom_t *BUFFER;
	volatile uint32_t SIZE;
} i2c_buffer_t;

typedef struct{
	const uint32_t BUFFER_SIZE;
	const uint32_t BUFFER_ADDR;
	const mailbox_generic_t* PERIPHERAL;
} i2c_config_t;


static inline i2c_atom_t read_atom(mailbox_generic_t* device){
	i2c_atom_t data;
	
	data.bytes.a0 = (device->COMMAND & 0xFF);
	data.bytes.a1 = (device->COMMAND & 0xFF);
	data.bytes.a2 = (device->COMMAND & 0xFF);
	data.bytes.a3 = (device->COMMAND & 0xFF);

	return data;
}


// i2c_config_t config is a global variable
static inline void push_atom_2_buffer(i2c_buffer_t* buffer, i2c_atom_t atom){
	uint32_t next_tail = buffer->TAIL + 1 % buffer->SIZE;


	uint32_t index = (uint32_t)buffer->BUFFER + buffer->TAIL;
	buffer->TAIL = next_tail;

	*((i2c_atom_t*) index) = atom;
}

static inline i2c_buffer_t* malloc_i2c_buffer(i2c_config_t config){
	i2c_buffer_t* newBuffer = malloc(sizeof(i2c_buffer_t));

	newBuffer->BUFFER = malloc(config.BUFFER_SIZE*sizeof(i2c_atom_t));
	newBuffer->HEAD = 0;
	newBuffer->TAIL = 0;

	return newBuffer;
}


// Warning this does not check if memory addresses are being used
static inline i2c_buffer_t* mm_fixed_i2c_buffer(i2c_config_t config){
	uint32_t base_address = config.BUFFER_ADDR;
	uint32_t buffer_address = base_address + sizeof(i2c_buffer_t);

	i2c_atom_t* buffer_array = (i2c_atom_t*) buffer_address;
	i2c_buffer_t* access_struct = (i2c_buffer_t*) base_address;

	access_struct->BUFFER = buffer_array;
	access_struct->HEAD=0;
	access_struct->TAIL=0;

	return access_struct;
}

// I don't know why I created these enums
typedef enum{
	IDLE    = 0x00 , // I2C not running, CPU is doing other things waiting for interruption
	READING = 0x01 , // getting atoms from mailbox
	WORKING = 0x02   // placeholder for future commands
} i2c_slave_state_t;

//why????
typedef enum {
	MODE_DEBUG    = 0xF0,
	MODE_REGULAR  = 0x0F,
	SEND_POSITION = 0x00,
	ADC_READ      = 0x01
} i2c_commands_t;



static inline i2c_message_t i2c_mailbox2buffer(mailbox_generic_t* slave, i2c_buffer_t* buffer){
	uint32_t start = buffer->TAIL;
	uint32_t length = 0;
	
	while(pending_bit_read(slave)){
		i2c_atom_t data = read_atom(slave);
		push_atom_2_buffer(buffer, data);
		length ++;
	}

	i2c_message_t new_message;
	new_message.SIZE = length;
	new_message.START = start;

	return new_message;
}


void __attribute__((interrupt)) i2c_isr_handler(void){
 //DRAFT

}
