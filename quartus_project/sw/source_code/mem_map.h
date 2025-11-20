/*
*============================================
*  HEADER TO DEFINE MEMORY MAPPED ADDRESSES
*============================================
*/

// ╭─────────╮
// │ HELPERS │
// ╰─────────╯

#define gpio_offset(B,SA,SB)    ((B)+(((3*(SB))+(SA)) * 0x10))





// ╭──────────────╮
// │ QSYS DEFINED │
// ╰──────────────╯

#define JTAG_BASE         (0x00100010)
#define TIMER_BASE        (0x002000A0)

#define gpio_base         (0x00300000)

#define GPIO_A_R          (gpio_offset(gpio_base, 0, 0))
#define GPIO_A_W          (gpio_offset(gpio_base, 0, 1))
#define GPIO_A_S          (gpio_offset(gpio_base, 0, 2))

#define GPIO_B_R          (gpio_offset(gpio_base, 1, 0))
#define GPIO_B_W          (gpio_offset(gpio_base, 1, 1))
#define GPIO_B_S          (gpio_offset(gpio_base, 1, 2))

#define GPIO_C_R          (gpio_offset(gpio_base, 2, 0))
#define GPIO_C_0          (gpio_offset(gpio_base, 2, 1))
#define GPIO_C_1          (gpio_offset(gpio_base, 2, 2))
#define GPIO_C_2          (gpio_offset(gpio_base, 2, 3))





// ╭───────╮
// │ OTHER │
// ╰───────╯

#define VARIABLES               (0x02000000)

