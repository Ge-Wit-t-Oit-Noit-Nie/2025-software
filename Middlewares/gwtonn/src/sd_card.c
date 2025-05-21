#include <stdio.h>
#include "sd_card.h"

FRESULT
write_file (const char *filename, const char *content)
{
  FATFS filesystem;             // file system
  FRESULT status = FR_OK; // flag to check if the card is enabled
  FIL file;

  status = f_mount (&filesystem, "0:", 1); // mount the file system
  if (FR_OK == status)
    {
	status = f_open (&file, filename, FA_OPEN_APPEND | FA_READ | FA_WRITE);
      if (FR_OK
          == status)
        {
          /* write the string to the file */
          f_puts (content, &file);
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

FRESULT
read_file (const char *filename, uint8_t *content)
{
  FATFS filesystem;             // file system
  FRESULT status = FR_OK; // flag to check if the card is enabled
  FIL file;

  status = f_mount (&filesystem, "0:", 1); // mount the file system
  if (FR_OK == status)
    {
	status = f_open (&file, filename, FA_OPEN_EXISTING | FA_READ);
      if (FR_OK
          == status)
        {
			uint16_t br;
          f_read (&file, content, 10, &br);
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