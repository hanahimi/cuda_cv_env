#ifndef _CU_CVF_H
#define _CU_CVF_H

class CuBilateralFilter
{
private:
    unsigned char *_dev_input_img;
    unsigned char *_dev_result_img;
    int _img_rows;
    int _img_cols;
    int _img_channels;
    int _img_size;

public:

    void init(int img_width, int img_heigth, int img_channels);
    void bilateral_caller(unsigned char *host_src_img, 
                          unsigned char *host_dst_img, 
                          int kernel_size,
                          float sigma_spatial,
                          float sigma_color);
    void release();
};


#endif