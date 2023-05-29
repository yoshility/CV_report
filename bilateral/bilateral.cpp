#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;

double gauss(double x, double sigma) {
  return exp(-x / (2*sigma*sigma));
}

void g(cv::Mat src, cv::Mat filtered, int i, int j, int d, double sigma_d, double sigma_r) {
  double up = 0;
  double down = 0;
  double wd, wr;
  double w = 0;
  int range = d/2;
  for (int m=-range; m<=range; ++m) {
    for (int n=-range; n<=range; ++n) {
      wd = gauss(m*m + n*n, sigma_d);
      wr = gauss(pow((src.at<uchar>(i, j) - src.at<uchar>(i+m, j+n)), 2), sigma_r);
      w = wd * wr;
      up += src.at<uchar>(i+m, j+n) * w;
      down += w;
    }
  }
  filtered.at<double>(i-range, j-range) = up / down;
}

cv::Mat bilateralFilter(cv::Mat src, int d, double sigma_d, double sigma_r) {
  cv::Mat filteredImage = cv::Mat::zeros(src.rows-d/2*2, src.cols-d/2*2, CV_64F); // 256*256
  int width = src.cols; // 258
  int height = src.rows; // 258
  // 各ピクセルに対し関数を適用
  int start = d/2; // 1
  int end_h = height - d/2 - 1; // 258-2=256
  int end_w = width - d/2 - 1; // 256
  for (int i=start; i<=end_h; ++i) {
    for (int j=start; j<=end_w; ++j) {
      g(src, filteredImage, i, j, d, sigma_d, sigma_r);
    }
  }
  return filteredImage;
}

int main(int argc, char** argv) {
  // ノイズ付き入力画像
  cv::Mat src = cv::imread(argv[1]);

  // 余白を追加
  int r = 5;
  double sigma_d = 12;
  double sigma_r = 16;
  cv::copyMakeBorder(src, src, r/2, r/2, r/2, r/2, cv::BORDER_CONSTANT, cv::Scalar(0,0,0));

  // BGRに分割
  vector<cv::Mat> split;
  cv::split(src, split);
  cv::Mat src_blue = split.at(0);
  cv::Mat src_green = split.at(1);
  cv::Mat src_red = split.at(2);
  
  // BGRごとにフィルタ
  printf("Processing...\n");
  cv::Mat filtered_blue = bilateralFilter(src_blue, r, sigma_d, sigma_r);
  printf("Blue finished.\n");
  cv::Mat filtered_green = bilateralFilter(src_green, r, sigma_d, sigma_r);
  printf("Green finished.\n");
  cv::Mat filtered_red = bilateralFilter(src_red, r, sigma_d, sigma_r);
  printf("Red finished.\n");

  // 統合
  cv::Mat filtered;
  vector<cv::Mat> filtered_split = {filtered_blue, filtered_green, filtered_red};
  cv::merge(filtered_split, filtered);
  cv::imwrite("filtered.png", filtered);
  printf("Finished.\nr: %d\nsigma_d: %f\nsigma_r: %f\n", r, sigma_d, sigma_r);

  return 0;
}
