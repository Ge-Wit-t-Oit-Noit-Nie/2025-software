/**
 ******************************************************************************
 * @file   internal_sensors.h
 * @brief  Headerfile for the internal_sensors.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 Ge Wit't Oit Noit Nie.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#ifndef __MIDDLEWARES_GWTONN_INTERNAL_SENSORS_H__
#define __MIDDLEWARES_GWTONN_INTERNAL_SENSORS_H__

#include "rtc.h"

#ifdef __cplusplus
extern "C" {
#endif // End of extern "C" block

/**
 * @brief  Get the temperature value of the STM32F4xx microcontroller
 * 
 * @retval Temperature value in Celsius
 */
int is_get_temperature(void);
/**
 * @brief  Get the internal voltage of the STM32F4xx microcontroller
 *
 * @retval Internal voltage value in millivolts
 */
int is_get_vref(void);
/**
 * @brief  Get the date and time from the RTC
 *
 * @param  date: Pointer to a RTC_DateTypeDef structure that will hold the date
 * @param  time: Pointer to a RTC_TimeTypeDef structure that will hold the time
 */
void is_get_date_time(RTC_DateTypeDef *date, RTC_TimeTypeDef *time);
/**
 * @brief  Set the RTC time
 *
 * @param  hr: Hour value (0-23)
 * @param  min: Minute value (0-59)
 * @param  sec: Second value (0-59)
 */
void is_set_time(uint8_t hr, uint8_t min, uint8_t sec);

#ifdef __cplusplus
}
#endif // End of extern "C" block

#endif // __MIDDLEWARES_GWTONN_INTERNAL_SENSORS_H__