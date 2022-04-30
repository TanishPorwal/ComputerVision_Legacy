#pragma once

#include "Image.h"

#include <cmath>

enum class ColorConversionCodes
{
    COLOR_RGB2RGBA     = 0, // add ALPHA channel to RGB images
    COLOR_RGBA2RGB     = 1, // remove ALPHA channel from RGB images
                       
    COLOR_RGB2GRAY     = 2, // convert RGB image to GRAYSCALE
    COLOR_RGBA2GRAY    = 3, 
                       
    COLOR_RGB2HSV      = 4, // convert RGB to HSV (hue saturation value) with H range [0, 180]
    COLOR_HSV2RGB      = 5,

    COLOR_RGB2YUV      = 6,
    COLOR_YUV2RGB      = 7,
};

#define HUE_BOUND(value) value < 0 ? 0 : ((value) > 360 ? 360 : (value))

namespace cv
{
    void RGB2HSV(Image& src, Image& dst)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(uint64_t i = 0; i < src.size(); i += src.channels())
        {
            double R = static_cast<double>(src.at(i)) / 255.0;
            double G = static_cast<double>(src.at(i+1)) / 255.0;
            double B = static_cast<double>(src.at(i+2)) / 255.0;

            double minColor = std::min({B, G, R});

            double H = 0, S = 0, V = 0;

            V = std::max({B, G, R});
            constexpr double pi = 3.141592653589793;
            if(V == 0){
                S = 0;
                H = 0; //undefined color
            }else{
                S = (V - minColor)/V;
                if(R == V){
                    H = (G-B)/(V-minColor)*pi/3.0;
                }else if(G == V){
                    H = (2 + (B-R)/(V-minColor))*pi/3.0;
                }else if(B == V){
                    H = (4 + (R-G)/(V-minColor))*pi/3.0;
                }
                if(H < 0){
                    H += 2*pi;
                }
            }
                
            dst.at(i) = static_cast<uint8_t>(H/(2.0*pi) * 255.0);
            dst.at(i+1) = static_cast<uint8_t>(S * 255);
            dst.at(i+2) = static_cast<uint8_t>(V * 255.0);
        }
    }

    void HSV2RGB(Image& src, Image& dst)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());

        for(uint64_t k = 0; k < src.size(); k += src.channels())
        {
            double V = static_cast<double>(src.at(k)) / 255.0;
            double S = static_cast<double>(src.at(k+1)) / 255.0;
            double H = static_cast<double>(src.at(k+2)) / 255.0;

            double R = 0, G = 0, B = 0;

            constexpr double pi = 3.141592653589793;
            int i = static_cast<int>(H / (pi/3.0)); //falls in which sector
            double f = H/(pi/3.0) - k;
            double p = V * (1-S);
            double q = V * (1-f*S);
            double t = V * (1 - (1-f) * S);

            switch(i){
            case 0:
                R = V;
                G = t;
                B = p;
                break;
            case 1:
                R = q;
                G = V;
                B = p;
                break;
            case 2:
                R = q;
                G = V;
                B = t;
                break;
            case 3:
                R = p;
                G = q;
                B = V;
                break;
            case 4:
                R = t;
                G = p;
                B = V;
                break;
            case 5:
                R = V;
                G = p;
                B = q;
                break;
            }

            dst.at(i) = static_cast<uint8_t>(R * 255);
            dst.at(i+1) = static_cast<uint8_t>(G * 255);
            dst.at(i+2) = static_cast<uint8_t>(B * 255);
        }
    }

    void RGB2YUV(Image& src, Image& dst)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(uint64_t i = 0; i < src.size(); i += src.channels())
        {
            double R = src.at(i);
            double G = src.at(i+1);
            double B = src.at(i+2);

            double Y = 0.299*R + 0.587*G + 0.114*B;
            double U = 0.567*(B-Y);
            double V = 0.713*(R-Y);

            //truncate value exceed the range, it seems we can use a better normalization method?
            Y = std::min(std::max((int)Y, 0), 255); 
            U = std::min(std::max((int)U, 0), 255); 
            V = std::min(std::max((int)V, 0), 255);

            dst.at(i) = static_cast<uint8_t>(Y);
            dst.at(i+1) = static_cast<uint8_t>(U);
            dst.at(i+2) = static_cast<uint8_t>(V);
        }
    }

    void YUV2RGB(Image& src, Image& dst)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(uint64_t i = 0; i < src.size(); i += src.channels())
        {
            double Y = src.at(i);
            double U = src.at(i+1);
            double V = src.at(i+2);

            double R = Y + 1.402 * V;
            double G = Y - 0.344 * U - 0.714 * V;
            double B = Y + 1.772 * U;

            R = std::min(std::max((int)R, 0), 255);
            G = std::min(std::max((int)G, 0), 255);
            B = std::min(std::max((int)B, 0), 255);
            
            dst.at(i) = static_cast<uint8_t>(R);
            dst.at(i+1) = static_cast<uint8_t>(G);
            dst.at(i+2) = static_cast<uint8_t>(B);
        }
    }
    
    void RGB2GRAY(Image& src, Image& dst)
    {
        if(src.channels() < 3)
        {
            std::cout << "Channels in image are less than 3.\n";
            return;
        }
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        else
            for(uint64_t i = 0; i < src.size(); i += src.channels())
            {
                uint8_t gray = static_cast<uint8_t>(BYTE_BOUND((0.2126 * src.at(i) + 0.7152 * src.at(i + 1) + 0.0722 * src.at(i + 2))));
                src.at(i) = gray;
                src.at(i+1) = gray;
                src.at(i+2) = gray;
            }
    }
}
