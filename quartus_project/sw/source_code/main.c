#include <stdio.h>

int main(int argc, char **argv){
    
    __asm__(
        "li x7, 0x00200000\n"
        "li x6, 0x0000000A\n"
        "sw x6, 0(x7)"
    );

    while (1);

    return 0;
}
