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

#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {                                // object data type
  uint8_t message;
  uint8_t index;
} MSGQUEUE_OBJ_t;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 128

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
/* Definitions for loggerQueue */
osMessageQueueId_t loggerQueueHandle;
const osMessageQueueAttr_t loggerQueue_attributes = {
  .name = "loggerQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/**
 * @brief Fill the buffer with '\0'
 * @param buf the buffer to clear
 */
void clear_buffer (char *buf);

/* USER CODE END FunctionPrototypes */

void startProgramTask(void *argument);
void startLogTask(void *argument);

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of programTask */
  programTaskHandle = osThreadNew(startProgramTask, NULL, &programTask_attributes);

  /* creation of logTask */
  logTaskHandle = osThreadNew(startLogTask, NULL, &logTask_attributes);

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
	MSGQUEUE_OBJ_t msg;

	/* Infinite loop */
	for(;;)
	{
		msg.index++;
		msg.message = 'A';
		osMessageQueuePut(loggerQueueHandle, &msg, 0, 0U);
		osDelay(pdMS_TO_TICKS(1000));
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
void startLogTask(void *argument)
{
  /* USER CODE BEGIN startLogTask */
		MSGQUEUE_OBJ_t msg;
		osStatus_t status;
		char string[BUFFER_SIZE];  // to store strings..

		osMutexId_t mutex_id;
		mutex_id = osMutexNew(NULL);
		sd_logger_open();

		while (1) {
			status = osMessageQueueGet(loggerQueueHandle, &msg, NULL, osWaitForever);   // wait for message
			if (osOK == status) {
				clear_buffer(string);
				sprintf (string, "Message: {index: %d, message: %d}\n\r", msg.index, msg.message);
				uart_print_string(string);

				if(osOK == osMutexAcquire(mutex_id, 0)){
					sd_logger_print(string);
					osMutexRelease(mutex_id);
				} else {
					uart_print_string("Could not get the MUTEX 'mutex_sd_card_lockHandle'\0");
				}

			}
		}
		sd_logger_close();

  /* USER CODE END startLogTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
 * @brief Fill the buffer with '\0'
 */
void clear_buffer (char *buf)
{
	for (int i=0; i<BUFFER_SIZE; i++) buf[i] = '\0';
}

/* USER CODE END Application */

