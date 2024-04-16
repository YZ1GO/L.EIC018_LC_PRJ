#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;

uint32_t count = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) {
    return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id)) {
    return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  count++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  //Check for NULL pointer
  if (st == NULL) {
    return 1;
  }
  //Read-Back Command 
  u32_t cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  //Make a call to retrieve the config
  if (sys_outb(TIMER_CTRL, cmd)) {
    return 1;
  }
  //Choose the port based on timer
  int port;
  switch(timer) {
    case 0: port = TIMER_0; break;
    case 1: port = TIMER_1; break;
    case 2: port = TIMER_2; break;
    default: return 1;
  }
  //Get config
  if(util_sys_inb(port, st)) {
    return 1;
  }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val conf;
  uint8_t mode;
  switch(field){
    case tsf_all: conf.byte = st; break;
    case tsf_initial:
      //0x30 = 0b00110000
      mode = (st & 0x30) >> 4; 
      switch(mode) {
        case 0: conf.in_mode = INVAL_val; break;
        case 1: conf.in_mode = LSB_only; break;
        case 2: conf.in_mode = MSB_only; break;
        case 3: conf.in_mode = MSB_after_LSB; break;
        default: return 1;
      }
    case tsf_mode:
      //0x0E = 0b00001110
      conf.count_mode = (st & 0x0E) >> 1; 
      // Check if it is mode 2 or 3
      if (conf.count_mode == 0x6 || conf.count_mode == 0x7) {
        conf.count_mode &= 0x03;
      }
      break;
    case tsf_base:
      conf.bcd = st & 0x1;
      break;
    default: return 1;
    }
    if (timer_print_config(timer, field, conf)) {
      return 1;
    }
    return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 1) {
        return 1;
    }
  uint8_t st = 0;
  if (timer_get_conf(timer, &st)) {
    return 1;
  }
  uint8_t cmd = 0;
  switch(timer) {
    case 0: cmd |= TIMER_SEL0; break;
    case 1: cmd |= TIMER_SEL1; break;
    case 2: cmd |= TIMER_SEL2; break;
    default: return 1;
  }

  cmd |= TIMER_LSB_MSB;
  cmd |= (st & (0x0E | TIMER_BCD));

  if (sys_outb(TIMER_CTRL, cmd)) {
    return 1;
  }

  uint16_t counter_init = (uint16_t)(TIMER_FREQ / freq);
  
  int port = 0;
  switch(timer) {
    case 0: port = TIMER_0; break;
    case 1: port = TIMER_1; break;
    case 2: port = TIMER_2; break;
    default: return 1;
  }
  uint8_t lsb = 0;
  uint8_t msb = 0;
  if (util_get_LSB(counter_init, &lsb)) {
    return 1;
  }
  if (util_get_MSB(counter_init, &msb)) {
    return 1;
  }
  if (sys_outb(port, lsb)) {
    return 1;
  }
  if (sys_outb(port, msb)) {
    return 1;
  }
  return 0;
}
