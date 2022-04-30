#include <iostream>

#include "Image.h"
#include "Imgproc/ConvertColor.h"
#include "Imgproc/Filters.h"
#include "Timer.h"

#include "Imgproc/ArithmaticOperations.h"
#include "Imgproc/ImageOperations.h"

#include "Imgproc/EdgeDetection.h"

#include "Imgproc/DrawShapes.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

int main()
{
    Timer timer;
    double emboss[9] = { -2,  -1,  0,
                         -1,   1,  1,
                          0,   1,  2};

    double sharpen[9] = { 0, -1,  0,
                         -1,  5, -1,
                          0, -1,  0};

    double raised[9] = {  0,  0, -2,
                          0,  2,  0,
                          1,  0,  0};

    double edge_detect[9] = {  0, -1,  0,
                              -1,  4, -1,
                               0, -1,  0};

    double outline[9] = { -1/3.0, -1/3.0, -1/3.0,
                          -1/3.0,  8/3.0, -1/3.0,
                          -1/3.0, -1/3.0, -1/3.0};

    double denoise[9] = {1/9.0, 1/9.0, 1/9.0,
                         1/9.0, 1/9.0, 1/9.0,
                         1/9.0, 1/9.0, 1/9.0};

    Image test1("Resources/Lena.png");
    cv::circle_test(test1, {100, 100}, 4, {255, 0, 0});
    test1.write("Resources/Circles.png");

    // cv::Mat mat = cv::imread("Resources/Lena.png");
    // cv::circle(mat, {100, 100}, 4, {0, 0, 255});
    // cv::imwrite("Resources/Circles_opencv.png", mat);

    // cv::gaussianBlur(test1, test1, 3, 3, 1);
    // cv::filter2D(test1, test2, edge_detect, 3, 3);
    // test2.write("Resources/Test_Edge_Detect.png");
    // cv::bitwise_and(test1, test1, test1);
    // test1.write("Resources/Bitwise_And.png");
    return 0;
}
