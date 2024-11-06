#pragma once
#include <opencv2/opencv.hpp>

// this version of this algorithm will manipulate the symmetry in the sobel kernel to optimize the algorithm for the performance
void sobel(const cv::Mat& src, cv::Mat& dst, cv::Mat& gradients) {
    // Sobel Kernels for reference, they are unrolled in the loop for extra performance
/*    float sobel_x[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    float sobel_y[3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}
    };
*/
    dst = cv::Mat(src.size(), CV_32F, cv::Scalar(0));
    gradients = cv::Mat(src.size(), CV_32F, cv::Scalar(0));

    for(int y = 1; y < src.rows - 1; ++y)
        for(int x = 1; x < src.cols - 1; ++x) {
            uchar p_00 = src.at<uchar>(y - 1, x - 1);
            uchar p_01 = src.at<uchar>(y - 1, x);
            uchar p_02 = src.at<uchar>(y - 1, x + 1);
            uchar p_10 = src.at<uchar>(y, x - 1);
            uchar p_11 = src.at<uchar>(y, x);
            uchar p_12 = src.at<uchar>(y, x + 1);
            uchar p_20 = src.at<uchar>(y + 1, x - 1);
            uchar p_21 = src.at<uchar>(y + 1, x);
            uchar p_22 = src.at<uchar>(y + 1, x + 1);


            const float sum_x = -p_00 + p_02  -2 * p_10 + 2 * p_12 - p_20 + p_22;
            const float sum_y = p_00 + 2 * p_01 + p_02 - p_20 - 2 * p_21 - p_22;

            float mag = std::sqrt(sum_x * sum_x + sum_y * sum_y);
            dst.at<float>(y, x) = mag;

            float angle = atan2(sum_y, sum_x) * 180.0 / CV_PI;
            if (angle < 0) angle += 180;  // map angle to [0, 180) range

            // quantize to 0, 45, 90, or 135
            if ((angle >= 0 && angle < 22.5) || (angle >= 157.5 && angle < 180))
                angle = 0;
            else if (angle >= 22.5 && angle < 67.5)
                angle = 45;
            else if (angle >= 67.5 && angle < 112.5)
                angle = 90;
            else
                angle = 135;

            gradients.at<float>(y, x) = angle;
        }
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
}

void nms(const cv::Mat& src, cv::Mat& dst, const cv::Mat& angles) {
    dst = cv::Mat(src.size(), CV_8UC1, cv::Scalar(0));

    for (int y = 1; y < src.rows - 1; ++y) {
        for (int x = 1; x < src.cols - 1; ++x) {
            float angle = angles.at<float>(y, x);
            float magnitude = src.at<float>(y, x);
            float neighbor1, neighbor2;

            if (angle == 0) {
                neighbor1 = src.at<float>(y, x - 1);
                neighbor2 = src.at<float>(y, x + 1);
            }
            else if (angle == 45) {
                neighbor1 = src.at<float>(y - 1, x + 1);
                neighbor2 = src.at<float>(y + 1, x - 1);
            }
            else if (angle == 90) {
                neighbor1 = src.at<float>(y - 1, x);
                neighbor2 = src.at<float>(y + 1, x);
            }
            else {
                neighbor1 = src.at<float>(y - 1, x - 1);
                neighbor2 = src.at<float>(y + 1, x + 1);
            }

            if (magnitude >= neighbor1 && magnitude >= neighbor2) {
                dst.at<uint8_t>(y, x) = static_cast<uint8_t>(magnitude);
            } else {
                dst.at<uint8_t>(y, x) = 0;
            }
        }
    }
}

void doubleThresholding(const cv::Mat& src, cv::Mat& dst, int lower_bound, int upper_bound) {

}

void canny(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat grayscale, blurred, edge_sobel, gradients, suppressed_image;
    cv::cvtColor(src, grayscale, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(grayscale, blurred, cv::Size(3,3), 0);
    sobel(blurred, edge_sobel, gradients);
    nms(edge_sobel, dst, gradients);
}