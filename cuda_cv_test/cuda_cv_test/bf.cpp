cv::Mat BilateralFilter(cv::Mat inputImg, int filterSize, double sigmaD, double sigmaR) {
    int len; //must be odd number
    cv::Mat gray; // must be 1-channel image
    cv::Mat LabImage; // if channels == 3

    if (filterSize % 2 != 1 || filterSize <= 0) {
        std::cerr << "Filter Size must be a positive odd number!" << std::endl;
        return inputImg;
    }
    len = filterSize / 2;

    if (inputImg.channels() >= 3) {
        cv::cvtColor(inputImg, LabImage, cv::COLOR_BGR2Lab);
        gray = cv::Mat::zeros(LabImage.size(), CV_8UC1);
        for (int i = 0; i < LabImage.rows; i++) {
            for (int j = 0; j < LabImage.cols; j++) {
                gray.ptr<uchar>(i)[j] = LabImage.ptr<uchar>(i, j)[0];
            }
        }
    }
    else if(inputImg.channels() == 1){
        inputImg.copyTo(gray);
    }
    else {
        std::cerr << "the count of input image's channel can not be 2!" << std::endl;
        return inputImg;
    }

    cv::Mat resultGrayImg = cv::Mat::zeros(gray.size(), CV_8UC1);
    for (int i = 0; i < gray.rows; i++) {
        for (int j = 0; j < gray.cols; j++) {
            double k = 0;
            double f = 0;
            for (int r = i - len; r <= i + len; r++) {
                for (int c = j - len; c <= j + len; c++) {
                    if (r < 0 || c < 0 || r >= gray.rows || c >= gray.cols)
                        continue;
                    f = f + gray.ptr<uchar>(r)[c] * SpaceFactor(i, j, r, c, sigmaD) * ColorFactor(gray.ptr<uchar>(i)[j], gray.ptr<uchar>(r)[c], sigmaD);
                    k += SpaceFactor(i, j, r, c, sigmaD) * ColorFactor(gray.ptr<uchar>(i)[j], gray.ptr<uchar>(r)[c], sigmaD);
                }
            }
            int value = f / k;
            if (value < 0) value = 0;
            else if (value > 255) value = 255;

            resultGrayImg.ptr<uchar>(i)[j] = (uchar)value;
        }
    }

    cv::Mat resultImg;
    if (inputImg.channels() >= 3) {
        for (int i = 0; i < LabImage.rows; i++) {
            for (int j = 0; j < LabImage.cols; j++) {
                LabImage.ptr<uchar>(i, j)[0] = resultGrayImg.ptr<uchar>(i)[j];
            }
        }
        cv::cvtColor(LabImage, resultImg, cv::COLOR_Lab2BGR);
    }
    else {
        resultGrayImg.copyTo(resultImg);
    }

    return resultImg;
}