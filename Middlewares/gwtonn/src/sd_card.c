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

#include "sd_card.h"

/**
 * \brief  Write a file to the SD Card.
 *
 * Writing to the SD Card requires some extra steps, such as mounting the SD
 * Card and checking the status. This function handles that all. After the
 * checkes, it will open the file for APPEND and write the content of content
 * to the file.
 * \param[in] filename the filename to write to
 * \param[in] content the content to write (must the \0 terminated)
 * \retval FR_OK in case of sucess; else any of the #FRESULT.
 */
FRESULT
write_file (const char *filename, const char *content)
{
  FATFS filesystem;
  FRESULT status = FR_OK;
  FIL file;

  status = f_mount (&filesystem, "0:", 1);
  if (FR_OK == status)
    {
      status = f_open (&file, filename, FA_OPEN_APPEND | FA_READ | FA_WRITE);
      if (FR_OK == status)
        {
          /* write the string to the file */
          f_puts (content, &file);
          f_close (&file);
        }
      else
        {
          printf ("Cannot open file (errno: %d)\n\r", status);
        }
      f_mount (NULL, "", 0); // unmount the file system
    }
  else
    {
      printf ("Card not mounted (errno: %d)\n\r", status);
    }

  return status;
}

/**
 * \brief  Load the program (binary) into the specified memory location.
 *
 * This function will read the binary and load it into the specific memory
 * location.
 *
 * \param[in] filename the filename to write to
 * \param[in] content the location to write to
 * \retval FR_OK in case of sucess; else any of the #FRESULT.
 */
FRESULT
load_progam (const char *filename, uint8_t *content)
{
  FATFS filesystem;       // file system
  FRESULT status = FR_OK; // flag to check if the card is enabled
  FIL file;

  status = f_mount (&filesystem, "0:", 1); // mount the file system
  if (FR_OK == status)
    {
      status = f_open (&file, filename, FA_OPEN_EXISTING | FA_READ);
      if (FR_OK == status)
        {
          uint16_t buffer_size = 1024;
          uint16_t br;
          do
            {
              f_read (&file, content, buffer_size, &br);
            }
          while (*br < content);
        }
      else
        {
          printf ("Cannot open file (errno: %d)\n\r", status);
        }
      f_close (&file);
      f_mount (NULL, "", 0); // unmount the file system
    }
  else
    {
      printf ("Card not mounted (errno: %d)\n\r", status);
    }

  return status;
}