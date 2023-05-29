#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char** argv) {
    cv::Mat filtered_blue = cv::imread(argv[1]);
    cv::Mat filtered_green = cv::imread(argv[2]);
    cv::Mat filtered_red = cv::imread(argv[3]);

    cv::cvtColor(filtered_blue, filtered_blue, cv::COLOR_BGR2GRAY, 1);
    cv::cvtColor(filtered_green, filtered_green, cv::COLOR_BGR2GRAY, 1);
    cv::cvtColor(filtered_red, filtered_red, cv::COLOR_BGR2GRAY, 1);

    cv::Mat filtered;
    vector<cv::Mat> filtered_split = {filtered_blue, filtered_green, filtered_red};
    cv::merge(filtered_split, filtered);
    cv::imwrite("filtered.png", filtered);
    printf("Merged.\n");

    return 0;
}