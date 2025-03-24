/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fatfs.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "sd_logger.h"
#include "program_controller.h"

#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for programTask */
osThreadId_t programTaskHandle;
const osThreadAttr_t programTask_attributes = {
  .name = "programTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for logTask */
osThreadId_t logTaskHandle;
const osThreadAttr_t logTask_attributes = {
  .name = "logTask",
  .stack_size = 3000 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for uartWriterTask */
osThreadId_t uartWriterTaskHandle;
const osThreadAttr_t uartWriterTask_attributes = {
  .name = "uartWriterTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for loggerQueue */
osMessageQueueId_t loggerQueueHandle;
const osMessageQueueAttr_t loggerQueue_attributes = {
  .name = "loggerQueue"
};
/* Definitions for uart_queue */
osMessageQueueId_t uart_queueHandle;
const osMessageQueueAttr_t uart_queue_attributes = {
  .name = "uart_queue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void startProgramTask(void *argument);
void startLogTask(void *argument);
void uart_writer_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of loggerQueue */
  loggerQueueHandle = osMessageQueueNew (8, sizeof(MSGQUEUE_OBJ_t), &loggerQueue_attributes);

  /* creation of uart_queue */
  uart_queueHandle = osMessageQueueNew (16, sizeof(MSGQUEUE_OBJ_t), &uart_queue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of programTask */
  programTaskHandle = osThreadNew(startProgramTask, NULL, &programTask_attributes);

  /* creation of logTask */
  logTaskHandle = osThreadNew(startLogTask, NULL, &logTask_attributes);

  /* creation of uartWriterTask */
  uartWriterTaskHandle = osThreadNew(uart_writer_task, NULL, &uartWriterTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_startProgramTask */
/**
  * @brief  Function implementing the programTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_startProgramTask */
void startProgramTask(void *argument)
{
  /* USER CODE BEGIN startProgramTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END startProgramTask */
}

/* USER CODE BEGIN Header_startLogTask */
/**
* @brief Function implementing the logTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startLogTask */
__weak void startLogTask(void *argument)
{
  /* USER CODE BEGIN startLogTask */
  UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable' warning

  /* USER CODE END startLogTask */
}

/* USER CODE BEGIN Header_uart_writer_task */
/**
* @brief Function implementing the uartWriterTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_uart_writer_task */
__weak void uart_writer_task(void *argument)
{
  /* USER CODE BEGIN uart_writer_task */
  UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable' warning

  /* USER CODE END uart_writer_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

