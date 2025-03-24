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
#define MSG_PROGRAM_COUNTER 0x03
#define MSG_PROGRAM_STATE_TRIGGER 0x04

extern osThreadId_t programTaskHandle;
extern const osThreadAttr_t programTask_attributes;

typedef enum
{
    // Specific
    OPCODE_PIN_TOGGLE = 0x00,
    OPCODE_PIN_STATE = 0x01,

    // Program related
    OPCODE_DELAY = 0x11,
    OPCODE_LOG_PROGRAM_STATE = 0x12,    // Log the state of the program

    // Control flow
    OPCODE_JUMP = 0x21,
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
} program_controller_registers_t;

void program_controller_step(program_controller_registers_t *program_controller_registers);

#endif // INC_PROGAM_CONTROLLER_H_