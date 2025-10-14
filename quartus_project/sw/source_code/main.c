#include <stdio.h>
#include <string.h>
#include <stdint.h>

volatile int *gpio_addr = (int*)0x100000;
volatile int *jtag_uart_data = (int*)0x100010;
volatile int *jtag_uart_ctrl = (int*)0x100014;



void print_jtag(void){
    const char msg[] = "Hello Kitty :3\n";
    int i;

    for (i=0; i<strlen(msg); i++) {
        while ((*jtag_uart_ctrl >> 16) == 0) ;
        
        *jtag_uart_data = msg[i];
    }
}



int main(int argc, char **argv){
    const char msg[] = "Ta rodando!\n";
    
    int i;

    for (i=0; i<strlen(msg); i++) {
        while ((*jtag_uart_ctrl >> 16) == 0) ;
        
        *jtag_uart_data = msg[i];
    }

    while (1){
        print_jtag();
        wait(1, 1);
    }

    return 0;
}
