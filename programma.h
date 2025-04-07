/*
 * programma.h
 *
 */

#ifndef __PROGRAMMA_H__
#define __PROGRAMMA_H__

#include <program_controller.h>
#include <main.h>

const instruction_t instruction[] = {
    {.opcode = OPCODE_STORE_SHUTDOWN_INDEX, .parameter0 = (void *)9},
    {.opcode = OPCODE_PIN_STATE, .parameter0 = (void *)GREEN_LED_Pin, .parameter1 = (void *)GREEN_LED_GPIO_Port, .parameter2 = (void *)GPIO_PIN_SET},
    {.opcode = OPCODE_PIN_STATE, .parameter0 = (void *)BLUE_LED_Pin, .parameter1 = (void *)BLUE_LED_GPIO_Port, .parameter2 = (void *)GPIO_PIN_SET},
    {.opcode = OPCODE_LOG_PROGRAM_STATE},
    {.opcode = OPCODE_DELAY, .parameter0 = (void *)1500},
    {.opcode = OPCODE_LOG_PROGRAM_STATE},
    {.opcode = OPCODE_PIN_TOGGLE, .parameter0 = (void *)BLUE_LED_Pin, .parameter1 = (void *)BLUE_LED_GPIO_Port},
    {.opcode = OPCODE_LOG_PROGRAM_STATE},
    {.opcode = OPCODE_JUMP, .parameter0 = (void *)3},
    {.opcode = OPCODE_LOG_PROGRAM_STATE},
    {.opcode = OPCODE_PIN_STATE, .parameter0 = (void *)RED_LED_Pin, .parameter1 = (void *)RED_LED_GPIO_Port, .parameter2 = (void *)GPIO_PIN_SET},
    {.opcode = OPCODE_PIN_STATE, .parameter0 = (void *)BLUE_LED_Pin, .parameter1 = (void *)BLUE_LED_GPIO_Port, .parameter2 = (void *)GPIO_PIN_SET},
    {.opcode = OPCODE_PIN_STATE, .parameter0 = (void *)GREEN_LED_Pin, .parameter1 = (void *)GREEN_LED_GPIO_Port, .parameter2 = (void *)GPIO_PIN_SET},
    {.opcode = OPCODE_HALT},
};

#endif // __PROGRAMMA_H__