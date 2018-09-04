#include <stdio.h>
#include <string>
#include "cu_cvf.h"
#include "cv_adapter.h"
#include <time.h>

int main()
{
	time_t starttime, endtime;
	CV_Tester cv_test;
	CuBilateralFilter cuf;

	cuf.init(512, 512, 3);
	cv_test.init(512, 512);
	cv_test.load_txt("../test_data/test.txt");

	starttime = clock();
	cuf.bilateral_caller(cv_test.src_img, cv_test.dst_img, 7, 31, 31);
	endtime = clock();
	printf("Dev process time %d ms\n", endtime - starttime);


	display_image(cv_test.dst_img, 512, 512);

	cv_test.release();
	cuf.release();

	//system("pause");
}