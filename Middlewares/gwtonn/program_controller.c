
#include <main.h>
#include <stdio.h>
#include "program_controller.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "sd_logger.h"

#include "programma.h"

extern const instruction_t instruction[];

/**
 * @brief  Function implementing the program_controller_task thread.
 * @param  argument: Not used
 * @retval None
 */
void program_controller_task(void *argument)
{
    UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable' warning
    program_controller_registers_t pcr = {
        .program_counter = 0,
        .program_size = sizeof(instruction) / sizeof(instruction_t),
    };
    uint32_t state = 0;

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
        switch (instruction[pcr.program_counter].opcode)
        {
        case OPCODE_PIN_TOGGLE:
            HAL_GPIO_TogglePin((GPIO_TypeDef *)instruction[pcr.program_counter].parameter1,
                               (uint32_t)instruction[pcr.program_counter].parameter0);
            program_controller_step(&pcr);
            break;

        case OPCODE_PIN_STATE:
            HAL_GPIO_WritePin((GPIO_TypeDef *)instruction[pcr.program_counter].parameter1,
                              (uint32_t)instruction[pcr.program_counter].parameter0,
                              (uint32_t)instruction[pcr.program_counter].parameter2);
            program_controller_step(&pcr);
            break;

        case OPCODE_DELAY:
            state = osEventFlagsWait(ext_interrupt_eventHandle, EXTERN_INTERRUPT_EVENT_KILL | EXTERN_INTERRUPT_EVENT_PAUZE, osFlagsWaitAny, pdMS_TO_TICKS((uint32_t)instruction[pcr.program_counter].parameter0));
            if ((osFlagsErrorTimeout & state) == osFlagsErrorTimeout)
            {
                program_controller_step(&pcr);
            }
            else if ((state & EXTERN_INTERRUPT_EVENT_KILL) == EXTERN_INTERRUPT_EVENT_KILL)
            {
                pcr.program_counter = pcr.shutdown_index_register;
                return;
            }
            else if ((state & EXTERN_INTERRUPT_EVENT_PAUZE) == EXTERN_INTERRUPT_EVENT_PAUZE)
            {
                // TODO: Implement pause functionality
                HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
                osThreadTerminate(programTaskHandle);
                return;
            }

            break;

        case OPCODE_LOG_PROGRAM_STATE:
        {
            MSGQUEUE_OBJ_t msg = {pcr.program_counter, MSG_PROGRAM_COUNTER};
            if (osOK != osMessageQueuePut(loggerQueueHandle, &msg, 0, 0U))
            {
                printf("Error: Could not send message to loggerQueueHandle\n\r");
            }
            printf("Program counter: %ld;message: %d\n\r", pcr.program_counter, MSG_PROGRAM_COUNTER);
            program_controller_step(&pcr);
            break;
        }
        case OPCODE_JUMP:
            pcr.program_counter = (uint32_t)instruction[pcr.program_counter].parameter0;
            break;

        case OPCODE_STORE_SHUTDOWN_INDEX:
            pcr.shutdown_index_register = (uint32_t)instruction[pcr.program_counter].parameter0;
            program_controller_step(&pcr);
            break;
        case OPCODE_HALT:
            osThreadTerminate(programTaskHandle);
            break;
        }
    }
}

/**
 * @brief  Set the program counter to the next instruction.
 *
 * This function increments the program counter and wraps around if the end of the program is reached.
 * @param  argument: Not used
 */
void program_controller_step(program_controller_registers_t *program_controller_registers)
{
    program_controller_registers->program_counter++;
    if (program_controller_registers->program_counter >= program_controller_registers->program_size)
        program_controller_registers->program_counter = 0;
}

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