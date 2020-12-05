#include "sdcard_stuff.h"
#include "nvs_storage_stuff.h"
#include "img_low_bat.h"
#include "qdbmp.h"
#include "floyd_steinberg.h"
#include "pixel_converter.h"
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

#include "epd5in65f.h"

#include "lis3dh.h"


//#define CONFIG_SLEEP_SECONDS 86400//60s * 60min * 24h
#define CONFIG_SLEEP_SECONDS 10

void pitureframe_deepsleep_init(){
	esp_sleep_enable_timer_wakeup(1000000 * CONFIG_SLEEP_SECONDS);
	esp_sleep_enable_ext1_wakeup((((uint64_t)1)<<35), ESP_EXT1_WAKEUP_ANY_HIGH);
}

void shutdown_pictureframe(void){
	Suspend();
	sdcard_deinit();
	pitureframe_deepsleep_init();
	puts("going to sleep in 1 second...");
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	esp_deep_sleep_start();
}

//integer values
#define NVS_FLASH_KEY_BATTERY_EMPTY_FLAG "BAT_EMPTY"  //0: false, 1: true, default: 0
#define NVS_FLASH_KEY_HORIZONTAL_IMAGE_COUNTER "HORI_IMG"  //stores last displayed image in horizontal mode, default: 0
#define NVS_FLASH_KEY_VERTICAL_IMAGE_COUNTER "VERT_IMG"  //stores last displayed image in vertical mode, default: 0
#define NVS_FLASH_KEY_LAST_ORIENTATION "LAST_ORIN"  //0: horizontal, 1: vertical, default: 0

void manage_wakeup_reason(){
	esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
	switch(cause){
		case ESP_SLEEP_WAKEUP_UNDEFINED:{
			puts("first boot");
			nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_BATTERY_EMPTY_FLAG, 0);
			nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_HORIZONTAL_IMAGE_COUNTER, 0);
			nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_VERTICAL_IMAGE_COUNTER, 0);
			nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_LAST_ORIENTATION, 0);
			break;
		}
		case ESP_SLEEP_WAKEUP_TIMER:{
			puts("timer wakeup");
			break;
		}
		case ESP_SLEEP_WAKEUP_EXT1:{
			puts("interrupt wakeup");
			break;
		}
		default:{
			puts("ERROR: unexpected wakeup cause");
			printf("wakeup value: %d\n", (unsigned int)cause);
			break;
		}
	};
}



unsigned char* epdbuf;

void display_all_bmps(filename_list_t* list){
	filename_list_t* copy = list;
	while(!(copy == NULL)){
		//puts(copy->filename);
		char path[550];
		strcpy(path, "/sdcard/");
		strcat(path, copy->filename);

		printf("going to display \"%s\"\n", copy->filename);
		BMP* bitmap = BMP_ReadFile(path);
		//printf("opened bitmap\n");
		//printf("going to read error description\n");
		const char* bmp_err = BMP_GetErrorDescription();
		printf(bmp_err == NULL ? "" : bmp_err);
		//printf("\n\n");
		if(bmp_err != NULL){
			puts("ERROR while reading BMP, continuing...");
			copy = copy->next;
			continue;
		}
		//printf("done reading error description\nStarting BMPtoBUF function now\n");
		unsigned char* pixelbuf = BMPtoBUF(bitmap);
		//printf("done with BMPtoBUF\nStarting floyd steinberg dither() now\n");
		dither(pixelbuf);
		//printf("done with dithering\nconverting to epd colors...\n");
		epdbuf = imgtoepd(pixelbuf);
		//printf("done with converting\ndisplaying...\n\n\n\n");
		EPD_5IN65F_Display((const unsigned char*) epdbuf);

		BMP_Free(bitmap);
		free(pixelbuf);
		free(epdbuf);

		vTaskDelay(10000 / portTICK_PERIOD_MS);
		copy = copy->next;
	}
}

