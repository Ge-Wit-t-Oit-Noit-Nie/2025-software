#ifndef __INTERNAL_SENSORS_H__
#define __INTERNAL_SENSORS_H__

#include "rtc.h"

int get_temperature(void);
int get_vref(void);
void get_date_time(RTC_DateTypeDef *date, RTC_TimeTypeDef *time);

#endif // __INTERNAL_SENSORS_H__