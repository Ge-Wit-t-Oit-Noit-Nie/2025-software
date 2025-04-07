/*
 * program.h
 *
 *  Created on: Mar 17, 2025
 *      Author: Rudi Middel
 */

#ifndef INC_PROGAM_CONTROLLER_H_
#define INC_PROGAM_CONTROLLER_H_

#include <stdint.h>
#include "cmsis_os2.h"

#define MSG_PROGRAM_START 0x01
#define MSG_PROGRAM_STOP 0x02
#define MSG_PROGRAM_RESUME 0x03
#define MSG_PROGRAM_STATE_TRIGGER 0x04

#define MSG_PROGRAM_COUNTER 0x10

#define EXTERN_INTERRUPT_EVENT_PAUZE 0x01
#define EXTERN_INTERRUPT_EVENT_KILL 0x02

extern osThreadId_t programTaskHandle;
extern const osThreadAttr_t programTask_attributes;
/* Definitions for ext_interrupt_event */
extern osEventFlagsId_t ext_interrupt_eventHandle;
extern const osEventFlagsAttr_t ext_interrupt_event_attributes;

typedef enum
{
    // Specific
    OPCODE_PIN_TOGGLE = 0x00,
    OPCODE_PIN_STATE = 0x01,

    // Program related
    OPCODE_DELAY = 0x11,
    OPCODE_LOG_PROGRAM_STATE = 0x12, // Log the state of the program
    OPCODE_HALT = 0x03,

    // Control flow
    OPCODE_JUMP = 0x21,
    OPCODE_STORE_SHUTDOWN_INDEX = 0x02,

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
    uint32_t program_counter;
    uint16_t program_size;
    uint16_t shutdown_index_register;
} program_controller_registers_t;

void program_controller_step(program_controller_registers_t *program_controller_registers);

#endif // INC_PROGAM_CONTROLLER_H_