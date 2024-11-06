#pragma once

#include "opencv2/opencv.hpp"
#include "omp.h"

cv::Mat addPadding(const cv::Mat& input, int padding) {
    cv::Mat padded;
    cv::copyMakeBorder(input, padded, padding, padding, padding, padding, cv::BORDER_CONSTANT, cv::Scalar(0));
    return padded;
}

cv::Mat conv2D(const cv::Mat& src, const cv::Mat& kernel, int stride, int padding) {
    cv::Mat padded_input = addPadding(src, padding);

    const int kernel_height = kernel.rows, kernel_width = kernel.cols;
    const int padded_height = padded_input.rows, padded_width = padded_input.cols;

    const int output_height = (padded_height - kernel_height) / stride + 1;
    const int output_width = (padded_width - kernel_width) / stride + 1;

    cv::Mat output(output_height, output_width, CV_32F, cv::Scalar(0));

    for(int y = 0; y < output_height; ++y)
        for(int x = 0; x < output_width; ++x) {
            float sum = 0.0f;
            for(int ky = 0; ky < kernel_height; ++ky)
                for (int kx = 0; kx < kernel_width; ++kx) {
                    int iy = y * stride + ky;
                    int ix = x * stride + kx;
                    sum += src.at<float>(iy, ix) * kernel.at<float>(ky, kx);
                }
            output.at<float>(y, x);
        }
    return output;
}
