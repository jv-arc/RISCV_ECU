#include <stdint.h>


#define REG(addr)            (*((volatile uint32_t*) (addr)))



typedef struct{
    uint32_t DATA;
    uint32_t DIRECTION;
    uint32_t INT_MASK;
    uint32_t EDGE_CAPTURE;
    uint32_t OUT_SET;
    uint32_t OUT_CLEAR;
} pio_t;

#define PIO1_T(addr)   (*((volatile pio_t*) (addr)))



typedef struct{
    uint32_t DATA;
    uint32_t CONTROL;
} jtag_uart_t;

#define JTAG_UART_T(addr)   (*((volatile jtag_uart_t*) (addr)))



typedef struct {
    uint32_t STATUS;
    uint32_t CONTROL;
    uint32_t PERIOD_L;
    uint32_t PERIOD_H;
    uint32_t SNAP_L;
    uint32_t SNAP_H;
} interval_timer32_t;

#define TIMER32_T(addr)    (*((volatile interval_timer32_t*) (addr)))



typedef struct {
    uint32_t STATUS;
    uint32_t CONTROL;
    uint32_t PERIOD_0;
    uint32_t PERIOD_1;
    uint32_t PERIOD_2;
    uint32_t PERIOD_3;
    uint32_t SNAP_0;
    uint32_t SNAP_1;
} interval_timer64_t;

#define TIMER64_T(addr)    (*((volatile interval_timer64_t*) (addr)))
