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
