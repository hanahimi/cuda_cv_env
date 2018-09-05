#include <stdio.h>
#include "cu_cvf.h"
#include "cv_adapter.h"
#include <time.h>

int main()
{
    time_t start_time, end_time;
    CV_Tester cv_tester;
    CuBilateralFilter cu_tool;

	cu_tool.init(512, 512, 3);

    cv_tester.init(512, 512);
    cv_tester.load_txt("../test_data/test.txt");

    start_time = clock();
    cu_tool.bilateral_caller(cv_tester.src_img, cv_tester.dst_img, 7, 31, 31);
    end_time = clock();
    printf("Dev process time %d ms\n", end_time - start_time);

    display_image(cv_tester.dst_img, 512, 512);

    cv_tester.release();
	cu_tool.release();
	//system("pause");
}