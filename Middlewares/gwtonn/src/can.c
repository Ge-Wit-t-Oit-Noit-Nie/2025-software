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

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "can.h"

/**
 * @brief  FREERTOS thread handler for the CAN interface.
 */
void can_thread_handler(void *argument) {

    for (;;)
    {
        osDelay(pdMS_TO_TICKS(10)); // Delay for 1000 ms (1 second)
    }
    
}
