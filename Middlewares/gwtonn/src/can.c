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
#define UART_RX_BUF_SIZE 128
uint8_t uartRxBuf[UART_RX_BUF_SIZE];

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

    enum {
        LOOKING_FOR_START, // Initial state, waiting for the start of a message
        WAITING_FOR_DATA   // Waiting for data to be received
    } state = LOOKING_FOR_START;
    MESSAGE_CODE code = 0;   // Variable to store the message code
    uint8_t data[64];        // Buffer to store the received data
    uint8_t data_length = 0; // Length of the received data
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,
                      GPIO_PIN_SET); // Set a pin to indicate activity

    // Start the UART receive process
    //HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    HAL_UART_Receive_DMA(&huart1, uartRxBuf, UART_RX_BUF_SIZE);

    for (;;) {
        osDelay(100); // Yield to other tasks
        if (head != tail) {

            uint8_t byte = rx_buffer[tail];
            tail = (tail + 1) % RX_BUFFER_SIZE; // Move tail forward

            switch (state) {
            case LOOKING_FOR_START:
                code = byte;              // Start of a new message
                state = WAITING_FOR_DATA; // Move to the next state
                data_length = 0;          // Reset data length
                break;

            case WAITING_FOR_DATA:
                data[data_length++] = byte; // Store the received byte
                if (data_length >= 3 &&
                    data[data_length - 3] == termination_sequence[0] &&
                    data[data_length - 2] == termination_sequence[1] &&
                    data[data_length - 1] == termination_sequence[2]) {

                    // If we received the termination sequence, process the
                    // message
                    data_length -= 3; // Remove the termination sequence

                    state =
                        LOOKING_FOR_START; // Reset state for the next message
                }
                break;
            default:
                break;
            }
        }
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

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,
                      GPIO_PIN_SET); // Set a pin to indicate activity
    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY) {
        // Wait until the UART is ready to transmit
        osDelay(1); // Avoid busy-waiting, yield to other tasks
    }
    HAL_UART_Transmit(&huart1, &code, 1, HAL_MAX_DELAY);
    if (data != NULL && length != 0) {
        while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY) {
            // Wait until the UART is ready to transmit
            osDelay(1); // Avoid busy-waiting, yield to other tasks
        }
        HAL_UART_Transmit(&huart1, data, length, HAL_MAX_DELAY);
    }
    // Send a termination sequence to indicate the end of the message
    // This is optional, but can be useful for the receiver to know when the
    // message ends
    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY) {
        // Wait until the UART is ready to transmit
        osDelay(1); // Avoid busy-waiting, yield to other tasks
    }
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
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,
                          GPIO_PIN_SET); // Set a pin to indicate activity
        uint16_t next_head = (head + 1) % RX_BUFFER_SIZE;

        if (next_head != tail) // Avoid overwrite
        {
            rx_buffer[head] = rx_byte;
            head = next_head;
        }

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,
                          GPIO_PIN_RESET); // Set a pin to indicate activity
        // Restart interrupt for next byte
        HAL_UART_Receive_IT(huart, &rx_byte, 1);
    }
}

