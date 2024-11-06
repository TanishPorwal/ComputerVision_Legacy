#pragma once

#include "opencv2/opencv.hpp"

cv::Mat linearBlend(const cv::Mat& src1, const cv::Mat& src2, float alpha) {
    if (alpha < 0.0f || alpha > 1.0f) {
        throw std::invalid_argument("Alpha must be between 0 and 1.");
    }

    if (src1.size() != src2.size() || src1.type() != src2.type()) {
        throw std::invalid_argument("Input images must have the same size and type.");
    }

    cv::Mat blended_image;
    src1.convertTo(src1, CV_32F);
    src2.convertTo(src2, CV_32F);

    blended_image = (1.0f - alpha) * src1 + alpha * src2;

    blended_image.convertTo(blended_image, src1.type());

    return blended_image;
}

// Ensure that the image passed to this function is grayscale as this algorithm is supposed to work only on gray scale images
cv::Mat gammaCorrection(const cv::Mat& src, float gamma) {
    cv::Mat corrected_image(src.rows, src.cols, CV_32F);

    for (int y = 0; y < src.rows; ++y) {
        for (int x = 0; x < src.cols; ++x) {
            float normalized_value = src.at<uint8_t>(y, x) / 255.0f;
            float corrected_value = std::pow(normalized_value, gamma);
            corrected_image.at<float>(y, x) = corrected_value * 255.0f;
        }
    }

    corrected_image.convertTo(corrected_image, CV_8U);

    return corrected_image;
}
