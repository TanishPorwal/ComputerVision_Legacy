#pragma once

#include "opencv2/opencv.hpp"

void generateGaussianPyramid(const cv::Mat& src, std::vector<std::vector<cv::Mat>> gaussian_pyramid, int num_octaves,
                             int num_scales, double sigma, double k) {;
    cv::Mat current_image = src.clone();

    for(int octave = 0; octave < num_octaves; ++octave) {
        std::vector<cv::Mat> octave_images;

        for(int scale = 0; scale < num_scales; ++scale) {
            double current_sigma = sigma * pow(k, scale); // sigma(i+1) = k * sigma(i)
            cv::Mat blurred_image;
            cv::GaussianBlur(current_image, blurred_image, cv::Size(), current_sigma);
            octave_images.push_back(blurred_image);
        }
        gaussian_pyramid.push_back(octave_images);

        // Downsampling the Image by a factor of 2
        cv::pyrDown(current_image, current_image);
    }
}

void differenceOfGaussians(const std::vector<std::vector<cv::Mat>>& gaussian_pyramid, std::vector<std::vector<cv::Mat>> dog_pyramid) {
    for(const auto& octave_images : gaussian_pyramid) {
        std::vector<cv::Mat>  dog_octave;
        for(uint32_t i = 0; i < dog_octave.size() - 1; ++i) {
            cv::Mat dog_image;
            cv::subtract(octave_images[i], octave_images[i - 1], dog_image);
            dog_octave.push_back(dog_image);
        }
        dog_pyramid.push_back(dog_octave);
    }
}

void detectKeypoints(const std::vector<std::vector<cv::Mat>>& dog_pyramid, std::vector<cv::KeyPoint>& keypoints,
                     const double corner_threshold, const double edge_threshold) {
    for (const auto& octave : dog_pyramid) {
        for (auto scale = 1; scale < octave.size() - 1; ++scale) { // Avoid first and last scales (boundary)
            for (int y = 1; y < octave[scale].rows - 1; ++y) {
                for (int x = 1; x < octave[scale].cols - 1; ++x) {
                    auto current_pixel = octave[scale].at<uint8_t>(y, x);

                    bool is_extremum = true;

                    // Compare with the 8 neighbors in the same scale
                    for (int iy = -1; iy <= 1; ++iy) {
                        for (int ix = -1; ix <= 1; ++ix) {
                            if (iy == 0 && ix == 0) continue;  // Skip the center pixel itself
                            if (current_pixel <= octave[scale].at<uint8_t>(y + iy, x + ix)) {
                                is_extremum = false;
                                break;
                            }
                        }
                    }

                    // Compare with the 9 neighbors in the scale above
                    if (scale > 0) {  // Check if we are not on the first scale
                        for (int iy = -1; iy <= 1; ++iy) {
                            for (int ix = -1; ix <= 1; ++ix) {
                                if (current_pixel <= octave[scale - 1].at<uint8_t>(y + iy, x + ix)) {
                                    is_extremum = false;
                                    break;
                                }
                            }
                        }
                    }

                    // Compare with the 9 neighbors in the scale below
                    if (scale < octave.size() - 1) {  // Check if we are not on the last scale
                        for (int iy = -1; iy <= 1; ++iy) {
                            for (int ix = -1; ix <= 1; ++ix) {
                                if (current_pixel <= octave[scale + 1].at<uint8_t>(y + iy, x + ix)) {
                                    is_extremum = false;
                                    break;
                                }
                            }
                        }
                    }

                    if (is_extremum) {
                        float scale_factor = 1.6;
                        float keypoint_size = scale_factor * (1 << scale);
                        cv::KeyPoint keypoint(x, y, keypoint_size);
                        keypoints.push_back(keypoint);
                    }
                }
            }
        }
    }
}

void computeGradients(const cv::Mat& image, cv::Mat& grad_x, cv::Mat& grad_y) {
    cv::Sobel(image, grad_x, CV_32F, 1, 0, 3);
    cv::Sobel(image, grad_y, CV_32F, 0, 1, 3);
}

void computeHessianAndGradient(const cv::Mat& image, int x, int y,
                                float& dxx, float& dyy, float& dxy, float& dx, float& dy) {
    cv::Mat grad_x, grad_y;
    computeGradients(image, grad_x, grad_y);

    dx = grad_x.at<float>(y, x);
    dy = grad_y.at<float>(y, x);

    dxx = grad_x.at<float>(y, x);
    dyy = grad_y.at<float>(y, x);
    dxy = (grad_x.at<float>(y + 1, x + 1) - grad_x.at<float>(y, x)) / 2 +
          (grad_y.at<float>(y, x + 1) - grad_y.at<float>(y + 1, x)) / 2;
}

bool refineKeypoint(cv::KeyPoint& keypoint, const cv::Mat& image, float threshold = 0.5) {
    int x = static_cast<int>(keypoint.pt.x);
    int y = static_cast<int>(keypoint.pt.y);

    float dxx, dyy, dxy, dx, dy;
    computeHessianAndGradient(image, x, y, dxx, dyy, dxy, dx, dy);

    float H[2][2] = {{dxx, dxy}, {dxy, dyy}};
    cv::Mat H_inv = cv::Mat(2, 2, CV_32F, H);
    cv::Mat gradient = (cv::Mat_<float>(2, 1) << dx, dy);

    cv::Mat displacement = H_inv.inv() * gradient;

    if (cv::norm(displacement) < threshold) {
        keypoint.pt.x += displacement.at<float>(0);
        keypoint.pt.y += displacement.at<float>(1);
        return true;
    }
    return false;
}

