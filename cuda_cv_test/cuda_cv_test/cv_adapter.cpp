#include "cv_adapter.h"
#include <stdio.h>
#include <iostream>
#include<sstream>
#include<fstream>
#include "cpu_bitmap.h"
using namespace std;

void CV_Tester::init(int _rows, int _cols)
{
    rows = _rows;
    cols = _cols;
    src_img = (unsigned char *)malloc(rows * cols * 3 * sizeof(unsigned char));
    dst_img = (unsigned char *)malloc(rows * cols * 3 * sizeof(unsigned char));
}

void CV_Tester::load_txt(string fname)
{
    ifstream ifile;				//创建输入文件流对象
    ifile.open(fname.c_str(), ifstream::in);//打开文件
    string line;
    int n_row_cnt = 0;
    while (getline(ifile, line))	//逐行读
    {
        vector<unsigned char> nums = split_a2num<unsigned char>(line, ' ');	// 将当前行使用空格分割
        for (int i = 0; i < cols; i++)
        {
            src_img[i + n_row_cnt * cols] = nums[i];
        }
        n_row_cnt += 1;
    }
    ifile.close();
}

void CV_Tester::save_txt(string fname, unsigned char *target_arr)
{
    FILE *fp = fopen(fname.c_str(), "w");
    for (int k = 0; k < 3; k++)
    {
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols - 1; c++)
            {
                fprintf(fp, "%d ", target_arr[k*rows*cols + r * cols + c]);
            }
            fprintf(fp, "%d\n", target_arr[k*rows*cols + r * cols + cols - 1]);
        }
    }
    fclose(fp);
}

void CV_Tester::release()
{
    free(src_img);
    free(dst_img);
}

// 调用openGL接口进行图像显示
void display_image(unsigned char *src_img, int rows, int cols)
{
    CPUBitmap bitmap(rows, cols);
    unsigned char* pbit_map = bitmap.get_ptr();
    int bit_offset = 0;

    for (int x = 0; x < cols; x++)
    {
        for (int y = 0; y < rows; y++)
        {
            bit_offset = x + y * cols;
            pbit_map[bit_offset * 4 + 2] = src_img[bit_offset];
            pbit_map[bit_offset * 4 + 1] = src_img[bit_offset + rows * cols];
            pbit_map[bit_offset * 4 + 0] = src_img[bit_offset + rows * cols * 2];
        }
    }

    bitmap.display_and_exit();
}

