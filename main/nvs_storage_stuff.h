#ifndef __NVS_FLASH_STUFF_H__
#define __NVS_FLASH_STUFF_H__
void nvs_flash_stuff_init_func(void);

void nvs_flash_stuff_writeInt(unsigned char* key, int number);
void nvs_flash_stuff_writeString(unsigned char* key, unsigned char* str);

int nvs_flash_stuff_readInt(unsigned char* key);
unsigned char* nvs_flash_stuff_readString(unsigned char* key, unsigned int length);

void nvs_flash_stuff_deleteValueByKey(unsigned char* key);

#endif