void assignOrientation(const cv::Mat& image, cv::KeyPoint& keypoint, int window_size = 16, int bins = 36) {
    int x = static_cast<int>(keypoint.pt.x);
    int y = static_cast<int>(keypoint.pt.y);

    cv::Mat grad_x, grad_y;
    computeGradients(image, grad_x, grad_y);

    std::vector<int> orientation_histogram(bins, 0);

    // Define region around the keypoint (16x16 window)
    int half_window = window_size / 2;
    int start_x = x - half_window;
    int start_y = y - half_window;

    for (int i = start_y; i < start_y + window_size; ++i) {
        for (int j = start_x; j < start_x + window_size; ++j) {
            float gx = grad_x.at<float>(i, j);
            float gy = grad_y.at<float>(i, j);
            float magnitude = std::sqrt(gx * gx + gy * gy);
            float orientation = std::atan2(gy, gx) * 180 / CV_PI;
            if (orientation < 0) orientation += 360;

            int bin = static_cast<int>(orientation / (360.0 / bins)) % bins;
            orientation_histogram[bin] += static_cast<int>(magnitude);
        }
    }

    int max_bin_value = 0;
    for (int i = 0; i < bins; ++i) {
        max_bin_value = std::max(max_bin_value, orientation_histogram[i]);
    }

    std::vector<int> peaks;
    for (int i = 0; i < bins; ++i) {
        if (orientation_histogram[i] > 0.8 * max_bin_value) {
            peaks.push_back(i);
        }
    }

    // If multiple peaks are found, create additional keypoints
    if (peaks.size() > 1) {
        for (int i = 0; i < peaks.size(); ++i) {
            float peak_orientation = peaks[i] * (360.0 / bins);
            keypoint.angle = peak_orientation;
        }
    } else {
        keypoint.angle = peaks[0] * (360.0 / bins);
    }
}

void generateDescriptors(const std::vector<cv::KeyPoint>& keypoints, const cv::Mat& src,
                         std::vector<cv::Mat>& descriptors, int num_scales, double sigma, double k) {
    for (const auto& keypoint : keypoints) {
        cv::Mat descriptor(1, 128, CV_32F, cv::Scalar(0));
        int x = cvRound(keypoint.pt.x);
        int y = cvRound(keypoint.pt.y);
        float angle = keypoint.angle;

        int window_size = 16;
        int cell_size = 4;
        int num_cells = window_size / cell_size;
        int hist_size = 8;

        for (int i = 0; i < num_cells; i++) {
            for (int j = 0; j < num_cells; j++) {
                int start_x = x + (i - num_cells / 2) * cell_size;
                int start_y = y + (j - num_cells / 2) * cell_size;
                cv::Mat cell_hist = cv::Mat::zeros(1, hist_size, CV_32F);

                for (int di = 0; di < cell_size; di++) {
                    for (int dj = 0; dj < cell_size; dj++) {
                        int gx = src.at<cv::Vec3b>(start_y + di, start_x + dj)[0]; // Gradient in X
                        int gy = src.at<cv::Vec3b>(start_y + di, start_x + dj)[1]; // Gradient in Y
                        float magnitude = std::sqrt(gx * gx + gy * gy);
                        float orientation = std::atan2(gy, gx) * 180 / CV_PI;
                        if (orientation < 0) orientation += 360;
                        int bin = static_cast<int>(orientation / (360.0 / hist_size));
                        cell_hist.at<float>(bin) += magnitude;
                    }
                }
                cell_hist.copyTo(descriptor.row(0).colRange(i * hist_size, (i + 1) * hist_size));
            }
        }

        cv::normalize(descriptor, descriptor, 0, 255, cv::NORM_MINMAX);
        descriptors.push_back(descriptor);
    }
}


// A standard SIFT algorithm has 3 octaves and 4 scales per octave
void SIFT(const cv::Mat& src, std::vector<cv::KeyPoint>& keypoints, std::vector<cv::Mat>& descriptors, const int num_octaves = 3,
          const int num_scales = 4, const double sigma = 1.6, const double k = 1.414,
          const double contrast_threshold = 0.04, const double edge_threshold = 10.0) {
    std::vector<std::vector<cv::Mat>> gaussian_pyramid, dog_pyramid;
    // STEP 1: FINDING DoG(Difference of Gaussians)
    generateGaussianPyramid(src, gaussian_pyramid, num_octaves, num_scales, sigma, k);
    differenceOfGaussians(gaussian_pyramid, dog_pyramid);

    // STEP 2: Keypoint Detection in the DoG Pyramid
    detectKeypoints(dog_pyramid, keypoints, contrast_threshold, edge_threshold);

    // STEP 3: Keypoint Refinement
    for (auto& keypoint : keypoints) {
        refineKeypoint(keypoint, src);
    }

    // STEP 4: Orientation Assignment
    for (auto& keypoint : keypoints) {
        assignOrientation(keypoint, src);
    }

    // STEP 5: Descriptor Generation
    generateDescriptors(keypoints, src, descriptors, num_scales, sigma, k);


}