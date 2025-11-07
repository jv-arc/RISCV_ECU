#define DEBUG_FLAG

// No-reloading guard
#ifndef DEBUG_H
#define DEBUG_H

	#ifdef DEBUG_FLAG

		#define DEBUG_BASE_ADDR         0x00300000
		#define DEBUG(n)                ((*((volatile uint32_t*) DEBUG_BASE_ADDR)) = (n))
		#define timer_conversion_factor 10
	
	#else

		#define DEBUG_BASE_ADDR         0x00000000
		#define DEBUG(n)                /* n */
		#define timer_conversion_factor 25000

	#endif

	// Independent of DEBUG_FLAG
	#define MS2CYCLES(n)              (((n)*(timer_conversion_factor))-1)

	typedef enum {
		NORMAL = 0x00,
		SUCCESS = 0xF0,
		FAILURE = 0xFF
	} STATUS_T;

	typedef enum {
		MAIN = 0x00,
		WHILE = 0x01,
		FUNC = 0x02,
		SETUP = 0x03,
		ISR = 0x04
	} TYPE_T;

	#define FLAG(a,b,c,d)               DEBUG(((a)<<24)+((b)<<16)+((c)<<8)+((d)<<0))


#endif // DEBUG_H

