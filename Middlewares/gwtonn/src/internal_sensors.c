/**
 ******************************************************************************
 * @file   internal_sensors.c
 * @brief  Implementation of the internal sensors
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

#include "internal_sensors.h"
#include "main.h"
#include <stdint.h>

uint16_t adc_buffer[ADC_SAMPLES * 2 * 2] = {0};

int temperature = 0;
int vref = 0;
void process_adc_buffer(uint16_t *buffer);

/***************************************
 * Public functions
 **************************************/

/*
 * The temperature is constantly updated using the HAL_ADC_ConvCpltCallback function. 
 * The value is stored in the global variable temperature.
 * 
 * This function returns just returns the value of the temperature variable.
 */
int is_get_temperature(void)
{
    return temperature;
}
/*
 * The vref is constantly updated using the HAL_ADC_ConvCpltCallback function.
 * The value is stored in the global variable vref.
 *
 * This function returns just returns the value of the vref variable.
 */
int is_get_vref(void)
{
    return vref;
}
/*
 * Return the value of the RTC date and time.
 */
void is_get_date_time(RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
    HAL_RTC_GetDate(&hrtc, date, RTC_FORMAT_BIN);
    HAL_RTC_GetTime(&hrtc, time, RTC_FORMAT_BIN);
}
/*
 * Set the RTC time to the given values.
 */
void is_set_time(uint8_t hr, uint8_t min, uint8_t sec)
{
    RTC_TimeTypeDef sTime = {0};
    sTime.Hours = hr;
    sTime.Minutes = min;
    sTime.Seconds = sec;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
}

/***************************************
 * Callback functions
 **************************************/
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    UNUSED(hadc);
    process_adc_buffer(&adc_buffer[0]);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    UNUSED(hadc);
    process_adc_buffer(&adc_buffer[ADC_SAMPLES * 2]);
}

/***************************************
 * Internal functions
 **************************************/
// Process half a buffer full of data
void process_adc_buffer(uint16_t *buffer)
{

    uint32_t sum1 = 0, sum2 = 0;
    for (int i = 0; i < ADC_SAMPLES; ++i)
    {
        sum1 += buffer[i * 2];
        sum2 += buffer[1 + i * 2];
    }

    temperature = (((float)sum1 / ADC_SAMPLES - 279) * 100);
    vref = (((float)sum2 / 1000 / ADC_SAMPLES) * 1000);
}
