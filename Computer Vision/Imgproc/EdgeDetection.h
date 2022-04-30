#pragma once

#include "Filters.h"
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
        filter2D(src, sobelX_, sobelX, 3, 3);
        filter2D(src, sobelY_, sobelY, 3, 3);
        for(uint64_t i = 0; i < src.size(); i++)
            dst.at(i) = static_cast<uint8_t>(BYTE_BOUND(sqrt(sobelX_.at(i)*sobelX_.at(i) + sobelY_.at(i)*sobelY_.at(i))));

        linearTransform(dst, dst, -1, 255);
        //"Thining" assumes black as fg, white as bg
        // Thining(dst);
        //invert color
        linearTransform(dst, dst, -1, 255);
    }
}