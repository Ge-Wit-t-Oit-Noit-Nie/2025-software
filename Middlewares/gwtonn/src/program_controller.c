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

#include "FreeRTOS.h"
#include "asserts.h"
#include "cmsis_os2.h"
#include "fatfs.h"
#include "internal_sensors.h"
#include "asserts.h"
#include "program_controller.h"
#include "logger.h"

/***************************************
 * Private functions
 **************************************/
typedef uint16_t (*program_controller_function_t)(
    program_controller_registers_t *program_controller_registers);

#define NUMBER_OF_PINS_MAPPED 3
typedef struct
{
  uint16_t pin_number;
  GPIO_TypeDef *port;
} pin_mapping_t;

static const pin_mapping_t pin_mapping[NUMBER_OF_PINS_MAPPED] = {
    {GPIO_PIN_0, GPIOB}, // Pin 0 on GPIOA
    {GPIO_PIN_7, GPIOB},
    {GPIO_PIN_14, GPIOB},
};

/**
 * @brief  Function to exit the program.
 *
 * This function will terminate the program and turn off the green LED.
 * It will also terminate the program controller task.
 *
 * @note   This function is called when the program is finished or when the
 * kill switch is pressed.
 * * @param  program_controller_registers: Pointer to the program controller
 * registers.
 * @retval 0 = VM_EXIT
 */
uint16_t
vm_exit(program_controller_registers_t *program_controller_registers)
{
  UNUSED(program_controller_registers);
  printf("Exit program\n\r");
  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
  osThreadTerminate(programTaskHandle);
  return VM_EXIT;
}

/**
 * @brief  Function to store the shutdown index.
 *
 * This function will store the shutdown index in the program controller
 * registers. It will also increment the instruction pointer.
 *
 * @param  program_controller_registers: Pointer to the program controller
 * registers.
 * @retval 0 = VM_OK
 */
uint16_t
vm_store_shutdown_index(
    program_controller_registers_t *program_controller_registers)
{
  ASSERT_NULL(program_controller_registers, VM_ERROR);

  printf("Store shutdown index %d\n\r",
         (uint16_t)program_controller_registers->register1);
  program_controller_registers->shutdown_instruction_pointer = (uint16_t)program_controller_registers->register1;
  program_controller_registers->instruction_pointer++;
  return VM_OK;
}

/**
 * @brief  Function to delay the program.
 *
 * This function will delay the program for the amount of time specified in
 * register1. It will also check for the kill switch and pause events.
 * @param  program_controller_registers: Pointer to the program controller
 * registers.
 * @retval 0 = VM_OK if the delay was successful; VM_EXIT if the program was
 * terminated; VM_ERROR if the program controller registers are NULL.
 */
uint16_t
vm_delay(program_controller_registers_t *program_controller_registers)
{
  ASSERT_NULL(program_controller_registers, VM_ERROR);

  printf("Delay for %d ms\n\r", program_controller_registers->register1);
  uint32_t state = osEventFlagsWait(
      ext_interrupt_eventHandle,
      EXTERN_INTERRUPT_EVENT_KILL | EXTERN_INTERRUPT_EVENT_PAUZE,
      osFlagsWaitAny,
      pdMS_TO_TICKS((uint32_t)program_controller_registers->register1) * 100); // Convert sec to ticks
  if (osFlagsErrorTimeout == (osFlagsErrorTimeout & state))
  {
    program_controller_registers->instruction_pointer++;
  }
  else if (EXTERN_INTERRUPT_EVENT_KILL == (state & EXTERN_INTERRUPT_EVENT_KILL))
  {
    HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
    osThreadTerminate(programTaskHandle);
    return VM_EXIT;
  }
  else if (EXTERN_INTERRUPT_EVENT_PAUZE == (state & EXTERN_INTERRUPT_EVENT_PAUZE))
  {
    program_controller_registers->instruction_pointer = program_controller_registers->shutdown_instruction_pointer;
  }
  return VM_OK;
}

/**
 * @brief  Function to turn on a pin.
 *
 * This function will turn on the pin specified in the lowest 5 bits (0x1F) in
 * resister1. It will also increment the instruction pointer.
 *
 * @param  program_controller_registers: Pointer to the program controller
 * registers.
 * @retval 0 = VM_OK; VM_ERROR if the program controller registers are NULL or
 * if the pin index is invalid.
 */
