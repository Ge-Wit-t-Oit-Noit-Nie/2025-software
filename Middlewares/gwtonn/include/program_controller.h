/**
 ******************************************************************************
 * @file   internal_sensors.h
 * @brief  Headerfile for the internal_sensors.c file
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

#ifndef INC_PROGAM_CONTROLLER_H_
#define INC_PROGAM_CONTROLLER_H_

#include <stdint.h>
#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C"
{
#endif // End of extern "C" block

#define EXTERN_INTERRUPT_EVENT_PAUZE 0x01
#define EXTERN_INTERRUPT_EVENT_KILL 0x02

#define MEM_PROGRAM_DATA_BLOCK __attribute__((section(".program_data_block"), aligned(4)))

extern osThreadId_t programTaskHandle;
extern const osThreadAttr_t programTask_attributes;
/* Definitions for ext_interrupt_event */
extern osEventFlagsId_t ext_interrupt_eventHandle;
extern const osEventFlagsAttr_t ext_interrupt_event_attributes;

typedef enum
{
    // Control flow
    OPCODE_HALT = 0x00,
    OPCODE_JUMP = 0x01,
    OPCODE_STORE_SHUTDOWN_INDEX = 0x02,

    // Specific
    OPCODE_PIN_TOGGLE = 0x10,
    OPCODE_PIN_STATE = 0x11,

    // Program related
    OPCODE_DELAY = 0x20,
    OPCODE_LOG_PROGRAM_STATE = 0x21,

} OPCODE;

typedef struct
{
    OPCODE opcode;
    void *parameter0;
    void *parameter1;
    void *parameter2;
} instruction_t;

typedef struct
{
    uint8_t instruction_pointer;
    uint8_t shutdown_instruction_pointer;
} program_controller_registers_t;

#ifdef __cplusplus
}
#endif // End of extern "C" block

#endif // INC_PROGAM_CONTROLLER_H_