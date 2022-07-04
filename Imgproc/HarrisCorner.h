/*
* ComputerVision Library (https://github.com/BubblyBunny2/ComputerVision).
*
* Copyright (c) 2022 Tanish.
*/


#pragma once

#include "Image.h"
#include "ColorConversion.h"
#include "Convolution.h"

namespace cv
{
    void HarrisCornerDetector(Image& src, Image& dst, float k, int filterRange, bool gauss)
    {
        Image grayscale = cv::CvtColor(src, src, ColorConversionCodes::COLOR_RGB2GRAY);

        double sobelX[9] = { 1, 0, -1,
                             2, 0, -2,
                             1, 0, -1};

        double sobelY[9] = { 1,  2,  1,
                             0,  0,  0,
                             -1, -2, -1};

        Image sobelX_, sobelY_;
        Convolve2D(src, sobelX_, sobelX, 3, 3);
        Convolve2D(src, sobelY_, sobelY, 3, 3);


    }
}

