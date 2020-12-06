#include "sdcard_stuff.h"
#include "nvs_storage_stuff.h"
#include "img_low_bat.h"
#include "qdbmp.h"
#include "floyd_steinberg.h"
#include "pixel_converter.h"
#include "epd5in65f.h"
#include "lis3dh.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_int_wdt.h"
#include "esp_task_wdt.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"


#define CONFIG_SLEEP_SECONDS 10

//integer values
#define NVS_FLASH_KEY_BATTERY_EMPTY_FLAG "BAT_EMPTY"  //0: false, 1: true, default: 0
#define NVS_FLASH_KEY_HORIZONTAL_IMAGE_COUNTER "HORI_IMG"  //stores last displayed image in horizontal mode, default: 0
#define NVS_FLASH_KEY_VERTICAL_IMAGE_COUNTER "VERT_IMG"  //stores last displayed image in vertical mode, default: 0
#define NVS_FLASH_KEY_LAST_ORIENTATION "LAST_ORIN"  //states defined in lis3dh.h, default: LIS3DH_ORIENTATION_INCONCLUSIVE

void pictureframe_deepsleep_init();

void shutdown_pictureframe(void);

unsigned int manage_orientation();

void display_empty_folder_picture();

unsigned char* epdbuf;

void display_bmp(unsigned char* path);

void display_next_horizontal();

void display_next_vertical();

void display_all_bmps(filename_list_t* list);

void display_random_bmp(filename_list_t* list);

void epaper(void *arg);

void accelerometer(void *arg);

void test(void *arg);








