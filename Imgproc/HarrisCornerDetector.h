#pragma once

#include <opencv2/opencv.hpp>

void sobel_separate(const cv::Mat& src, cv::Mat& gradients_X, cv::Mat& gradients_Y) {
    gradients_X = cv::Mat(src.size(), CV_32F, cv::Scalar(0));
    gradients_Y = cv::Mat(src.size(), CV_32F, cv::Scalar(0));

    for (int y = 1; y < src.rows - 1; ++y) {
        for (int x = 1; x < src.cols - 1; ++x) {
            float p_00 = static_cast<float>(src.at<uchar>(y - 1, x - 1));
            float p_01 = static_cast<float>(src.at<uchar>(y - 1, x));
            float p_02 = static_cast<float>(src.at<uchar>(y - 1, x + 1));
            float p_10 = static_cast<float>(src.at<uchar>(y, x - 1));
            float p_12 = static_cast<float>(src.at<uchar>(y, x + 1));
            float p_20 = static_cast<float>(src.at<uchar>(y + 1, x - 1));
            float p_21 = static_cast<float>(src.at<uchar>(y + 1, x));
            float p_22 = static_cast<float>(src.at<uchar>(y + 1, x + 1));

            float sum_x = -p_00 + p_02 - 2 * p_10 + 2 * p_12 - p_20 + p_22;
            float sum_y = p_00 + 2 * p_01 + p_02 - p_20 - 2 * p_21 - p_22;

            gradients_X.at<float>(y, x) = sum_x;
            gradients_Y.at<float>(y, x) = sum_y;
        }
    }
}

void getGradientMatrices(const cv::Mat& gradients_X, const cv::Mat& gradients_Y, cv::Mat& Ixx, cv::Mat& Iyy, cv::Mat& Ixy, cv::Size gaussian_kernel_size, float gaussian_blur_sigma) {
    Ixx = cv::Mat(gradients_X.size(), CV_32F, cv::Scalar(0));
    Iyy = cv::Mat(gradients_X.size(), CV_32F, cv::Scalar(0));
    Ixy = cv::Mat(gradients_X.size(), CV_32F, cv::Scalar(0));

    for(int y = 0; y < gradients_X.rows; ++y)
        for(int x = 0; x < gradients_X.cols; ++x) {
            float gx = gradients_X.at<float>(y, x);
            float gy = gradients_Y.at<float>(y, x);
            Ixx.at<float>(y, x) = gx * gx;
            Iyy.at<float>(y, x) = gy * gy;
            Ixy.at<float>(y, x) = gx * gy;
        }

    cv::GaussianBlur(Ixx, Ixx, gaussian_kernel_size, gaussian_blur_sigma);
    cv::GaussianBlur(Iyy, Iyy, gaussian_kernel_size, gaussian_blur_sigma);
    cv::GaussianBlur(Ixy, Ixy, gaussian_kernel_size, gaussian_blur_sigma);
}

void harrisCornerDetector(const cv::Mat& src, cv::Mat& dst, float k, cv::Size gaussian_kernel_size, float gaussian_blur_sigma) {
    cv::Mat gradients_X, gradients_Y, Ixx, Iyy, Ixy, response_mat;

    sobel_separate(src, gradients_X, gradients_Y);
    getGradientMatrices(gradients_X, gradients_Y, Ixx, Iyy, Ixy, gaussian_kernel_size, gaussian_blur_sigma);

    response_mat = cv::Mat(Ixx.size(), CV_32F, cv::Scalar(0));
    for (int y = 0; y < gradients_X.rows; ++y) {
        for (int x = 0; x < gradients_X.cols; ++x) {
            float det = Ixx.at<float>(y, x) * Iyy.at<float>(y, x) - Ixy.at<float>(y, x) * Ixy.at<float>(y, x);
            float trace = Ixx.at<float>(y, x) + Iyy.at<float>(y, x);
            response_mat.at<float>(y, x) = det - k * trace * trace;
        }
    }

    double max_response = 0.0;
    cv::minMaxLoc(response_mat, nullptr, &max_response);
    float threshold = 0.01f * static_cast<float>(max_response);

    for (int y = 0; y < response_mat.rows; ++y) {
        for (int x = 0; x < response_mat.cols; ++x) {
            if (response_mat.at<float>(y, x) < threshold) {
                response_mat.at<float>(y, x) = 0;
            }
        }
    }

    cv::Mat suppressed = response_mat.clone();
    for (int y = 1; y < response_mat.rows - 1; ++y) {
        for (int x = 1; x < response_mat.cols - 1; ++x) {
            float current_value = response_mat.at<float>(y, x);
            if (current_value > 0) {
                bool is_local_maximum = true;
                for (int j = -1; j <= 1; ++j) {
                    for (int i = -1; i <= 1; ++i) {
                        if (response_mat.at<float>(y + j, x + i) > current_value) {
                            is_local_maximum = false;
                            break;
                        }
                    }
                    if (!is_local_maximum) break;
                }
                if (!is_local_maximum) {
                    suppressed.at<float>(y, x) = 0;
                }
            }
        }
    }
    cv::normalize(suppressed, suppressed, 0, 255, cv::NORM_MINMAX);
    suppressed.convertTo(suppressed, CV_8U);

    dst = src.clone();
    for (int y = 0; y < suppressed.rows; ++y) {
        for (int x = 0; x < suppressed.cols; ++x) {
            if (suppressed.at<uchar>(y, x) > 0) {
                cv::circle(dst, cv::Point(x, y), 2, cv::Scalar(0, 0, 255), -1);
            }
        }
    }
    
}
