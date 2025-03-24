/**
 ******************************************************************************
 * @file   uart_writer.h
 * @brief  Headerfile for the uart_writer.c file
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
#ifndef __UART_WRITER_H__
#define __UART_WRITER_H__

#include "cmsis_os2.h"

/**
 * @brief  Handle for the UART writer thread
 */
extern osMessageQueueId_t uart_queueHandle;
/**
 * @brief  Attributes for the UART writer thread
 */
extern const osMessageQueueAttr_t uart_queue_attributes;

int __io_putchar(int ch);

#endif // __UART_WRITER_H__