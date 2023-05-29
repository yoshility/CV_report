#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;

double max(double a, double b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

void g(cv::Mat src, cv::Mat filtered, int i1, int i2, int d, double sigma, double h) {
    int width = src.cols; // 258(d=3)
    int height = src.rows; // 258
    int start = d/2; // 1
    int end_h = height - d/2 - 1; // 258-2=256
    int end_w = width - d/2 - 1; // 256
    double w = 0;
    double sum = 0;
    double l2norm[height*width] = {0}; // 258*258:真ん中だけ使う
    double Z = 0;
    int range = d/2; // 1
    // Zを求める
    for (int j1=start; j1<=end_h; ++j1) {
        for (int j2=start; j2<=end_w; ++j2) {
            // l2normを求めて保管
            for (int m=-range; m<=range; ++m) {
                for (int n=-range; n<=range; ++n) {
                    l2norm[j1*width+j2] += pow((src.at<uchar>(i1+m, i2+n) - src.at<uchar>(j1+m, j2+n)), 2);
                }
            }
            l2norm[j1*width+j2] /= (d * d);
            // あるjに対するl2normが求まったらZに入れる
            Z += exp(-max((l2norm[j1*width+j2] - 2*sigma*sigma), 0) / (h*h));
        }
    }
    // src(i1, i2)を変換
    for (int j1=start; j1<=end_h; ++j1) {
        for (int j2=start; j2<=end_w; ++j2) {
            w = exp(-max((l2norm[j1*width+j2] - 2*sigma*sigma), 0) / (h*h)) / Z;
            sum += w * src.at<uchar>(j1, j2);
        }
    }
    filtered.at<double>(i1-range, i2-range) = sum;
}

cv::Mat nonLocalMeans(cv::Mat src, int d, double sigma, double h) {
    cv::Mat filteredImage = cv::Mat::zeros(src.rows-d/2*2, src.cols-d/2*2, CV_64F); // 256*256
    int width = src.cols; // 258
    int height = src.rows; // 258
    int start = d/2; // 1
    int end_h = height - d/2 - 1; // 258-2=256
    int end_w = width - d/2 - 1; // 256
    int time = 0;
    for (int i1=start; i1<=end_h; ++i1) {
        printf("Loop %d\n", ++time);
        for (int i2=start; i2<=end_w; ++i2) {
            g(src, filteredImage, i1, i2, d, sigma, h);
        }
    }
    return filteredImage;
}

int main(int argc, char** argv) {
    // ノイズ付き入力画像
    cv::Mat src = cv::imread(argv[1]);

    // 余白を追加
    int r = 3;
    cv::copyMakeBorder(src, src, r/2, r/2, r/2, r/2, cv::BORDER_CONSTANT, cv::Scalar(0,0,0));
    
    vector<cv::Mat> split;
    cv::split(src, split);
    cv::Mat src_blue = split.at(0);
    // cv::Mat src_green = split.at(1);
    // cv::Mat src_red = split.at(2);

    double sigma = 10;
    double h = 10;
    printf("Blue Processing...\n");
    cv::Mat filtered_blue = nonLocalMeans(src_blue, r, sigma, h);
    // printf("Blue finished.\n");
    // cv::Mat filtered_green = nonLocalMeans(src_green, r, sigma, h);
    // printf("Green finished.\n");
    // cv::Mat filtered_red = nonLocalMeans(src_red, r, sigma, h);
    // printf("Red finished.\n");

    // cv::Mat filtered;
    // vector<cv::Mat> filtered_split = {filtered_blue, filtered_green, filtered_red};
    // cv::merge(filtered_split, filtered);
    cv::imwrite("filtered_blue.png", filtered_blue);
    printf("Finished.\nr: %d\nsigma: %f\nh: %f\n", r, sigma, h);

    return 0;
}