#pragma once

#include "Convolution.h"
#include "Image.h"
#include "ImageOperations.h"

namespace cv
{
    inline void Sobel(Image& src, Image& dst)
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
        Convolve2D(src, sobelX_, sobelX, 3, 3);
        Convolve2D(src, sobelY_, sobelY, 3, 3);
        for(uint64_t i = 0; i < src.size(); i++)
            dst.at(i) = static_cast<uint8_t>(BYTE_BOUND(sqrt(sobelX_.at(i)*sobelX_.at(i) + sobelY_.at(i)*sobelY_.at(i))));
    }

    void Sobel_optimized(Image& src, Image& dst)
    {
        
    }

    // implement nonmaxima suppression
    void NonMaximaSuppression(Image& src, Image& dst)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(uint64_t i = 0; i < src.size(); i++)
        {
            uint8_t* p = &src.at(i);
            uint8_t* d = &dst.at(i);
            if(p[0] > p[1] && p[0] > p[2])
                d[0] = p[0];
            else
                d[0] = 0;
            if(p[1] > p[0] && p[1] > p[2])
                d[1] = p[1];
            else
                d[1] = 0;
            if(p[2] > p[0] && p[2] > p[1])
                d[2] = p[2];
            else
                d[2] = 0;
        }
    }

    // implement canny edge detector
    void canny(Image& src, Image& dst, double low, double high)
    {
        Image grayscale = cv::CvtColor(src, src, ColorConversionCodes::COLOR_RGB2GRAY);
        Image sobelX, sobelY;
        cv::Sobel(grayscale, sobelX);
        cv::Sobel(grayscale, sobelY);
        NonMaximaSuppression(grayscale, grayscale);
        for(uint64_t i = 0; i < src.size(); i++)
        {
            double magnitude = sqrt(sobelX.at(i)*sobelX.at(i) + sobelY.at(i)*sobelY.at(i));
            if(magnitude > high)
                dst.at(i) = 255;
            else if(magnitude < low)
                dst.at(i) = 0;
            else
                dst.at(i) = 128;
        }
    }

}


