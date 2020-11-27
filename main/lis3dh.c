#include "lis3dh.h"
#include "lis3dh_drv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lis3dh_init(void){
	lis3dh_drv_init();
}

void lis3dh_test(void){
	printf("read %d from WHO_AM_I register \n", lis3dh_drv_read(WHO_AM_I));
}
