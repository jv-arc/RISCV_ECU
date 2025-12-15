#include <stdint.h>

typedef struct{
	volatile uint32_t SOURCE_ADDR;
	volatile uint32_t MASK;
	volatile uint32_t OFST;
	void  (*before_hook)(uint32_t*);
	void  (*after_hook)(uint32_t*);
} sensor_interface_t;

static inline uint32_t read_sensor_data(sensor_interface_t* sensor){
	sensor->before_hook((uint32_t*) sensor);
	uint32_t read_value = *((uint32_t*) sensor->SOURCE_ADDR);
	sensor->after_hook((uint32_t*) sensor);
	return ((read_value & sensor->MASK) << sensor->OFST);
}

typedef enum{
	SINGLE_OFFSET = 0,
	SINGLE_DIVISION = 1,
	DOUBLE_OFFSET = 2,
	DOUBLE_DIVISION = 3,
	QUAD_OFFSET = 4,
	QUAD_DIVISION = 5
}lut_type_t;

static inline uint32_t ofst_based(lut_type_t type){
	return ((uint32_t) type) % 2;
}

typedef struct{
	volatile lut_type_t type;
	volatile uint32_t ADDR;
	volatile uint32_t BASE;
	volatile uint32_t DIVISOR;
	volatile uint32_t OFST;
} lut_info_t;


typedef struct{
	volatile uint32_t word1;
	volatile uint32_t word2;
} double_lut_data;

typedef struct{
	volatile uint32_t word1;
	volatile uint32_t word2;
	volatile uint32_t word3;
	volatile uint32_t word4;
} quad_lut_data;


uint32_t interpolate_2_values(uint32_t, uint32_t, uint32_t, uint32_t );
uint32_t interpolate_2_values();
uint32_t interpolate_4_values();
uint32_t read_simple_lut_data(lut_info_t, uint32_t);
uint32_t read_double_lut_data(lut_info_t, uint32_t);
uint32_t read_quad_lut_data(lut_info_t, uint32_t);


