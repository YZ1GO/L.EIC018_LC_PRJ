#include <lcom/lcf.h>
#include "keyboard_macros.h"

//5.1
int hook_id_kb = 1;
uint8_t scancode;
uint8_t bytes[2];
int size = 0;
int size_temp = 0;
bool flag_complete = false;

int (kb_subscribe_int)(uint8_t *bit_no) {
  hook_id_kb = 1;
  *bit_no = BIT(hook_id_kb);
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &hook_id_kb)) {
    return 1;
  }
  return 0;
}

int (kb_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_kb)) {
    return 1;
  }
  return 0;
}

int(kb_read_status)(uint8_t *status) {
  if (util_sys_inb(STATUS_REG, status)) {
    return 1;
  }
  return 0;
}

int(kb_full)(){
  uint8_t status;
  if(kb_read_status(&status)) {
    return -1;
  }
  return (status & OUT_BUF_FUL);
}

int(kb_read_scancode)(uint8_t status, uint8_t* scancode) {
  int try = 10;
  int try_count = 0;
  if (status & 0x01) { //output is full
    while(try_count < try) {
      if (util_sys_inb(OUTPUT_BUF, scancode)) {
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

void(kb_interupt_handler)() {
  uint8_t status;
  kb_read_status(&status);
  kb_read_scancode(status, &scancode);
  if (scancode == 0xE0) {
    size_temp = 2;
    size = 2;
    bytes[0] = scancode;
  }
  else {
    size_temp = 1;
    if (size == 2) {
      bytes[1] = scancode;
      flag_complete = true;
    }
    else {
      size = 1;
      bytes[0] = scancode;
      flag_complete = true;
    }
  }
}
