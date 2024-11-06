#pragma once

#include "opencv2/opencv.hpp"

cv::Mat convert_to_grayscale(const cv::Mat& src){
    cv::Mat dst(src.rows, src.cols, CV_8UC1);

    int totalPixels = src.rows * src.cols;

    // Loop over each pixel
    for (int i = 0; i < totalPixels; ++i) {
        // Extract RGB values
        uchar blue = src.data[i * 3];
        uchar green = src.data[i * 3 + 1];
        uchar red = src.data[i * 3 + 2];

        // Calculate the grayscale value
        dst.data[i] = static_cast<uchar>(0.299 * red + 0.587 * green + 0.114 * blue);
    }
    return dst;
}