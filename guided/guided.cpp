#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;

void q(cv::Mat guide, cv::Mat src, cv::Mat filtered, int i1, int i2, int d, double eps) {
    // 画素(i1,i2)を含む各パッチについて係数を求める
    double a = 0; // ここにd*d個の係数を足していく
    double b = 0; // 同上
    int range = d - 1; // range=2
    for (int k1=i1-range; k1<=i1; ++k1) {
        for (int k2=i2-range; k2<=i2; ++k2) {
            // いまの(k1,k2)を左上とするパッチについて計算する
            double sum1 = 0;
            double mu = 0;
            double p = 0;
            for (int m=0; m<=range; ++m) {
                for (int n=0; n<=range; ++n) {
                    sum1 += guide.at<uchar>(k1+m, k2+n) * src.at<uchar>(k1+m, k2+n);
                    mu += guide.at<uchar>(k1+m, k2+n);
                    p += src.at<uchar>(k1+m, k2+n);
                }
            }
            sum1 /= (d*d);
            mu /= (d*d);
            p /= (d*d);

            double sigma = 0;
            for (int m=0; m<=range; ++m) {
                for (int n=0; n<=range; ++n) {
                    sigma += pow((guide.at<uchar>(k1+m, k2+n) - mu), 2);
                }
            }
            sigma /= (d*d);

            // 求めた係数を累積
            a += ((sum1 - mu * p) / (sigma + eps));
            b += (p - (sum1 - mu * p) / (sigma + eps) * mu);
        }
    }
    // 係数の平均を求める
    a /= (d*d);
    b /= (d*d);

    filtered.at<double>(i1-range, i2-range) = a * guide.at<uchar>(i1, i2) + b;
}

cv::Mat guided(cv::Mat guide, cv::Mat src, int d, double eps) {
    cv::Mat filteredImage = cv::Mat::zeros(src.rows-2*(d-1), src.cols-2*(d-1), CV_64F); // 256*256
    int width = src.cols; // 260
    int height = src.rows; // 260
    int start = d - 1; // 2
    int end_h = height - d; // 260-3=257
    int end_w = width - d; // 257
    for (int i1=start; i1<=end_h; ++i1) {
        for (int i2=start; i2<=end_w; ++i2) {
            // 各画素について変換
            q(guide, src, filteredImage, i1, i2, d, eps);
        }
    }
    return filteredImage;
}

int main(int argc, char** argv) {
    // ノイズ付き入力画像
    cv::Mat src = cv::imread(argv[1]);

    // 余白を追加
    int r = 8;
    cv::copyMakeBorder(src, src, r-1, r-1, r-1, r-1, cv::BORDER_CONSTANT, cv::Scalar(0,0,0));

    // BGRに分割
    vector<cv::Mat> split;
    cv::split(src, split);
    cv::Mat src_blue = split.at(0);
    cv::Mat src_green = split.at(1);
    cv::Mat src_red = split.at(2);

    // BGRごとにフィルタ
    double eps = 100;
    printf("Processing...\n");
    cv::Mat filtered_blue = guided(src_blue, src_blue, r, eps);
    printf("Blue finished.\n");
    cv::Mat filtered_green = guided(src_green, src_green, r, eps);
    printf("Green finished.\n");
    cv::Mat filtered_red = guided(src_red, src_red, r, eps);
    printf("Red finished.\n");

    // 統合
    cv::Mat filtered;
    vector<cv::Mat> filtered_split = {filtered_blue, filtered_green, filtered_red};
    cv::merge(filtered_split, filtered);
    cv::imwrite("filtered.png", filtered);
    printf("Finished.\nr: %d\neps: %f\n", r, eps);

    return 0;
}