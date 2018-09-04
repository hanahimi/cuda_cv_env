#ifndef _CV_ADAPTER_CUDA_H
#define _CV_ADAPTER_CUDA_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <stdio.h>
#include<sstream>
#include<fstream>
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

	void init(int _rows, int _cols)
	{
		rows = _rows;
		cols = _cols;
		src_img = (unsigned char *)malloc(rows * cols * 3 * sizeof(unsigned char));
		dst_img = (unsigned char *)malloc(rows * cols * 3 * sizeof(unsigned char));
	};

	void load_txt(string fname)
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

	void save_txt(string fname, unsigned char *target_arr)
	{
		FILE *fp = fopen(fname.c_str(), "w");
		for (int k = 0; k < 3; k++)
		{
			for (int r = 0; r < rows; r++)
			{
				for (int c = 0; c < cols-1; c++)
				{
					fprintf(fp, "%d ", target_arr[k*rows*cols + r * cols + c]);
				}
				fprintf(fp, "%d\n", target_arr[k*rows*cols + r * cols + cols-1]);
			}
		}
		fclose(fp);
	}

	void release()
	{
		free(src_img);
		free(dst_img);
	};
};

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

#endif // !_CV_ADAPTER_CUDA_H


