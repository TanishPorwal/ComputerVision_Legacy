#include <iostream>

#include "Imgproc/Canny.h"
#include "Imgproc/ColorConversion.h"
#include "Imgproc/Convolutions.h"
#include "Imgproc/HarrisCornerDetector.h"
#include "Imgproc/PixelTransforms.h"

int main() {
    cv::Mat image = cv::imread("../Resources/Lena.png"); // ../ is used as a prefix because the dll is placed in the cmake-build-debug/release folder and to locate the image the exe file will switch to the parent directory and then from there it will navigate to resources folder where the image is placed
    cv::Mat edges, corners;
    if (image.empty()) {
        std::cerr << "Could not open or find the image." << std::endl;
        return -1;
    }

    canny(image, edges);
    harrisCornerDetector(image, corners, 0.05, {3, 3}, 1);

    cv::imshow("Original", image);
    cv::imshow("Canny Edge", edges);
    cv::imshow("Corner Detector", corners);
    cv::waitKey(0);
    return 0;
}
