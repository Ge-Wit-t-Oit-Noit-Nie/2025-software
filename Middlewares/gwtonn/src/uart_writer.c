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

#include "uart_writer.h"
#include "usart.h"

/**
 * @brief  Retargets the C library printf function to the USART.
 * 
 * This function is used to retarget the C library printf function to the USART.
 * @param  ch: The character to write
 * @retval the character written
 */
int __io_putchar(int ch)
{
    if (HAL_UART_STATE_READY == HAL_UART_GetState(&huart3))
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
    }
    return ch;
}
