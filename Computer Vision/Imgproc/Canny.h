#pragma once

#include "Convolution.h"
#include "Image.h"
#include "ImageOperations.h"

namespace cv
{
    void sobel(Image& src, Image& dst)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        double sobelX[9] = { 1, 0, -1,
                             2, 0, -2,
                             1, 0, -1};
        
        double sobelY[9] = { 1,  2,  1,
                             0,  0,  0,
                            -1, -2, -1};
        
        Image sobelX_, sobelY_;
        convolve2D(src, sobelX_, sobelX, 3, 3);
        convolve2D(src, sobelY_, sobelY, 3, 3);
        for(uint64_t i = 0; i < src.size(); i++)
            dst.at(i) = static_cast<uint8_t>(BYTE_BOUND(sqrt(sobelX_.at(i)*sobelX_.at(i) + sobelY_.at(i)*sobelY_.at(i))));
    }

    void sobel_optimized(Image& src, Image& dst)
    {
        
    }

    // void nonMaximumSuppression(Image& src, Image& dst, double theta)
    // {
    //         
    // }
}