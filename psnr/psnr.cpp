#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    cv::Mat ori = cv::imread(argv[1]);
    cv::Mat filtered = cv::imread(argv[2]);
    double mse = 0;
    int height = ori.rows;
    int width = ori.cols;
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {
            mse += pow((ori.at<uchar>(i, j) - filtered.at<uchar>(i, j)), 2);
        }
    }
    mse /= (height * width);
    double psnr = 10 * std::log10(255 * 255 / mse);
    printf("PSNR: %f\n", psnr);
    
    return 0;
}