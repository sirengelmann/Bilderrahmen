#ifndef __LIS3DH_DRV_H__
#define __LIS3DH_DRV_H__

//---------------------------------
//		ERROR VALUES DEFINES
//---------------------------------
#define LIS3DH_OK 0x00
#define LIS3DH_NO_NEW_DATA 0x01

//---------------------------------
//		REGISTER DEFINES
//---------------------------------

#define STATUS_REG_AUX 0x07
#define _321OR (1<<7)
#define _3OR (1<<6)
#define _2OR (1<<5)
#define _1OR (1<<4)
#define _321DA (1<<3)
#define _3DA (1<<2)
#define _2DA (1<<1)
#define _1DA 1

#define OUT_ADC1_L 0x08

#define OUT_ADC1_H 0x09

#define OUT_ADC2_L 0x0A

#define OUT_ADC2_H 0x0B

#define OUT_ADC3_L 0x0C

#define OUT_ADC3_H 0x0D

#define WHO_AM_I 0x0F

#define CTRL_REG0 0x1E
#define SDO_PU_DISC (1<<7)

#define TEMP_CFG_REG 0x1F
#define TEMP_EN (1<<7)
#define ADC_EN (1<<6)

#define CTRL_REG1 0x20
#define ODR3 (1<<7)
#define ODR2 (1<<6)
#define ODR1 (1<<5)
#define ODR0 (1<<4)
#define LPen (1<<3)
#define Zen (1<<2)
#define Yen (1<<1)
#define Xen 1

#define CTRL_REG2 0x21
#define HPM1 (1<<7)
#define HPM0 (1<<6)
#define HPCF2 (1<<5)
#define HPCF1 (1<<4)
#define FDS (1<<3)
#define HPCLICK (1<<2)
#define HP_IA2 (1<<1)
#define HP_IA1 1

#define CTRL_REG3 0x22
#define I1_CLICK (1<<7)
#define I1_IA1 (1<<6)
#define I1_IA2 (1<<5)
#define I1_ZYXDA (1<<4)
#define I1_321DA (1<<3)
#define I1_WTM (1<<2)
#define I1_OVERRUN (1<<1)

#define CTRL_REG4 0x23
#define BDU (1<<7)
#define BLE (1<<6)
#define FS1 (1<<5)
#define FS0 (1<<4)
#define HR (1<<3)
#define ST1 (1<<2)
#define ST0 (1<<1)
#define SIM 1

#define CTRL_REG5 0x24
#define BOOT (1<<7)
#define FIFO_EN (1<<6)
#define LIR_INT1 (1<<3)
#define D4D_INT1 (1<<2)
#define LIR_INT2 (1<<1)
#define D4D_INT2 1

#define CTRL_REG6 0x25
#define I2_CLICK (1<<7)
#define I2_IA1 (1<<6)
#define I2_IA2 (1<<5)
#define I2_BOOT (1<<4)
#define I2_ACT (1<<3)
#define INT_POLARITY (1<<1)

#define REFERENCE 0x26
#define Ref7 (1<<7)
#define Ref6 (1<<6)
#define Ref5 (1<<5)
#define Ref4 (1<<4)
#define Ref3 (1<<3)
#define Ref2 (1<<2)
#define Ref1 (1<<1)
#define Ref0 1

#define STATUS_REG 0x27
#define ZYXOR (1<<7)
#define ZOR (1<<6)
#define YOR (1<<5)
#define XOR (1<<4)
#define ZYXDA (1<<3)
#define ZDA (1<2)
#define YDA (1<1)
#define XDA 1

#define OUT_X_L 0x28

#define OUT_X_H 0x29

#define OUT_Y_L 0x2A

#define OUT_Y_H 0x2B

#define OUT_Z_L 0x2C

#define OUT_Z_H 0x2D

#define FIFO_CTRL_REG 0x2E
#define FM1 (1<<7)
#define FM0 (1<<6)
#define TR (1<<5)
#define FTH4 (1<<4)
#define FTH3 (1<<3)
#define FTH2 (1<<2)
#define FTH1 (1<<1)
#define FTH0 1

