/*
    ============================================
      HEADER TO DEFINE MEMORY MAPPED ADDRESSES
    ============================================
*/


// Qsys Defined
#define JTAG_BASE               0x00100010
#define PIO_OUT_BASE            0x00200000
#define PIO_IN_BASE             0x00200020
#define GPIO_0_BASE             0x00200040
#define GPIO_1_BASE             0x00200060
#define GPIO_E_BASE             0x00200060
#define TIMER_BASE              0x002000A0


// Defined by me :3
#define VARIABLES               0x02000000


// Pulpino fixed
#define PULPINO_BASE            0x10000000
#define SOC_PERIPHERALS_BASE  ( 0x0A100000 + PULPINO_BASE )
#define EVENT_UNIT_BASE       ( 0X00004000 + SOC_PERIPHERALS_BASE )
