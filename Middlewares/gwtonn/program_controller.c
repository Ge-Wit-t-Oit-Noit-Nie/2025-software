#include <main.h>
#include <stdio.h>
#include "program_controller.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
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
    MSGQUEUE_OBJ_t msg;

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

/* Definitions for loggerQueue */
osMessageQueueId_t interruptQueueHandle;
const osMessageQueueAttr_t interruptQueueHandle_attributes = {
    .name = "interruptQueue"};

typedef struct
{
    uint8_t new_state;
    uint8_t trigger;
} TASKQUEUE_OBJ_t;

#define TASK_STATE_STOPPED 0
#define TASK_STATE_RUNNING !TASK_STATE_STOPPED

uint8_t program_controller_current_state = TASK_STATE_RUNNING;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    TASKQUEUE_OBJ_t msg;
    /* Prevent unused argument(s) compilation warning */
    if (PROGRAM_STATE_TRIGGER_Pin == GPIO_Pin)
    {
        msg.trigger = MSG_PROGRAM_STATE_TRIGGER;
        msg.new_state = !program_controller_current_state;
        osMessageQueuePut(interruptQueueHandle, &msg, 0, 0U);
        program_controller_current_state = !program_controller_current_state;
    }
}

/**
 * @brief Function implementing the triggerTask thread.
 * @param argument: Not used
 * @retval None
 */
void trigger_task(void *argument)
{
    UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable' warning
    interruptQueueHandle = osMessageQueueNew(2, sizeof(TASKQUEUE_OBJ_t), &interruptQueueHandle_attributes);
    TASKQUEUE_OBJ_t msg;

    osStatus_t status;

    /* Infinite loop */
    for (;;)
    {
        status = osMessageQueueGet(interruptQueueHandle, &msg, NULL, osWaitForever);
        if (osOK == status) {
            if (msg.trigger == MSG_PROGRAM_STATE_TRIGGER)
            {
                if (TASK_STATE_STOPPED == msg.new_state)
                {
                    osMessageQueuePut(loggerQueueHandle, &msg, 0, 0U);
                    osThreadTerminate(programTaskHandle);
                    printf("Stopping program\n");
                }
                else
                {
                    osMessageQueuePut(loggerQueueHandle, &msg, 0, 0U);
                    programTaskHandle = osThreadNew(program_controller_task, NULL, &programTask_attributes);
                    printf("Resuming program\n");
                }
            }
        }
    }
}
