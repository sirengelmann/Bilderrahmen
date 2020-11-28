/* SPI Master example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "sdcard_stuff.h"
//#include "i2c_stuff.h"

#include "imagedata000.h"
#include "imagedata001.h"
#include "imagedata002.h"
#include "imagedata003.h"
#include "img_low_bat.h"
#include "qdbmp.h"
#include "floyd_steinberg.h"
#include "pixel_converter.h"
#include <stdio.h>
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

void app_main(void)
{
	esp_task_wdt_init(20, false);

	sdcard_init();

	//puts("executing lis3dh_init now...");
	lis3dh_init();
	//puts("done with lis3dh_init");

	//puts("reading who_am_i register now...");
	lis3dh_test();
	//puts("done reading register");

	xTaskCreate(epaper, "ePaper", 8192, NULL, 2, NULL);
	//xTaskCreate(bitmap, "bitmap", 8192, NULL, 2, NULL);

	while(1){
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}




