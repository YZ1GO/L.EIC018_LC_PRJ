//headers for lcf
#include <lcom/lcf.h>
#include <lcom/proj.h>

//headers for game engine
#include "game.h"
#include "xpm_templates.h"

//headers for devices
#include "graphic.h"
#include "mouse.h"
#include "keyboard.h"
#include "i8254.h"
#include "utils.h"
#include "list.h"

//extern variables
extern int kb_count;
extern uint8_t scancode;
extern uint8_t bytes[2];
extern int size;
extern int size_temp;
extern bool flag_complete_kb;
extern uint32_t count;
extern int hook_id;
extern int hook_id_kb;

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/g6/proj/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/g6/proj/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
    return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
    vg_init(0x105);
    sprite_t *sp = sprite_ctor(NINE_xpm);
    int x = 100;
    int y = 100;
    sprite_set_pos(sp, x, y);
    sprite_draw(sp);

    int ipc_status;
    message msg;
    int r;
    uint8_t bit_no_kbd;
    if (kb_subscribe_int(&bit_no_kbd)) {
        return 1;
    }
    uint32_t irq_set = BIT(1);

    while(bytes[0] != ESC_BREAK_CODE) { 
        /* You may want to use a different condition */
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */				
                    if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                        kb_interupt_handler();
                        size_temp--;
                        if (flag_complete_kb) {
                            size = 0;
                            flag_complete_kb = false;
                            handleMoviment(scancode, sp);
                        }
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */	
            }
        } else { 
            /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    if (kb_unsubscribe_int()) {
        return 1;
    }
    if (vg_exit()) {
        return 1;
    }
    return 0;
}
