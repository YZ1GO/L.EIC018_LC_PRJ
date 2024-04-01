// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "mouse.h"
#include "state.h"

extern int hook_id_mouse;
extern uint8_t byte;
extern uint8_t bytes[3];
extern uint8_t count_mouse;
extern bool flag_complete;
extern int count;
extern int hook_id;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/g6/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g6/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  message msg;
  uint8_t bit_no;
  uint32_t irq_set;
  int r;
  uint32_t number_packets = 0;
  struct packet mouse_packet;
  if (send_command(0xF4)) {
    return 1;
  }
  if (mouse_subscribe_int(&bit_no)) {
    return 1;
  }
  irq_set = BIT(bit_no);
  while(number_packets < cnt) { 
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
            mouse_ih();
            if (flag_complete) {
              build_packet(&mouse_packet);
              mouse_print_packet(&mouse_packet);
              number_packets++;
              flag_complete = false;
              count_mouse = 0;
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
  if (mouse_unsubscribe_int()) {
    return 1;
  }
  if (send_command(0xF5)) {
    return 1;
  }
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t time_elapsed = 0;
  uint32_t irq_set_timer;
  int ipc_status;
  message msg;
  uint8_t bit_no_mouse;
  uint8_t bit_no_timer;
  uint32_t irq_set;
  int r;
  uint32_t number_packets = 0;
  struct packet mouse_packet;
  if (send_command(0xF4)) {
    return 1;
  }
  /*if (send_command(0xF4)) {
    return 1;
  }*/
  if (mouse_subscribe_int(&bit_no_mouse)) {
    return 1;
  }
  if (timer_subscribe_int(&bit_no_timer)) {
    return 1;
  }
  irq_set = BIT(bit_no_mouse);
  irq_set_timer = BIT(bit_no_timer);
  while(time_elapsed < idle_time) { 
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
            mouse_ih();
            count = 0;
            if (flag_complete) {
              build_packet(&mouse_packet);
              mouse_print_packet(&mouse_packet);
              number_packets++;
              flag_complete = false;
              count_mouse = 0;
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
  if (timer_unsubscribe_int()) {
    return 1;
  }
  if (mouse_unsubscribe_int()) {
    return 1;
  }
  if (send_command(0xF5)) {
    return 1;
  }
  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  struct mouse_ev *event;
  int response = 0;
  int ipc_status;
  message msg;
  uint8_t bit_no;
  uint32_t irq_set;
  int r;
  struct packet mouse_packet;
  if (send_command(0xF4)) {
    return 1;
  }
  if (mouse_subscribe_int(&bit_no)) {
    return 1;
  }
  irq_set = BIT(bit_no);
  while(response != 1) { 
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
            mouse_ih();
            if (flag_complete) {
              build_packet(&mouse_packet);
              mouse_print_packet(&mouse_packet);
              event = mouse_get_event(&mouse_packet);
              response = state_machine(event, x_len, tolerance);
              flag_complete = false;
              count_mouse = 0;
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
  if (mouse_unsubscribe_int()) {
    return 1;
  }
  if (send_command(0xF5)) {
    return 1;
  }
  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
