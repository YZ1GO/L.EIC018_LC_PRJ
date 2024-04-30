#ifndef __RTC_H__
#define __RTC_H__
#include <stdint.h>

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_IRQ 8

#define RTC_SECONDS_ADDRESS 0
#define RTC_SECONDS_ALARM_ADDRESS 1
#define RTC_MINUTES_ADDRESS 2
#define RTC_MINUTES_ALARM_ADDRESS 3
#define RTC_HOURS_ADDRESS 4
#define RTC_HOURS_ALARM_ADDRESS 5
#define RTC_DAY_OF_WEEK_ADDRESS 6
#define RTC_DAY_OF_MONTH_ADDRESS 7
#define RTC_MONTH_ADDRESS 8
#define RTC_YEAR_ADDRESS 9
#define RTC_REGISTER_A 10
#define RTC_REGISTER_B 11
#define RTC_REGISTER_C 12
#define RTC_REGISTER_D 13

#define RTC_DONT_CARE_VALUE BIT(7) | BIT(6)

#define RTC_UF BIT(4)
#define RTC_AF BIT(5)
#define RTC_PF BIT(6)

int rtc_subscribe_int(uint8_t *bit_no);

int rtc_unsubscribe_int();

int rtc_init();

int rtc_clean();

int rtc_read_register(uint8_t address, uint8_t *data);

int rtc_set_register(uint8_t address, uint8_t data);

int rtc_read_date(char *string);

int rtc_read_time(char *string);

#endif
