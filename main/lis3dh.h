void lis3dh_init(void);

void lis3dh_test(void);

unsigned int lis3dh_readAccelerationInt(int* x_buf, int* y_buf, int* z_buf);

unsigned int lis3dh_readAccelerationFloat(float* x_buf, float* y_buf, float* z_buf);

unsigned char* chrtostr(unsigned char data);

unsigned int lis3dh_readOrientation(void);

float dot_product_3d(float v_a[3], float v_b[3]);

#define LIS3DH_ORIENTATION_VERTICAL 0
#define LIS3DH_ORIENTATION_HORIZONTAL 1
#define LIS3DH_ORIENTATION_FLAT 2
#define LIS3DH_ORIENTATION_INCONCLUSIVE 3
