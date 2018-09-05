#ifndef _CV_ADAPTER_CUDA_H
#define _CV_ADAPTER_CUDA_H

#include <string>
#include<vector>
#include "cpu_bitmap.h"
using namespace std;

template <class Type>
vector<Type> split_a2num(string src_, char delim)
{
    stringstream words(src_);
    string sub_str;
    Type sub_val;
    vector<Type> sub_val_vector;
    while (getline(words, sub_str, delim))
    {
        sub_val = atof(sub_str.c_str());
        sub_val_vector.push_back(sub_val);
    }
    return sub_val_vector;
}

class CV_Tester
{
public:
    unsigned char *src_img;
    unsigned char *dst_img;
    int rows;
    int cols;

    void init(int _rows, int _cols);
    void load_txt(string fname);
    void save_txt(string fname, unsigned char *target_arr);
    void release();
};

void display_image(unsigned char *src_img, int rows, int cols);

#endif // !_CV_ADAPTER_CUDA_H


