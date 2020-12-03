#include "lis3dh.h"
#include "lis3dh_drv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void lis3dh_init(void){
	lis3dh_drv_init();  //initializes I2C interface
	lis3dh_drv_write(CTRL_REG1, ODR1 | Zen | Yen | Xen);  //enable normal mode with all three axis enabled
	lis3dh_drv_write(CTRL_REG2, HPM1);
	lis3dh_drv_write(CTRL_REG3, I1_IA1);
	lis3dh_drv_write(CTRL_REG4, 0x00);
	lis3dh_drv_write(CTRL_REG5, 0x00);
	lis3dh_drv_write(CTRL_REG6, 0x00);  //???currently, no interrupt is used
	lis3dh_drv_write(REFERENCE, 0x00);
	lis3dh_drv_write(INT1_THS, 0x32);  //interrupt threshold is 0x32 or dec 50 (50 * 0.016g = 0.8g)
	lis3dh_drv_write(INT1_DURATION, 0x00);
	lis3dh_drv_write(INT1_CFG, AOI | _6D | XLIE | YLIE);  //interrupt on direction x low and y low
	lis3dh_drv_write(CTRL_REG5, BOOT);
}

unsigned int lis3dh_readAccelerationInt(int* x_buf, int* y_buf, int* z_buf){
	unsigned char stat_reg_read = lis3dh_drv_read(STATUS_REG);
	unsigned int ret_val = (stat_reg_read & ZYXDA) ? LIS3DH_OK : LIS3DH_NO_NEW_DATA;
	unsigned char outxl = lis3dh_drv_read(OUT_X_L);
	unsigned char outxh = lis3dh_drv_read(OUT_X_H);
	unsigned char outyl = lis3dh_drv_read(OUT_Y_L);
	unsigned char outyh = lis3dh_drv_read(OUT_Y_H);
	unsigned char outzl = lis3dh_drv_read(OUT_Z_L);
	unsigned char outzh = lis3dh_drv_read(OUT_Z_H);
	x_buf[0] = (outxh<<2) | (outxl>>6) | ((outxh & 0x80) ? 0xFFFFFC00 : 0x00000000);
	y_buf[0] = (outyh<<2) | (outyl>>6) | ((outyh & 0x80) ? 0xFFFFFC00 : 0x00000000);
	z_buf[0] = (outzh<<2) | (outzl>>6) | ((outzh & 0x80) ? 0xFFFFFC00 : 0x00000000);
	return ret_val;
}

unsigned int lis3dh_readAccelerationFloat(float* x_buf, float* y_buf, float* z_buf){
	int x, y, z;
	unsigned char stat_reg_read = lis3dh_drv_read(STATUS_REG);
	unsigned int ret_val = (stat_reg_read & ZYXDA) ? LIS3DH_OK : LIS3DH_NO_NEW_DATA;
	unsigned char outxl = lis3dh_drv_read(OUT_X_L);
	unsigned char outxh = lis3dh_drv_read(OUT_X_H);
	unsigned char outyl = lis3dh_drv_read(OUT_Y_L);
	unsigned char outyh = lis3dh_drv_read(OUT_Y_H);
	unsigned char outzl = lis3dh_drv_read(OUT_Z_L);
	unsigned char outzh = lis3dh_drv_read(OUT_Z_H);
	x = (outxh<<2) | (outxl>>6) | ((outxh & 0x80) ? 0xFFFFFC00 : 0x00000000);
	y = (outyh<<2) | (outyl>>6) | ((outyh & 0x80) ? 0xFFFFFC00 : 0x00000000);
	z = (outzh<<2) | (outzl>>6) | ((outzh & 0x80) ? 0xFFFFFC00 : 0x00000000);
	x_buf[0] = (2.0 * x)/511;
	y_buf[0] = (2.0 * y)/511;
	z_buf[0] = (2.0 * z)/511;
	return ret_val;
}

void lis3dh_test(void){
	printf("read %d from WHO_AM_I register \n", lis3dh_drv_read(WHO_AM_I));
}

unsigned char* chrtostr(unsigned char data){
	unsigned char* bf = (unsigned char*) malloc(11 * sizeof(unsigned char));
	bf[0] = '0';
	bf[1] = 'b';
	bf[10] = '\0';
	for(int i = 0; i < 8; i++){
		bf[i + 2] = (data & (1<<(7-i))) ? '1' : '0';
	}
	return bf;
}