uint16_t
vm_pin_on(program_controller_registers_t *program_controller_registers)
{
  ASSERT_NULL(program_controller_registers, VM_ERROR);

  uint16_t pin_index = program_controller_registers->register1 & 0x001F;
  printf("Turn on pin %d\n\r", pin_index);
  if (NUMBER_OF_PINS_MAPPED <= pin_index)
  {
    return VM_ERROR; // Error code
  };

  HAL_GPIO_WritePin(pin_mapping[pin_index].port,
                    pin_mapping[pin_index].pin_number, GPIO_PIN_SET);
  program_controller_registers->instruction_pointer++;
  return VM_OK;
}

/**
 * @brief  Function to turn off a pin.
 *
 * This function will turn off the pin specified in the lowest 5 bits (0x1F) in
 * resister1. It will also increment the instruction pointer.
 *
 * @param  program_controller_registers: Pointer to the program controller
 * registers.
 * @retval 0 = VM_OK; VM_ERROR if the program controller registers are NULL or
 * if the pin index is invalid.
 */
uint16_t
vm_pin_off(program_controller_registers_t *program_controller_registers)
{
  ASSERT_NULL(program_controller_registers, VM_ERROR);

  uint16_t pin_index = program_controller_registers->register1 & 0x001F;
  printf("Turn off pin %d\n\r", pin_index);
  if (NUMBER_OF_PINS_MAPPED <= pin_index)
  {
    return VM_ERROR; // Error code
  };

  HAL_GPIO_WritePin(pin_mapping[pin_index].port,
                    pin_mapping[pin_index].pin_number, GPIO_PIN_RESET);
  program_controller_registers->instruction_pointer++;
  return VM_OK;
}
/**
 * @brief  Function to toggle a pin.
 *
 * This function will toggle the pin specified in the lowest 5 bits (0x1F) in
 * resister1. It will also increment the instruction pointer.
 *
 * @param  program_controller_registers: Pointer to the program controller
 * registers.
 * @retval 0 = VM_OK; VM_ERROR if the program controller registers are NULL or
 * if the pin index is invalid.
 */
uint16_t
vm_pin_toggle(program_controller_registers_t *program_controller_registers)
{
  ASSERT_NULL(program_controller_registers, VM_ERROR);

  uint16_t pin_index = program_controller_registers->register1 & 0x001F;
  printf("Toggle pin %d\n\r", pin_index);
  if (NUMBER_OF_PINS_MAPPED <= pin_index)
  {
    return VM_ERROR; // Error code
  };

  HAL_GPIO_TogglePin(pin_mapping[pin_index].port,
                     pin_mapping[pin_index].pin_number);
  program_controller_registers->instruction_pointer++;
  return VM_OK;
}
/**
 * @brief  Function to send telemetry data.
 *
 * This function will send telemetry data to the logger queue.
 * It will also increment the instruction pointer.
 *
 * @param  program_controller_registers: Pointer to the program controller
 * registers.
 * @retval 0 = VM_OK; VM_ERROR if the program controller registers are NULL.
 */
uint16_t
vm_send_telemetry(
    program_controller_registers_t *program_controller_registers)
{
  ASSERT_NULL(program_controller_registers, VM_ERROR);

  printf("Send telemetry\n\r");
  telemetry_t telemetry = {
      program_controller_registers->instruction_pointer,
      program_controller_registers->shutdown_instruction_pointer,
      is_get_temperature(),
      is_get_vref(),
      0,
  };
  if (osOK != osMessageQueuePut(loggerQueueHandle, &telemetry, 0, 0U))
  {
    printf("Error: Could not send message to loggerQueueHandle\n\r");
  }
  program_controller_registers->instruction_pointer++;
  return VM_OK;
}

/**
 * @brief  Function to jump to a specific instruction.
 *
 * This function will jump to the instruction specified in register1.
 * It will also set the instruction pointer to the value in register1.
 *
 * @param  program_controller_registers: Pointer to the program controller
 * registers.
 * @retval 0 = VM_OK; VM_ERROR if the program controller registers are NULL.
 */
