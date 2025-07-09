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
 * @brief  AVG Slope for temperature calculation
 * 
 * This value is used to calculate the temperature from the ADC value.
 * For the STM32F4xx series, the average slope is approximately 2.5 mV/°C.
 */
#define AVG_SLOPE (2.5F)
/**
 * @brief Voltage at 25 °C
 * 
 * This value is used to calculate the temperature from the ADC value.
 * For the STM32F4xx series, the voltage at 25 °C is approximately 0.76 V.
 */
#define V_AT_25C  (0.76F)
/**
 * @brief Internal reference voltage
 * 
 * This value is used to calculate the internal voltage from the ADC value.
 * For the STM32F4xx series, the internal reference voltage is approximately 1.2 V.
 */
#define V_REF_INT (1.2F)

/**
 * @brief  Get the temperature value of the STM32F4xx microcontroller
 * 
 * @retval Temperature value in Celsius
 */
float is_get_temperature(void);
/**
 * @brief  Get the internal voltage of the STM32F4xx microcontroller
 *
 * @retval Internal voltage value in millivolts
 */
float is_get_vref(void);
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
/**
 * @brief  Set the RTC date
 *
 * @param  year: Year value
 * @param  month: Month value (1-12)
 * @param  day: Day value (1-31)
 */
void is_set_date(uint8_t year, uint8_t month, uint8_t day);

#ifdef __cplusplus
}
#endif // End of extern "C" block

#endif // __MIDDLEWARES_GWTONN_INTERNAL_SENSORS_H__