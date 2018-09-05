#include "cu_cvf.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>

#define DIM 32

__host__ __device__ __forceinline__ 
int divUp(int total, int grain)		// 向上取整除法
{
	return (total + grain - 1) / grain;
}

__host__ __device__ __forceinline__ 
unsigned char saturate_cast(int v) // 转换为0~255
{
	unsigned char r;
	if (v > 255){ r = 255; }
	else if (v < 0){ r = 0; }
	else { r = (unsigned char)v; }
	return r;
}

__host__ __device__ __forceinline__
int clip(int x, int v_min, int v_max)
{
	int r = 0;
	r = (x >= v_min) ? x : v_min;
	r = (r <= v_max) ? r : v_max;
	return r;
}

__device__ __forceinline__ 
float norm_l1(const float& a)  { return fabs(a); }


void CuBilateralFilter::init(int _img_width, int _img_heigth, int _img_channels)
{
	this->img_cols = _img_width;
	this->img_rows = _img_heigth;
	this->img_channels = _img_channels;
	this->img_size = _img_width * _img_heigth * _img_channels;
	cudaMalloc( (void**)&dev_input_img, this->img_size * sizeof(unsigned char) );
    cudaMalloc( (void**)&dev_result_img, this->img_size * sizeof(unsigned char) );
}

void CuBilateralFilter::release()
{
	cudaFree( dev_input_img );
    cudaFree( dev_result_img );
}

__global__ 
void bilateral_kernel(unsigned char *src, unsigned char *dst,
					int cols, int rows, const int ksz, const float sigma_spatial2_inv_half, const float sigma_color2_inv_half)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if (x >= cols || y >= rows)
		return;
	int r = ksz / 2;
	float r2 = (float)(r * r);
	int offset = y * cols + x;

	//bgr
	float center[3] = { src[offset], src[cols * rows + offset], src[2 * cols * rows + offset] };
	float value[3] = { 0, 0, 0 };
	float weight[3] = { 0, 0, 0 };
	float sum1[3] = {0,0,0};
	float sum2[3] = { 0, 0, 0 };

	int x_tmp, y_tmp, tmp_offset;
	float space2;
	if (x >= 0 && y >= 0 && x < cols && y < rows)
	{
		for (int cy = -r; cy <= r; cy++)
		{
			for (int cx = -r; cx <=r; cx++)
			{
				space2 = cx*cx + cy*cy;
				if (space2 > r2) continue;
				x_tmp = clip(x + cx, 0, cols - 1);
				y_tmp = clip(y + cy, 0, rows - 1);
				tmp_offset = y_tmp * cols + x_tmp;
				for (int k = 0; k < 3; k++)
				{
					value[k] = (float)src[tmp_offset + cols * rows * k];
					weight[k] = expf(space2 * sigma_spatial2_inv_half + sqrtf(norm_l1(value[k] - center[k])) * sigma_color2_inv_half);
					sum1[k] = sum1[k] + weight[k] * value[k];
					sum2[k] = sum2[k] + weight[k];
				}
			}
		}
	}

	//__syncthreads();
	for (int k = 0; k < 3; k++)
	{
		dst[offset + cols * rows * k] = saturate_cast((int)(sum1[k] / sum2[k]));
	}
}

void CuBilateralFilter::bilateral_caller(unsigned char *host_src_img, unsigned char *host_dst_img, int kernel_size, float sigma_spatial, float sigma_color)
{
	dim3 block(32, 32);
	dim3 grid(divUp(this->img_cols, block.x), divUp(this->img_rows, block.y));

	float sigma_spatial2_inv_half = -0.5f / (sigma_spatial * sigma_spatial);
	float sigma_color2_inv_half = -0.5f / (sigma_color * sigma_color);

	cudaMemcpy(dev_input_img, host_src_img,
			this->img_size * sizeof(unsigned char), cudaMemcpyHostToDevice );
	
	//cudaDeviceSynchronize();
	bilateral_kernel << <grid, block >> >(dev_input_img, dev_result_img, this->img_cols, this->img_rows,
								kernel_size, sigma_spatial2_inv_half, sigma_color2_inv_half);
	//cudaDeviceSynchronize();

	cudaMemcpy(host_dst_img, dev_result_img,
		this->img_size * sizeof(unsigned char), cudaMemcpyDeviceToHost);

}

