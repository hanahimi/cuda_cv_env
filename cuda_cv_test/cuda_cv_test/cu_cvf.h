#ifndef _CU_CVF_H
#define _CU_CVF_H

class CuBilateralFilter
{
private:
	int img_size;
	unsigned char *dev_input_img;
	unsigned char *dev_result_img;
public:
	int img_rows;
	int img_cols;
	int img_channels;

	void init(int img_width, int img_heigth, int img_channels);
	void bilateral_caller(unsigned char *host_src_img, unsigned char *host_dst_img, int filterSize, float sigmaD, float sigmaR);
	void release();
};


#endif