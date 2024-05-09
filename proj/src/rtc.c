#include "rtc.h"
#include <lcom/lcf.h>

int hook_id = 8;
int rtc_subscribe_int(uint8_t *bit_no) {
  hook_id = RTC_IRQ;
  *bit_no = (uint8_t) hook_id;
  if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_id) != OK) {
    return 1;
  }
  return 0;
}

int rtc_unsubscribe_int() {
  if (sys_irqrmpolicy(&hook_id) != OK) {
    return 1;
  }
  return 0;
}

int rtc_read_register(uint8_t address, uint8_t *data) {
  if (sys_outb(RTC_ADDR_REG, address)) {
    return 1;
  }
  if (util_sys_inb(RTC_DATA_REG, data)) {
    return 1;
  }
  return 0;
}

int rtc_set_register(uint8_t address, uint8_t data) {
  if (sys_outb(RTC_ADDR_REG, address)) {
    return 1;
  }
  if (sys_outb(RTC_DATA_REG, data)) {
    return 1;
  }
  return 0;
}

int rtc_read_time(char *string) {
  uint8_t data;
  uint8_t hour, minute;
  //uint8_t second;
  do {
    rtc_read_register(RTC_REGISTER_A, &data);
  } while (data & BIT(7));
  /*if (rtc_read_register(RTC_SECONDS_ADDRESS, &second)) {
    return 1;
  }*/
  if (rtc_read_register(RTC_MINUTES_ADDRESS, &minute)) {
    return 1;
  }
  if (rtc_read_register(RTC_HOURS_ADDRESS, &hour)) {
    return 1;
  }
  sprintf(string, "%02x:%02x", hour, minute);
  return 0;
}
