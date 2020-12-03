#ifndef __NVS_FLASH_STUFF_H__
#define __NVS_FLASH_STUFF_H__
void nvs_flash_init_func(void);

void nvs_flash_writeInt(unsigned char* key, int number);
void nvs_flash_writeString(unsigned char* key, unsigned char* str);

int nvs_flash_readInt(unsigned char* key);
unsigned char* nvs_flash_readString(unsigned char* key, unsigned int length);

#endif
