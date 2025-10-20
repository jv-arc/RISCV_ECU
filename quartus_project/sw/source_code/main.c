#include <stdio.h>
#include <stdint.h>


#define REG(addr) (*((volatile uint32_t*) (addr)))
#define JTAG_BASE                           0x00100010
#define PIO_OUT                             0x00200000
#define PIO_IN                              0x00200020

#define PULPINO_BASE                        0x10000000
#define SOC_PERIPHERALS_BASE              ( 0x0A100000 + PULPINO_BASE )
#define EVENT_UNIT_BASE                   ( 0X00004000 + SOC_PERIPHERALS_BASE )
#define IRP                               ( 0x00000000 + EVENT_UNIT_BASE)
#define ICP                               ( 0x0000000C + EVENT_UNIT_BASE)





void enable_irq(void){    

    // Clear enabled interruptions
    REG(ICP)     = 0xFFFFFFFF;

    // Set IRP mask for interrupt 1 (bit 2)
    REG(IRP)     = 0x00000002;

    // Set mstatus to 8
    __asm__(
        "li x6, 0x00000008\n"
        "csrs mstatus, x6"
    );

}

void __attribute__((interrupt)) jtag_interrupt_handler(void){
    REG(ICP) = (1 << 0);
}


void __attribute__((interrupt)) interrupt_test_handler(void){
    REG(ICP) = (1 << 1);
    REG(PIO_OUT)    = 0x0000000F; // Show handler worked
}


int main(int argc, char **argv){

    // Configuration on PIO for all leds
    REG(PIO_IN+8)   = 0x00003FF0;

    // Call enable_rq
    enable_irq();

    while (1);
    return 0;
}
