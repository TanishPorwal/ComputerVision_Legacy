#include <iostream>

#include "Image.h"
#include "Imgproc/ColorConversion.h"
#include "Imgproc/Convolution.h"
#include "Timer.h"

#include "Imgproc/ArithmeticOperations.h"
#include "Imgproc/Blur.h"
#include "Imgproc/ImageOperations.h"

#include "Imgproc/Canny.h"

#include "Imgproc/DrawShapes.h"

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

    Image test1("Resources/WindowsLogo.jpg"), test2("Resources/LinuxLogo.jpg");
    // cv::cvtColor(test1, test1, ColorConversionCodes::COLOR_RGB2GRAY);
    // cv::gaussianBlur(test1, test1, 5, 5, 1);
    // cv::sobel(test1, test2);
    // test1.write("Resources/Line.png");
    cv::circle(test1, {300, 100}, 20, {255, 0, 0});
    // test2.write("Resources/Add.png");

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
