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
void is_get_date_time(RTC_DateTypeDef *date, RTC_TimeTypeDef *time);

#ifdef __cplusplus
}
#endif // End of extern "C" block

#endif // __MIDDLEWARES_GWTONN_INTERNAL_SENSORS_H__