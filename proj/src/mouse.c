#include "mouse.h"
#include "i8254.h"
#include "keyboard.h"
#include <stdint.h>

int hook_id_mouse = 12;
uint8_t byte;
uint8_t bytes[3];
uint8_t count_mouse = 0;
bool flag_complete = false;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  hook_id_mouse = 12;
  *bit_no = hook_id_mouse;
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &hook_id_mouse)) {
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_mouse)) {
    return 1;
  }
  return 0;
}

int (mouse_read_status)(uint8_t* status){
  if (util_sys_inb(STATUS_REG, status)) {
    return 1;
  }
  return 0;
}

int(mouse_read_byte)(uint8_t status, uint8_t* byte) {
  int try = 10;
  int try_count = 0;
  if (status & BIT(5)) { //mouse bit
    while(try_count < try) {
      if (util_sys_inb(OUTPUT_BUF, byte)) {
        return 1;
      }
      else {
        return 0;
      }
      tickdelay(micros_to_ticks(DELAY));
      try_count++;
    }
  }
  return 1;
}

void (mouse_ih)() {
  uint8_t status;
  mouse_read_status(&status);
  mouse_read_byte(status, &byte);
  if (count_mouse == 0 && (byte & BIT(3))) {
    bytes[count_mouse] = byte;
    count_mouse++;
  }
  else if (count_mouse != 0) {
    bytes[count_mouse] = byte;
    count_mouse++;
  }
  if (count_mouse >= 3) {
    flag_complete = true;
  }
}

void (build_packet)(struct packet* pp) {
  for (int i = 0; i < 3; i++) {
    pp->bytes[i] = bytes[i];
  }
  pp->lb = bytes[0] & LEFT_BUTTON;
  pp->mb = bytes[0] & MIDDLE_BUTTON;
  pp->rb = bytes[0] & RIGHT_BUTTON;
  pp->x_ov = bytes[0] & X_OVERFLOW;
  pp->y_ov = bytes[0] & Y_OVERFLOW;
  //X Delta
  pp->delta_x = bytes[1];
  if (bytes[0] & MSB_X_DELTA) {
    pp->delta_x |= 0xFF00; 
  }
  //Y Delta
  pp->delta_y = bytes[2];
  if (bytes[0] & MSB_Y_DELTA) {
    pp->delta_y |= 0xFF00;
  }
}

int (send_command)(uint8_t command) {
  uint8_t response = 0;
  uint8_t status = 0;
  if (sys_outb(0x64, 0xD4)) {
    return 1;
  }
  do {
    mouse_read_status(&status);
    if (status & BIT(1)) {
      continue;
    }
    else {
      if (sys_outb(0x60, command)) {
        return 1;
      }
      if (util_sys_inb(0x60, &response)) {
        return 1;
      }
    }
  } while (response != 0xFA);
  return 0;
}
