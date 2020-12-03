void lis3dh_init(void);

void lis3dh_test(void);

unsigned int lis3dh_readAccelerationInt(int* x_buf, int* y_buf, int* z_buf);

unsigned int lis3dh_readAccelerationFloat(float* x_buf, float* y_buf, float* z_buf);

unsigned char* chrtostr(unsigned char data);
