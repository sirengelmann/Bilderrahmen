#include "pictureframe_management_functions.h"

//#define CONFIG_SLEEP_SECONDS 86400//60s * 60min * 24h


void pictureframe_deepsleep_init(){
	esp_sleep_enable_timer_wakeup(1000000 * CONFIG_SLEEP_SECONDS);
	esp_sleep_enable_ext1_wakeup((((uint64_t)1)<<35), ESP_EXT1_WAKEUP_ANY_HIGH);
}

void shutdown_pictureframe(void){
	Suspend();
	sdcard_deinit();
	pictureframe_deepsleep_init();
	puts("going to sleep in 1 second...");
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	esp_deep_sleep_start();
}

unsigned int manage_orientation(){
	unsigned int ori_current = lis3dh_readOrientation();
	if(ori_current == LIS3DH_ORIENTATION_FLAT
	|| ori_current == LIS3DH_ORIENTATION_INCONCLUSIVE)
	{
		unsigned int ori_old = (unsigned int) nvs_flash_stuff_readInt((unsigned char*)NVS_FLASH_KEY_LAST_ORIENTATION);
		if(ori_old == LIS3DH_ORIENTATION_FLAT
		|| ori_old == LIS3DH_ORIENTATION_INCONCLUSIVE)
		{
			ori_current = LIS3DH_ORIENTATION_HORIZONTAL;
		}
		else {
			ori_current = ori_old;
		}
	}
	nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_LAST_ORIENTATION, ori_current);
	return ori_current;
}

void display_empty_folder_picture(){
	EPD_5IN65F_Show7Block();
}

//unsigned char* epdbuf;  in .h file

void display_bmp(unsigned char* path){
	printf("going to display \"%s\"\n", path);
	BMP* bitmap = BMP_ReadFile((const char*)path);
	const char* bmp_err = BMP_GetErrorDescription();
	printf(bmp_err == NULL ? "" : bmp_err);
	if(bmp_err != NULL){
		puts("ERROR while reading BMP, aborting...");
		BMP_Free(bitmap);
		return;
	}
	unsigned char* pixelbuf = BMPtoBUF(bitmap);
	dither(pixelbuf, BMP_GetHeight(bitmap), BMP_GetWidth(bitmap));
	epdbuf = imgtoepd(pixelbuf, BMP_GetHeight(bitmap), BMP_GetWidth(bitmap));
	EPD_5IN65F_Display((const unsigned char*) epdbuf);

	BMP_Free(bitmap);
	free(pixelbuf);
	free(epdbuf);
}

void display_next_horizontal(){
	filename_list_t* filename_list = list_files("/sdcard/horizontal", "bmp");
	filename_list_t* copy = filename_list;
	unsigned int num_files = 0;
	while(!(copy == NULL)) {
		copy = copy->next;
		num_files++;
	}
	if(num_files == 0){
		puts("no files found in \"/sdcard/horizontal\" folder");
		display_empty_folder_picture();
	}
	else{
		unsigned int last_display = (unsigned int) nvs_flash_stuff_readInt((unsigned char*)NVS_FLASH_KEY_HORIZONTAL_IMAGE_COUNTER);
		unsigned int current_display = (last_display + 1 >= num_files) ? 0 : last_display + 1;
		for(unsigned int i = 0; i < current_display; i++) filename_list = filename_list->next;
		display_bmp((unsigned char*)filename_list->filename);
		last_display = current_display;
		nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_HORIZONTAL_IMAGE_COUNTER, last_display);
	}
}


void display_next_vertical(){
	filename_list_t* filename_list = list_files("/sdcard/vertical", "bmp");
	filename_list_t* copy = filename_list;
	unsigned int num_files = 0;
	while(!(copy == NULL)) {
		copy = copy->next;
		num_files++;
	}
	if(num_files == 0){
		puts("no files found in \"/sdcard/vertical\" folder");
		display_empty_folder_picture();
	}
	else{
		unsigned int last_display = (unsigned int) nvs_flash_stuff_readInt((unsigned char*)NVS_FLASH_KEY_VERTICAL_IMAGE_COUNTER);
		unsigned int current_display = (last_display + 1 >= num_files) ? 0 : last_display + 1;
		for(unsigned int i = 0; i < current_display; i++) filename_list = filename_list->next;
		display_bmp((unsigned char*)filename_list->filename);
		last_display = current_display;
		nvs_flash_stuff_writeInt((unsigned char*)NVS_FLASH_KEY_VERTICAL_IMAGE_COUNTER, last_display);
	}
}

void display_all_bmps(filename_list_t* list){
	filename_list_t* copy = list;
	while(!(copy == NULL)){
		//puts(copy->filename);
		char path[550];
		path[0] = '\0';
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
		dither(pixelbuf, BMP_GetHeight(bitmap), BMP_GetWidth(bitmap));
		//printf("done with dithering\nconverting to epd colors...\n");
		epdbuf = imgtoepd(pixelbuf, BMP_GetHeight(bitmap), BMP_GetWidth(bitmap));
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
	puts("DEBUG -- Starting to decode Image");
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
	path[0] = '\0';
	strcpy(path, copy->filename);

	printf("going to display \"%s\"\n", copy->filename);
	printf("%s\n", path);

	uint32_t temp_time_buf2 = esp_log_timestamp();
	BMP* bitmap = BMP_ReadFile(path);
					printf("BMP_ReadFile took %d ms\n", esp_log_timestamp() - temp_time_buf2);
	const char* bmp_err = BMP_GetErrorDescription();
					printf(bmp_err == NULL ? "" : bmp_err);
	if(bmp_err != NULL){
		puts("ERROR while reading BMP, aborting ...");
		BMP_Free(bitmap);
		free(epdbuf);
		return;
	}
	temp_time_buf2 = esp_log_timestamp();
	unsigned char* pixelbuf = BMPtoBUF(bitmap);
					printf("BMPtoBUF took %d ms\n", esp_log_timestamp() - temp_time_buf2);
	temp_time_buf2 = esp_log_timestamp();
	dither(pixelbuf, BMP_GetHeight(bitmap), BMP_GetWidth(bitmap));
					printf("dither took %d ms\n", esp_log_timestamp() - temp_time_buf2);
	temp_time_buf2 = esp_log_timestamp();
	epdbuf = imgtoepd(pixelbuf, BMP_GetHeight(bitmap), BMP_GetWidth(bitmap));
					printf("imgtoepd took %d ms\n", esp_log_timestamp() - temp_time_buf2);

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
	puts("going to call list_files()");
	filename_list_t* filename_list = list_files("/sdcard/vertical", "bmp");
	if(filename_list == NULL) goto epaper_end;
	puts(filename_list->filename);
	display_random_bmp(filename_list);
	epaper_end:
	shutdown_pictureframe();

	for(;;){
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
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
