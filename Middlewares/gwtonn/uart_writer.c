/**
 ******************************************************************************
 * @file   uart_writer.c
 * @brief  Implementation of the uart writer
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

#include <FreeRTOS.h>
#include "uart_writer.h"
#include "usart.h"

/**
 * @brief  Retargets the C library printf function to the USART.
 *   None
 * @retval None
 */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}

/**
 * @brief Function implementing the uartWriterTask thread.
 * @param argument: Not used
 * @retval None
 */
void uart_writer_task(void *argument)
{
    UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable' warning

    for(;;)
    {
        osDelay(pdMS_TO_TICKS(100));
        printf("Hello, world!\n\r");
    }
}