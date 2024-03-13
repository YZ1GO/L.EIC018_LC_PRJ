#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "keyboard_macros.h"
#include "i8254.h"

extern int kb_count;
extern uint8_t scancode;
extern uint8_t bytes[2];
extern int size;
extern int size_temp;
extern bool flag_complete;
extern int count;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status;
  message msg;
  uint8_t irq_set = 1;
  int r;
  if (kb_subscribe_int(&irq_set)) {
    return 1;
  }
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
            if (size_temp == 0) {
              kbd_print_scancode(!(bytes[size-1] & BREAK_CODE_BIT), size, bytes);
            }
            if (flag_complete) {
              size = 0;
              flag_complete = false;
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
  if (kbd_print_no_sysinb(kb_count)) {
    return 1;
  };
  return 0;
}

int(kbd_test_poll)() {
  do {
    while(!kb_full()) {
      tickdelay(micros_to_ticks(DELAY));
    }
    kb_interupt_handler();
    size_temp--;
    if (size_temp == 0) {
      kbd_print_scancode(!(bytes[size-1] & BREAK_CODE_BIT), size, bytes);
    }
    if (flag_complete) {
      size = 0;
    }
  } while (bytes[0] != ESC_BREAK_CODE);
  
  if (sys_outb(0x64, 0x20)) {
    return 1;
  }
  uint8_t cmd = 0;
  if (util_sys_inb(0x60, &cmd)) {
    return 1;
  }
  if (sys_outb(0x64, 0x60)) {
    return 1;
  }
  cmd |= 0x01;
  if (sys_outb(0x60, cmd)) {
    return 1;
  }
  kbd_print_no_sysinb(kb_count);
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t irq_set_timer = 0;
  uint8_t irq_set_kb = 1;
  uint8_t time_elapsed = 0;
  int ipc_status;
  message msg;
  int r;
  if (kb_subscribe_int(&irq_set_kb)) {
    return 1;
  }
  if (timer_subscribe_int(&irq_set_timer)) {
    return 1;
  }
  while(bytes[0] != ESC_BREAK_CODE && time_elapsed < n) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 			
          if (msg.m_notify.interrupts & irq_set_kb) { 
            count = 0;
            kb_interupt_handler();
            size_temp--;
            if (size_temp == 0) {
              kbd_print_scancode(!(bytes[size-1] & BREAK_CODE_BIT), size, bytes);
            }
            if (flag_complete) {
              size = 0;
              flag_complete = false;
            }
          }
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            if (count % 60 == 0) {
              timer_print_elapsed_time();
              time_elapsed++;
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
  if (timer_unsubscribe_int()) {
    return 1;
  }
  if (kbd_print_no_sysinb(kb_count)) {
    return 1;
  }
  return 0;
}
