/**
 ******************************************************************************
 * @file   program_controller.c
 * @brief  Implementation of the program controller
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
#include <main.h>
#include <stdio.h>
#include "program_controller.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "logger.h"
#include "rtc.h"
#include "internal_sensors.h"

#include "programma.h"

extern volatile MEM_PROGRAM_DATA_BLOCK instruction_t instruction[];
void program_controller_step(program_controller_registers_t *program_controller_registers);

/***************************************
 * Public functions
 **************************************/

/**
 * @brief  Function implementing the program_controller_task thread.
 * @param  argument: Not used
 * @retval None
 */
void program_controller_task(void *argument)
{
    UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable' warning

    program_controller_registers_t pcr = {
        .instruction_pointer = 0,
        .shutdown_instruction_pointer = 0,
    };
    uint32_t state = 0;
    is_set_time(12, 2, 2); // Set the RTC time to 00:00:00

    /* Infinite loop */
    for (;;)
    {
        // Make sure the kill switch is handeld
        state = osEventFlagsGet(ext_interrupt_eventHandle);
        if ((state & EXTERN_INTERRUPT_EVENT_KILL) == EXTERN_INTERRUPT_EVENT_KILL)
        {
            HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
            osThreadTerminate(programTaskHandle);
            return;
        }
        printf("Program counter: %d, Shutdown index register: %d\n\r", pcr.instruction_pointer, pcr.shutdown_instruction_pointer);

        switch (instruction[pcr.instruction_pointer].opcode)
        {
        case OPCODE_PIN_TOGGLE:
            HAL_GPIO_TogglePin((GPIO_TypeDef *)instruction[pcr.instruction_pointer].parameter1,
                               (uint32_t)instruction[pcr.instruction_pointer].parameter0);
            program_controller_step(&pcr);
            break;

        case OPCODE_PIN_STATE:
            HAL_GPIO_WritePin((GPIO_TypeDef *)instruction[pcr.instruction_pointer].parameter1,
                              (uint32_t)instruction[pcr.instruction_pointer].parameter0,
                              (uint32_t)instruction[pcr.instruction_pointer].parameter2);
            program_controller_step(&pcr);
            break;

        case OPCODE_DELAY:
            state = osEventFlagsWait(ext_interrupt_eventHandle, EXTERN_INTERRUPT_EVENT_KILL | EXTERN_INTERRUPT_EVENT_PAUZE, osFlagsWaitAny, pdMS_TO_TICKS((uint32_t)instruction[pcr.instruction_pointer].parameter0));
            if ((osFlagsErrorTimeout & state) == osFlagsErrorTimeout)
            {
                program_controller_step(&pcr);
            }
            else if ((state & EXTERN_INTERRUPT_EVENT_KILL) == EXTERN_INTERRUPT_EVENT_KILL)
            {
                HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
                osThreadTerminate(programTaskHandle);
                return;
            }
            else if ((state & EXTERN_INTERRUPT_EVENT_PAUZE) == EXTERN_INTERRUPT_EVENT_PAUZE)
            {
                // TODO: Implement pause functionality
                pcr.instruction_pointer = pcr.shutdown_instruction_pointer;
            }
            break;

        case OPCODE_LOG_PROGRAM_STATE:
        {
            telemetry_t telemetry = {
                pcr.instruction_pointer,
                pcr.shutdown_instruction_pointer,
                is_get_temperature(),
                is_get_vref(),
            };
            if (osOK != osMessageQueuePut(loggerQueueHandle, &telemetry, 0, 0U))
            {
                printf("Error: Could not send message to loggerQueueHandle\n\r");
            }
            program_controller_step(&pcr);
            break;
        }
        case OPCODE_JUMP:
            pcr.instruction_pointer = (uint32_t)instruction[pcr.instruction_pointer].parameter0;
            break;

        case OPCODE_STORE_SHUTDOWN_INDEX:
            pcr.shutdown_instruction_pointer = (uint32_t)instruction[pcr.instruction_pointer].parameter0;
            program_controller_step(&pcr);
            break;

        case OPCODE_HALT:
            HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
            osThreadTerminate(programTaskHandle);
            break;
        }
    }
}

/***************************************
 * Callback functions
 **************************************/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /* Prevent unused argument(s) compilation warning */
    if (PROGRAM_STATE_EMERGENCY_Pin == GPIO_Pin)
    {
        osEventFlagsSet(ext_interrupt_eventHandle, EXTERN_INTERRUPT_EVENT_KILL);
    }
    /* Prevent unused argument(s) compilation warning */
    if (PROGRAM_STATE_TRIGGER_Pin == GPIO_Pin)
    {
        osEventFlagsSet(ext_interrupt_eventHandle, EXTERN_INTERRUPT_EVENT_PAUZE);
    }
}

/***************************************
 * private functions
 **************************************/

/**
 * @brief  Set the program counter to the next instruction.
 *
 * This function increments the program counter and wraps around if the end of the program is reached.
 * @param  argument: Not used
 */
void program_controller_step(program_controller_registers_t *program_controller_registers)
{
    program_controller_registers->instruction_pointer++;
}