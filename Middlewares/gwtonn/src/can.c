/**
 ******************************************************************************
 * @file   can.c
 * @brief  Implementation of the CAN interface
 ******************************************************************************
 *
 * The CAN Interface is used to communicate with other devices.
 * The CAN interface is implemented as an UART interface, however, for the
 * purporse of clarity, we call it CAN.
 *
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

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "usart.h"

#include "can.h"
#include "datetime.h"
#include "internal_sensors.h"

/* USER CODE BEGIN Header_can_thread_handler */
#define RX_BUFFER_SIZE 128

const uint8_t termination_sequence[3] = {0xFF, 0xFF, 0xFF};

volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t head = 0;
volatile uint16_t tail = 0;
uint8_t rx_byte;

/**
 * @brief  FREERTOS thread handler for the CAN interface.
 *
 * @param argument: Not used.
 */
void can_thread_handler(void *argument) {

    UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable'
                      // warning
    RTC_DateTypeDef gDate;
    RTC_TimeTypeDef gTime;
    datetime_t datetime = {0};
    dt_dense_time *encoded;

    // Start the UART receive process
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);

    for (;;) {
        // Delay for 100 ms
        osDelay(pdMS_TO_TICKS(1000));
        /*
        is_get_date_time(&gDate, &gTime); // get the time from the RTC
        datetime.year = gDate.Year;
        datetime.month = gDate.Month;
        datetime.day = gDate.Date;
        datetime.hour = (gTime.Hours + 12) % 24; // Convert to 24-hour format
        datetime.minute = gTime.Minutes;
        datetime.second = gTime.Seconds;
        datetime.millisecond = 0; // Set milliseconds to 0 for simplicity

        dt_encode(&datetime, encoded);
        // convert the dt_denste_time to a byte array
        uint8_t encoded_bytes[sizeof(dt_dense_time)];
        memcpy(encoded_bytes, &encoded, sizeof(dt_dense_time));
        can_write(MESSAGE_CODE_TIME, encoded_bytes, 0); // Send a time message*/
        }
}

/**
 * @brief  Function to write data to the CAN interface.
 *
 * @param  code: Code to be sent
 * @param  data: Pointer to the data to be written.
 * @param  length: Length of the data to be written.
 */
void can_write(MESSAGE_CODE code, uint8_t *data, uint16_t length) {

    if (data == NULL || length == 0) {
        return; // No data to send
    }

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,
                      GPIO_PIN_RESET); // Set a pin to indicate activity
    HAL_UART_Transmit(&huart1, &code, 1, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, data, length, HAL_MAX_DELAY);
    // Send a termination sequence to indicate the end of the message
    // This is optional, but can be useful for the receiver to know when the
    // message ends
    HAL_UART_Transmit(&huart1, termination_sequence, 3, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,
                      GPIO_PIN_RESET); // Set a pin to indicate activity
}

/**
 * @brief  Callback function for UART receive complete interrupt.
 * This function is called when a byte is received via UART.
 * It stores the received byte in a circular buffer and restarts the UART
 * receive interrupt.
 *
 * Remember that this function is called from an interrupt context, so it should
 * be kept short and efficient.
 *
 * @param  huart: Pointer to the UART handle.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        uint16_t next_head = (head + 1) % RX_BUFFER_SIZE;

        if (next_head != tail) // Avoid overwrite
        {
            rx_buffer[head] = rx_byte;
            head = next_head;
        }

        // Restart interrupt for next byte
        HAL_UART_Receive_IT(huart, &rx_byte, 1);
    }
}