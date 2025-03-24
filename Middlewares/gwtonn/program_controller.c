#include "program_controller.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include <main.h>
#include "programma.h"
#include "sd_logger.h"

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

    HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);

    /* Infinite loop */
    for (;;)
    {
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
            osDelay((uint32_t)instruction[pcr.program_counter].parameter0);
            program_controller_step(&pcr);
            break;

        case OPCODE_LOG_PROGRAM_STATE:
            // Log the state of the program
            MSGQUEUE_OBJ_t msg;
            msg.index = pcr.program_counter;
            msg.message = MSG_PROGRAM_COUNTER;
            osMessageQueuePut(loggerQueueHandle, &msg, 0, 0U);            
            program_controller_step(&pcr);
            break;

        case OPCODE_JUMP:
            pcr.program_counter = (uint32_t)instruction[pcr.program_counter].parameter0;
            break;
        }
    }

    HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
}

/**
 * @brief  Set the program counter to the next instruction.
 * 
 * This function increments the program counter and wraps around if the end of the program is reached.
 * @param  argument: Not used
 */
void program_controller_step(program_controller_registers_t *program_controller_registers) {
    program_controller_registers->program_counter++;
    if (program_controller_registers->program_counter >= program_controller_registers->program_size)
        program_controller_registers->program_counter = 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(GPIO_Pin);

    if (PROGRAM_STATE_TRIGGER_Pin == GPIO_Pin)
    {
        MSGQUEUE_OBJ_t msg;
        msg.message = MSG_PROGRAM_STATE_TRIGGER;
        msg.index = 0;
        osMessageQueuePut(loggerQueueHandle, &msg, 0, 0U);

    }
}