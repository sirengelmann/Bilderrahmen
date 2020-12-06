#include "sdcard_stuff.h"
#include "nvs_storage_stuff.h"
#include "img_low_bat.h"
#include "qdbmp.h"
#include "floyd_steinberg.h"
#include "pixel_converter.h"
#include "epd5in65f.h"
#include "lis3dh.h"
#include "pictureframe_management_functions.h"

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


void app_main(void)
{
	esp_task_wdt_init(20, false);
	sdcard_init();
	nvs_flash_stuff_init_func();
	lis3dh_init();
	if(0 != Init()) printf("EPAPER init failed\n");

	esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
	switch(cause){
		case ESP_SLEEP_WAKEUP_UNDEFINED:{
			puts("first boot");
			nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_BATTERY_EMPTY_FLAG, 0);
			nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_HORIZONTAL_IMAGE_COUNTER, 0);
			nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_VERTICAL_IMAGE_COUNTER, 0);
			nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_LAST_ORIENTATION, LIS3DH_ORIENTATION_INCONCLUSIVE);
			unsigned int ori_current = manage_orientation();
			switch(ori_current){
				case LIS3DH_ORIENTATION_HORIZONTAL:{
					display_next_horizontal();
				}break;
				case LIS3DH_ORIENTATION_VERTICAL:{
					display_next_vertical();
				}break;
			};
			break;
		}
		case ESP_SLEEP_WAKEUP_TIMER:{
			puts("timer wakeup");
			unsigned int ori_current = manage_orientation();
			switch(ori_current){
				case LIS3DH_ORIENTATION_HORIZONTAL:{
					display_next_horizontal();
				}break;
				case LIS3DH_ORIENTATION_VERTICAL:{
					display_next_vertical();
				}break;
			};
		} break;
		case ESP_SLEEP_WAKEUP_EXT1:{
			puts("interrupt wakeup");
			unsigned int ori_current = manage_orientation();
			switch(ori_current){
				case LIS3DH_ORIENTATION_HORIZONTAL:{
					display_next_horizontal();
				}break;
				case LIS3DH_ORIENTATION_VERTICAL:{
					display_next_vertical();
				}break;
			};
		} break;
		default:{
			puts("ERROR: unexpected wakeup cause");
			printf("wakeup value: %d\n", (unsigned int)cause);
		} break;
	};

	shutdown_pictureframe();

	while(1){
		//should never arrive here
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}
