/*
*============================================
*  HEADER TO DEFINE MEMORY MAPPED ADDRESSES
*============================================
*/

// ╭─────────╮
// │ HELPERS │
// ╰─────────╯

#define offset(B, S, X) ((B)+((S)*(X)))
#define gpios         (0x00300000)

#define byte          (4)
#define word          (4*byte)
#define pin           (4*word)
#define gpio          (3*pin)



// ╭──────────────╮
// │ QSYS DEFINED │
// ╰──────────────╯

#define JTAG_BASE         (0x00100010)
#define TIMER_BASE        (0x002000A0)


#define BANK_A            (offset(gpios, gpio, 0))
#define PINS_A_R          (offset(BANK_A,pin,0))
#define PINS_A_W          (offset(BANK_A,pin,1))
#define PINS_A_S          (offset(BANK_A,pin,2))

#define BANK_B            (offset(gpios, gpio, 1))
#define PINS_B_R          (offset(BANK_B,pin,0))
#define PINS_B_W          (offset(BANK_B,pin,1))
#define PINS_B_S          (offset(BANK_B,pin,2))

#define BANK_C            (offset(gpios, gpio, 2))
#define PINS_C_R          (offset(BANK_C,pin,0))
#define PINS_C_0          (offset(BANK_C,pin,1))
#define PINS_C_1          (offset(BANK_C,pin,2))





// ╭───────╮
// │ OTHER │
// ╰───────╯

#define VARIABLES               (0x02000000)