void display_random_bmp(filename_list_t* list){
	printf("DEBUG -- Starting to decode Image ");
	uint32_t temp_time_buf = esp_log_timestamp();
	filename_list_t* copy = list;
	unsigned int num_files = 0;
	while(!(copy == NULL)) {
		copy = copy->next;
		num_files++;
	}

	copy = list;
	unsigned int randomimage = esp_random() % num_files;
	for(int i = 0; i < randomimage; i++) {
		copy = copy->next;
	}
	puts(copy->filename);
	char path[550];
	strcpy(path, "/sdcard/");
	strcat(path, copy->filename);

	//printf("going to display \"%s\"\n", copy->filename);
	BMP* bitmap = BMP_ReadFile(path);
	const char* bmp_err = BMP_GetErrorDescription();
	printf(bmp_err == NULL ? "" : bmp_err);
	if(bmp_err != NULL){
		puts("ERROR while reading BMP, aborting ...");
		BMP_Free(bitmap);
		free(epdbuf);
		return;
	}
	unsigned char* pixelbuf = BMPtoBUF(bitmap);
	dither(pixelbuf);
	epdbuf = imgtoepd(pixelbuf);

	printf("DEBUG -- conversion took %d ms. calling ePaper Display function\n", esp_log_timestamp() - temp_time_buf);
	temp_time_buf = esp_log_timestamp();

	//EPD_5IN65F_Display((const unsigned char*) epdbuf);
	EPD_5IN65F_DisplayFast((const unsigned char*) epdbuf);
	printf("DEBUG -- ePaper Draw Function returned, took %d ms\n", esp_log_timestamp() - temp_time_buf);
	BMP_Free(bitmap);
	free(pixelbuf);
	free(epdbuf);
}

void epaper(void *arg){
	if(0 != Init()) printf("EPAPER init failed\n");

	vTaskDelay(100 / portTICK_PERIOD_MS);
	filename_list_t* filename_list = list_files("/sdcard", "bmp");
	display_random_bmp(filename_list);
	shutdown_pictureframe();

	for(;;){}
}

void accelerometer(void *arg){

	//puts("executing lis3dh_init now...");
	lis3dh_init();
	//puts("done with lis3dh_init");
	vTaskDelay(10 / portTICK_PERIOD_MS);
	float x_buf = 0;
	float y_buf = 0;
	float z_buf = 0;
	lis3dh_readAccelerationFloat(&x_buf, &y_buf, &z_buf);
	float g_mag = sqrtf((x_buf) * (x_buf) + (y_buf) * (y_buf) + (z_buf) * (z_buf));
	printf("accelerometer readings %f\nx: %f\ny: %f\nz: %f\n", g_mag, x_buf, y_buf, z_buf);

	switch(lis3dh_readOrientation()){
		case LIS3DH_ORIENTATION_HORIZONTAL:{
			puts("orientation: horizontal");
		}break;
		case LIS3DH_ORIENTATION_VERTICAL:{
			puts("orientation: vertical");
		}break;
		case LIS3DH_ORIENTATION_FLAT:{
			puts("orientation: flat");
		}break;
		case LIS3DH_ORIENTATION_INCONCLUSIVE:{
			puts("orientation unknown");
		}
	};
	for(;;){
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

void test(void *arg){
	if(0 != Init()) printf("EPAPER init failed\n");
	for(;;){
		EPD_5IN65F_Show7Block();
		vTaskDelay(10000 / portTICK_PERIOD_MS);
	}
}

void app_main(void)
{

	esp_task_wdt_init(20, false);

	sdcard_init();

	nvs_flash_stuff_init_func();

	manage_wakeup_reason();

	nvs_flash_stuff_writeInt((unsigned char*)"testKey", -5);

	printf("read %d from NVS flash storage\n", nvs_flash_stuff_readInt((unsigned char*)"testKey"));

	nvs_flash_stuff_deleteValueByKey((unsigned char*)"testKey");

	printf("read %d from NVS flash storage\n", nvs_flash_stuff_readInt((unsigned char*)"testKey"));

	xTaskCreate(epaper, "ePaper", 8192, NULL, 2, NULL);
	xTaskCreate(accelerometer, "accelerometer", 8192, NULL, 2, NULL);
	//xTaskCreate(test, "test", 8192, NULL, 2, NULL);
	//xTaskCreate(bitmap, "bitmap", 8192, NULL, 2, NULL);

	while(1){
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}