#define FIFO_SRC_REG 0x2F
#define WTM (1<<7)
#define OVRN_FIFO (1<<6)
#define EMPTY (1<<5)
#define FSS4 (1<<4)
#define FSS3 (1<<3)
#define FSS2 (1<<2)
#define FSS1 (1<<1)
#define FSS0 1

#define INT1_CFG 0x30
#define AOI (1<<7)
#define _6D (1<<6)
#define ZHIE (1<<5)
#define ZLIE (1<<4)
#define YHIE (1<<3)
#define YLIE (1<<2)
#define XHIE (1<<1)
#define XLIE 1

#define INT1_SRC 0x31
#define IA (1<<6)
#define ZH (1<<5)
#define ZL (1<<4)
#define YH (1<<3)
#define YL (1<<2)
#define XH (1<<1)
#define XL 1

#define INT1_THS 0x32
#define THS6 (1<<6)
#define THS5 (1<<5)
#define THS4 (1<<4)
#define THS3 (1<<3)
#define THS2 (1<<2)
#define THS1 (1<<1)
#define THS0 1

#define INT1_DURATION 0x33
#define D6 (1<<6)
#define D5 (1<<5)
#define D4 (1<<4)
#define D3 (1<<3)
#define D2 (1<<2)
#define D1 (1<<1)
#define D0 1

#define INT2_CFG 0x34

#define INT2_SRC 0x35

#define INT2_THS 0x36

#define INT2_DURATION 0x37

#define CLICK_CFG 0x38
#define ZD (1<<5)
#define ZS (1<<4)
#define YD (1<<3)
#define YS (1<<2)
#define XD (1<<1)
#define XS 1

#define CLICK_SRC 0x39
#define IA (1<<6)
#define DCLICK (1<<5)
#define SCLICK (1<<4)
#define Sign (1<<3)
#define Z (1<<2)
#define Y (1<<1)
#define X 1

#define CLICK_THS 0x3A
#define LIR_Click (1<<7)
#define Ths6 (1<<6)
#define Ths5 (1<<5)
#define Ths4 (1<<4)
#define Ths3 (1<<3)
#define Ths2 (1<<2)
#define Ths1 (1<<1)
#define Ths0 1

#define TIME_LIMIT 0x3B
#define TLI6 (1<<6)
#define TLI5 (1<<5)
#define TLI4 (1<<4)
#define TLI3 (1<<3)
#define TLI2 (1<<2)
#define TLI1 (1<<1)
#define TLI0 1

#define TIME_LATENCY 0x3C
#define TLA7 (1<<7)
#define TLA6 (1<<6)
#define TLA5 (1<<5)
#define TLA4 (1<<4)
#define TLA3 (1<<3)
#define TLA2 (1<<2)
#define TLA1 (1<<1)
#define TLA0 1

#define TIME_WINDOW 0x3D
#define TW7 (1<<7)
#define TW6 (1<<6)
#define TW5 (1<<5)
#define TW4 (1<<4)
#define TW3 (1<<3)
#define TW2 (1<<2)
#define TW1 (1<<1)
#define TW0 1

#define ACT_THS 0x3E
#define Acth6 (1<<6)
#define Acth5 (1<<5)
#define Acth4 (1<<4)
#define Acth3 (1<<3)
#define Acth2 (1<<2)
#define Acth1 (1<<1)
#define Acth0 1

#define ACT_DUR 0x3F
#define ActD7 (1<<7)
#define ActD6 (1<<6)
#define ActD5 (1<<5)
#define ActD4 (1<<4)
#define ActD3 (1<<3)
#define ActD2 (1<<2)
#define ActD1 (1<<1)
#define ActD0 1

void lis3dh_drv_init(void);

unsigned char lis3dh_drv_read(unsigned char addr);

void lis3dh_drv_write(unsigned char addr, unsigned char data);

void lis3dh_drv_ScanTest(void);

#endif
