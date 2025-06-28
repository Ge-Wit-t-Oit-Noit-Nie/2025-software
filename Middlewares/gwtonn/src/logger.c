/**
 ******************************************************************************
 * @file   logger.c
 * @brief  Implementation of the logger
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

#include "logger.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "internal_sensors.h"
#include "sd_card.h"

#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 128
#define CLEAR_BUFFER(buffer) memset(buffer, '\0', sizeof(buffer))

/**
 * @brief Function implementing the logTask thread.
 *
 * This function will run a log task that will write to the SD card.
 * It will wait for messages from the loggerQueue and write them to the SD card.
 *
 * In case the card cannot be mounted, the task will clear the buffer and disable the card.
 *
 * @param argument: Not used
 * @retval None
 */
void logger_task(void *argument)
{
	UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable' warning
	telemetry_t telemetry;
	osStatus_t status;
	char string[BUFFER_SIZE];	  // to store strings..
	RTC_DateTypeDef gDate;
	RTC_TimeTypeDef gTime;
	osMutexId_t mutex_id = osMutexNew(NULL);
	if (NULL == mutex_id)
	{
		printf("Could not create the MUTEX 'mutex_sd_card_lockHandle'\n\r");
		return; // exit if mutex creation failed
	}
	while (1)
	{
		status = osMessageQueueGet(loggerQueueHandle, &telemetry, NULL, osWaitForever); // wait for message
		if (osOK == status)
		{
			is_get_date_time(&gDate , &gTime); // get the time from the RTC

			CLEAR_BUFFER(string);
			snprintf(string, BUFFER_SIZE, "[%02d:%02d:%02d],%08u,%08u,%05d,%05d,%05d\n\r",
					 gTime.Hours,
					 gTime.Minutes,
					 gTime.Seconds,
					 telemetry.instruction_pointer,
					 telemetry.shutdown_index_register,
					 telemetry.temperature,
					 telemetry.vrefint,
					 telemetry.trigger); // format the string to write to the file
			printf(string); // print to uart

			if (osOK == osMutexAcquire(mutex_id, 0))
			{
				write_file(LOG_FILENAME "\0", string);
				osMutexRelease(mutex_id);
			}
			else
			{
				printf("Could not get the MUTEX 'mutex_sd_card_lockHandle'\n\r");
			}
		}
	}

	osMutexDelete(mutex_id);
}