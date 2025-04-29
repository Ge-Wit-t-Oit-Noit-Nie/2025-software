#include "internal_sensors.h"
#include "main.h"
#include <stdint.h>

uint16_t adc_buffer[ADC_SAMPLES * 2 * 2] = {0};

int temperature = 0;
int vref = 0;
int get_temperature(void) {
    return temperature;
}
int get_vref(void) {
    return vref;
}

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
    vref = (((float)sum2 / 1000 / ADC_SAMPLES) * 100);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    process_adc_buffer(&adc_buffer[0]);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    process_adc_buffer(&adc_buffer[ADC_SAMPLES * 2]);
}