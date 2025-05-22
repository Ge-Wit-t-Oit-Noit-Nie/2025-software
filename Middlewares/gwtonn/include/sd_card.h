#ifndef __GWTONN_INCLUDE_SD_CARD_H__
#define __GWTONN_INCLUDE_SD_CARD_H__

#include "fatfs.h"

#ifdef __cplusplus
extern "C" {
#endif // End of extern "C" block

FRESULT write_file(const char* filename, const char* content);
FRESULT load_progam(const char* filename, uint8_t* content);

#ifdef __cplusplus
}
#endif // End of extern "C" block

#endif  //__GWTONN_INCLUDE_SD_CARD_H__