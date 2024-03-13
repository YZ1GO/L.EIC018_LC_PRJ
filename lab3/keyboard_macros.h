#ifndef KBC_MACROS_H_INCLUDED
#define KBC_MACROS_H_INCLUDED

/* KBC IRQ Line */

#define KBC_IRQ     1   // KBC Controller IRQ Line
#define MOUSE_IRQ   12  // Mouse IRQ Line 

/* Delay for KBC */
#define DELAY           20000 // KBC Response Delay 
#define KBC_NUM_TRIES   10    // Number of tries to issue command before timeout

/* I/O Ports Addresses */

#define KBC_CMD     0x64 // Address to send commands to KBC 
#define KBC_CMD_ARG 0x60 // Address to write KBC Command Arguments 
#define STATUS_REG  0x64 // KBC Status Register address 

#define OUTPUT_BUF  0x60 // Address of Output Buffer of KBC 

/* KBC Commands */
#define READ_KBC_CMD    0x20 // Read KBC Command Byte 
#define WRITE_KBC_CMD   0x60 // Write KBC Command Byte
#define KBC_SELF_TEST   0xAA // KBC Diagnostic Tests 
#define KBC_INT_TEST    0xAB // Tests Keyboard Clock and Data lines 
#define KBC_INT_DISABLE 0xAD // Disable KBC Interface 
#define KBC_INT_ENABLE  0xAE // Enable KBC Interface 
#define MOUSE_DISABLE   0xA7 // Disable Mouse 
#define MOUSE_ENABLE    0xA8 // Enable Mouse 
#define MOUSE_INT_TEST  0xA9 // Tests Mouse data line 
#define MOUSE_WRITE_B   0xD4 // Write a byte directly to the mouse

/* Status Byte Masking */

#define OUT_BUF_FUL     BIT(0) // Output Buffer State 
#define IN_BUF_FULL     BIT(1) // Input Buffer State 
#define SYS_FLAG        BIT(2) // System Flag 
#define DATA_CMD_WRITE  BIT(3) // Identifier of type of byte in input buffer 
#define INH_FLAG        BIT(4) // Keyboard inihibited 
#define AUX_MOUSE       BIT(5) // Mouse Data 
#define TIME_OUT_REC    BIT(6) // Time Out Error - Invalid Data 
#define PARITY_ERROR    BIT(7) // Parity Error - Invalid Data 

/* Scancode Constants */

#define ESC_BREAK_CODE  0x81    // ESC Break Code
#define TWO_BYTE_CODE   0xE0    // First byte of a two byte Scancode
#define BREAK_CODE_BIT  BIT(7)  // Bit to distinguish between Make code and Break code

/* Command byte masks */
#define INT_KBD         BIT(0)  // Enable Keyboard Interrupts 
#define INT_MOU         BIT(1)  // Enable Mouse Interrupts 
#define DIS_KBD         BIT(4)  // Disable Keyboard 
#define DIS_MOU         BIT(5)  // Disable Mouse

void (kb_interupt_handler)();
int (kb_unsubscribe_int)();
int (kb_subscribe_int)(uint8_t *bit_no);
int(kb_poll)(uint8_t read[], uint8_t* size_poll);
int(kb_full)();

#endif 