uint16_t
vm_jump(program_controller_registers_t *program_controller_registers)
{
  ASSERT_NULL(program_controller_registers, VM_ERROR);

  printf("Jump to instruction %d\n\r",
         (uint16_t)program_controller_registers->register1);
  program_controller_registers->instruction_pointer = (uint16_t)program_controller_registers->register1;
  return VM_OK;
}

/**
 * Map all the functions to the opcodes.
 * The order of the functions in this array must match the order of the opcodes
 * in the enum. The functions are called in the program_controller_task()
 * function.
 */
static const program_controller_function_t program_controller_function[] = {
    vm_exit,                 // OPCODE_HALT
    vm_store_shutdown_index, // OPCODE_STORE_SHUTDOWN_INDEX
    vm_delay,                // OPCODE_DELAY
    vm_pin_on,               // OPCODE_PIN_ON
    vm_pin_off,              // OPCODE_PIN_OFF
    vm_pin_toggle,           // OPCODE_PIN_TOGGLE
    vm_send_telemetry,       // OPCODE_LOG_PROGRAM_STATE
    vm_jump,                 // OPCODE_JUMP
};

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
  UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable'
                    // warning

  program_controller_registers_t pcr = {
      .instruction_pointer = 0,
      .register1 = 0,
      .shutdown_instruction_pointer = 0,
  };
  is_set_time(0, 0, 0); // Set the RTC time to 00:00:00

  uint16_t *programma = (uint16_t *)&_program_data_start;

  /* Infinite loop */
  for (;;)
  {
    // Make sure the kill switch is handeld
    uint32_t state = osEventFlagsGet(ext_interrupt_eventHandle);
    if (EXTERN_INTERRUPT_EVENT_KILL == (state & EXTERN_INTERRUPT_EVENT_KILL))
    {
      HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin,
                        GPIO_PIN_RESET);
      osThreadTerminate(programTaskHandle);
      return;
    }

    uint16_t memory_entry = programma[pcr.instruction_pointer];
    pcr.register1 = REGISTER(memory_entry);
    uint8_t opcode = OPCODE(memory_entry);
    // If the opcode is 0xFF, this should be interpreted as a 0x00
    // By default, the flash memory is filled with 0xFF, so this is a way to
    // detect the end of the program or an empty instruction.
    if (0xFF == opcode)
    {
      opcode = 0x00;
    }
    program_controller_function[opcode](&pcr);
  }
}

/***************************************
 * Callback functions
 **************************************/

/**
 * @brief  This function is called when an external interrupt occurs.
 * @param  GPIO_Pin: The pin that triggered the interrupt.
 *
 * This function will set the event flag for the interrupt and send telemetry
 * data to the logger queue. It will also check which pin triggered the
 * interrupt and set the corresponding flag.
 *
 * @note   This function is called from the HAL library.
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint32_t flag = 0;
  switch (GPIO_Pin)
  {
  case PROGRAM_STATE_EMERGENCY_Pin:
    flag = EXTERN_INTERRUPT_EVENT_KILL;
    break;
  case PROGRAM_STATE_TRIGGER_Pin:
    flag = EXTERN_INTERRUPT_EVENT_PAUZE;
    break;
  default:
    break;
  }

  telemetry_t telemetry = {
      0,
      0,
      is_get_temperature(),
      is_get_vref(),
      flag,
  };

  if (osOK != osMessageQueuePut(loggerQueueHandle, &telemetry, 0, 0U))
  {
    printf("Error: Could not send message to loggerQueueHandle\n\r");
  }
  //osEventFlagsSet(ext_interrupt_eventHandle, flag);
}

static FATFS fs; // file system
void check_new_program()
{
  FIL fil;

  FRESULT mount_status = f_mount(&fs, "0:", 1); // mount the file system
  if (FR_OK == mount_status)
  {
    if (FR_OK == f_open(&fil, "program.bin", FA_READ))
    {
      printf("---- Programma gevonden: ");
    }
    else
    {
      printf("<<<<Programma niet gevonden>>>>");
    }
    f_close(&fil);
    f_mount(NULL, "", 0); // unmount the file system
  }
  else
  {
    printf("Card not mounted (errno: %d)\n\r", mount_status);
  }
  return;
}