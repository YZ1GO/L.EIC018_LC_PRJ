// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "graphic.h"
#include "keyboard_macros.h"
#include "sprite.h"
#include "i8254.h"

#define min(x, y) (((x) > (y)) ? (x) : (y))
#define max(x, y) (((x) < (y)) ? (x) : (y))

extern int kb_count;
extern uint8_t scancode;
extern uint8_t bytes[2];
extern int size;
extern int size_temp;
extern bool flag_complete;
extern uint32_t count;
extern int hook_id;
extern int hook_id_kb;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g6/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g6/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  vg_init(mode);
  

  sleep(delay);

  if (vg_exit()) {
    return 1;
  }
    
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  vg_init(mode);
  vg_draw_rectangle(x, y, width, height, color);

  int ipc_status;
  message msg;
  int r;
  uint8_t bit_no;
  if (kb_subscribe_int(&bit_no)) {
    return 1;
  }
  uint32_t irq_set = BIT(bit_no);
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
  if (vg_exit()) {
    return 1;
  }
  return 0;
}

/*int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  vg_init(mode);
  uint32_t color, R, G, B;
  for(uint8_t row = 0; row < no_rectangles; ++row) {
    for(uint8_t col = 0; col < no_rectangles; ++col) {
      if(get_bytes_pixel() == 1){
        color = (first + (row * no_rectangles + col) * step) % (1 << get_bits_pixel());
      }
      else {
        R = (GET_RED(first) + col*step) % (1 << get_RedMaskSize());
        G = (GET_GRE(first) + row*step) % (1 << get_GreenMaskSize());
        B = (GET_BLU(first) + (col+row)*step) % (1 << get_BlueMaskSize());
        color = SET_COLOR(R,G,B);
      }
      uint16_t x = col * W;
      uint16_t y = row * H;
      uint16_t w = (x + W > get_XRes()) ? (get_XRes() - x) : (W);
      uint16_t h = (y + H > get_YRes()) ? (get_YRes() - y) : (H);
      if (x < get_XRes() && y < get_YRes()) {
        if (vg_draw_rectangle(x,y,w,h,color)) {
          if (vg_exit()) {
            return 1;       
          }
        }
      }
    }
  }
  int ipc_status;
  message msg;
  int r;
  uint8_t bit_no;
  if (kb_subscribe_int(&bit_no)) {
    return 1;
  }
  uint32_t irq_set = BIT(bit_no);
  while(bytes[0] != ESC_BREAK_CODE) { 
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 				
          if (msg.m_notify.interrupts & irq_set) { 
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
          break; 	
      }
    } else { 
    }
  }
  if (kb_unsubscribe_int()) {
    return 1;
  }
  if (vg_exit()) {
    return 1;
  }
  return 0;
}*/



int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  vg_init(0x105);

  sprite_t *sp = sprite_ctor(xpm);
  sprite_set_pos(sp, x, y);
  sprite_draw(sp);

  int ipc_status;
  message msg;
  int r;
  uint8_t bit_no;
  if (kb_subscribe_int(&bit_no)) {
    return 1;
  }
  uint32_t irq_set = BIT(bit_no);
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
  if (vg_exit()) {
    return 1;
  }
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  vg_init(0x105);
  sprite_t *sp = sprite_ctor(xpm);

  const uint32_t frequency = 60;
  uint32_t irq_set_timer;
  uint32_t irq_set_kb;
  uint8_t bit_no_kb;
  uint8_t bit_no_timer;
  if (timer_subscribe_int(&bit_no_timer)) {
    return 1;
  }
  if (kb_subscribe_int(&bit_no_kb)) {
    return 1;
  }
  irq_set_timer = BIT(0);
  irq_set_kb = BIT(1);

  int16_t v = (speed <= 0 ? 1 : speed);
  int16_t vx = 0, vy = 0;
  if (xi != xf) {
    vx = (xi < xf ? v : -v);
  }
  else {
    vy = (yi < yf ? v : -v);
  }

  uint32_t ticks_per_frame = frequency/(uint32_t)fr_rate;

  uint16_t x = xi, y = yi;
  sprite_set_pos(sp,x,y);
  sprite_set_vx(sp, vx);
  sprite_set_vy(sp, vy);
  sprite_draw(sp);

  int ipc_status;
  message msg;
  int r;
  int good = 1;

  while(good) { 
    /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & irq_set_timer) { /* subscribed interrupt */
            timer_int_handler();
            if (count == ticks_per_frame) {
              vg_draw_rectangle(sp->x,sp->y,sp->w,sp->h, BLACK);
              sp->x += sp->vx;
              sp->y += sp->vy;
              if (((sp->x >= xf) && (vx > 0)) || ((vx < 0) && (sp->x <= xf))) {
                sp->x = xf;
              }
              if (((sp->y >= yf) && (vy > 0)) || ((sp->y <= yf) && (vy < 0))) {
                sp->y = yf;
              }
              if (sp->x == xf && sp->y == yf) {
                good = 0;
              }
              sprite_draw(sp);
              count = 0;
            }
          }
          if (msg.m_notify.interrupts & irq_set_kb) {
            kb_interupt_handler();
            size_temp--;
            if (flag_complete) {
              size = 0;
              flag_complete = false;
              if (bytes[0] == ESC_BREAK_CODE) {
                good = 0;
              }
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
  if (vg_exit()) {
    return 1;
  }
  return 0;
}


