#include "lut.h"


uint32_t read_simple_lut_data(lut_info_t lut, uint32_t read_value){
	uint32_t index = read_value - lut.BASE;
	if(ofst_based(lut.type)){
		index = index << lut.OFST;
	} else{
		index = index / lut.DIVISOR;
	}
	return (*(uint32_t*)(lut.ADDR + index));
}

uint32_t read_lut_interpolated_double_data(lut_info_t lut){
	
}
