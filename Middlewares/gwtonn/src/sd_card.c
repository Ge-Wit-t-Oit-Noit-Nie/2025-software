/**
 ******************************************************************************
 * \file   sd_card.c
 * \brief  Implementation of functions that interact with the SD Card.
 ******************************************************************************
 * \attention
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
#include <stdio.h>
#include <stdlib.h>

#include "sd_card.h"
#include "program_controller.h"
#include "stm32f4xx_hal.h"

#define WAIT_TILL_FLASH_READY                                                                                            \
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR); \
  if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY))                                                                              \
  {                                                                                                                      \
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY))                                                                         \
      ;                                                                                                                  \
  }

/**
 * \brief  Check if the SD Card is present.
 */
FRESULT sd_card_is_present(void)
{
  return (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 0) ? FR_OK : FR_NOT_READY;
}

/**
 * \brief  Check on the SD Card if a file exists.
 */
FRESULT file_exists(const char *filename)
{
  FATFS filesystem;
  FRESULT status = FR_OK;
  status = f_mount(&filesystem, "0:", 1);
  if (FR_OK == status)
  {
    status = f_stat(filename, NULL);
    f_mount(NULL, "", 0); // unmount the file system
  }
  return status;
}

/**
 * \brief  Write a file to the SD Card.
 * 
 */
FRESULT
write_file(const char *filename, const char *content)
{
  FATFS filesystem;
  FRESULT status = FR_OK;
  FIL file;

  if(sd_card_is_present() != FR_OK)
  {
    printf("SD Card not present\n\r");
    return FR_NOT_READY; // SD Card is not present
  }
  status = f_mount(&filesystem, "0:", 1);
  if (FR_OK == status)
  {
    status = f_open(&file, filename, FA_OPEN_APPEND | FA_READ | FA_WRITE);
    if (FR_OK == status)
    {
      /* write the string to the file */
      f_puts(content, &file);
      f_close(&file);
    }
    else
    {
      printf("Cannot open file (errno: %d)\n\r", status);
    }
    f_mount(NULL, "", 0); // unmount the file system
  }
  else
  {
    printf("Card not mounted (errno: %d)\n\r", status);
  }

  return status;
}

/**
 * \brief  Load the program (binary) into the specified memory location.
 *
 */
FRESULT
load_progam_from_sd_to_flash(const char *filename, uint32_t address_memory)
{
  FATFS filesystem;       // file system
  FRESULT status = FR_OK; // flag to check if the card is enabled
  FIL file;
  uint16_t buffer_size = 10;
  UINT br;                                          // number of bytes read
  uint8_t *buffer = (uint8_t *)malloc(buffer_size); // allocate a buffer for the file content

  WAIT_TILL_FLASH_READY

  status = f_mount(&filesystem, "0:", 1); // mount the file system
  if (FR_OK == status)
  {
    status = f_open(&file, filename, FA_OPEN_EXISTING | FA_READ);
    if (FR_OK == status)
    {

      __disable_irq();
      HAL_FLASH_Unlock(); // unlock the flash
      // erase the flash sector where the program will be loaded
      FLASH_Erase_Sector(FLASH_SECTOR_11, VOLTAGE_RANGE_3); 

      // read the file in chunks of buffer_size bytes
      do
      {
        f_read(&file, buffer, buffer_size, &br);
        for (UINT i = 0; i < br; i++)
        {
          HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address_memory, buffer[i]);
          address_memory++;
        }
      } while (br == buffer_size); // read the file in chunks of buffer_size bytes

      HAL_FLASH_Lock(); // lock the flash
      __enable_irq();
    }
    else
    {
      printf("Cannot open file (errno: %d)\n\r", status);
    }
    f_close(&file);
    f_mount(NULL, "", 0); // unmount the file system
  }
  else
  {
    printf("Card not mounted (errno: %d)\n\r", status);
  }

  return status;
}