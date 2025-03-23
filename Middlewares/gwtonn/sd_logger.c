/*
 * sd_logger.c
 *
 *  Created on: Mar 17, 2025
 *      Author: Rudi Middel
 */

#include "fatfs.h"
#include "sd_logger.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 128
#define CLEAR_BUFFER(buffer) memset(buffer, '\0', sizeof(buffer))

FATFS filesystem;  // file system

void sd_logger_open() {
	f_mount(&filesystem, "/", 1);
}
void sd_logger_close() {
	f_mount(&filesystem, "/", 1);
}
void sd_logger_print(char *string) {
	FIL fil; // File

	if (FR_OK == f_open(&fil, LOG_FILENAME, FA_OPEN_APPEND | FA_READ | FA_WRITE))
	{
		/* write the string to the file */
		f_puts(string, &fil);
	}

	f_close (&fil);

}

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
	char string[BUFFER_SIZE]; // to store strings..

	osMutexId_t mutex_id;
	mutex_id = osMutexNew(NULL);
	sd_logger_open();

	while (1)
	{
		status = osMessageQueueGet(loggerQueueHandle, &msg, NULL, osWaitForever); // wait for message
		if (osOK == status)
		{
			CLEAR_BUFFER(string);
			sprintf(string, "Message: {index: %d, message: %d}\n\r", msg.index, msg.message);
			//uart_print_string(string);

			if (osOK == osMutexAcquire(mutex_id, 0))
			{
				sd_logger_print(string);
				osMutexRelease(mutex_id);
			}
			else
			{
				//uart_print_string("Could not get the MUTEX 'mutex_sd_card_lockHandle'\0");
			}
		}
	}
	sd_logger_close();
	osMutexDelete(mutex_id);
	
	/* USER CODE END startLogTask */
}