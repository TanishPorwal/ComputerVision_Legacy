#pragma once

#include <immintrin.h>

#include <algorithm>

#include "Image.h"
#if defined(_MSC_VER)
#include "MathConstants.h"
#endif

enum class ColorConversionCodes
{
    COLOR_RGB2RGBA     = 0, // add ALPHA channel to RGB images
    COLOR_RGBA2RGB     = 1, // remove ALPHA channel from RGB images
                       
    COLOR_RGB2GRAY     = 2, // convert RGB image to GRAYSCALE
    COLOR_RGBA2GRAY    = 3, 
                       
    COLOR_RGB2HSV      = 4, // convert RGB to HSV (hue saturation value) with H range [0, 180]
    COLOR_HSV2RGB      = 5,

    COLOR_RGB2YUV      = 6, // convert RGB to YUV
    COLOR_YUV2RGB      = 7,
};

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
            if(V == 0){
                S = 0;
                H = 0; //undefined color
            }else{
                S = (V - minColor)/V;
                if(R == V){
                    H = (G-B)/(V-minColor)*M_PI/3.0;
                }else if(G == V){
                    H = (2 + (B-R)/(V-minColor))*M_PI/3.0;
                }else if(B == V){
                    H = (4 + (R-G)/(V-minColor))*M_PI/3.0;
                }
                if(H < 0){
                    H += 2*M_PI;
                }
            }
                
            dst.at(i) = static_cast<uint8_t>(H/(2.0*M_PI) * 255.0);
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

            int i = static_cast<int>(H / (M_PI/3.0)); //falls in which sector
            double f = H/(M_PI/3.0) - k;
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

    /* _in and _out must be 16-byte aligned */
    void rgb_to_bgrx_sse(unsigned w, const void *_in, void *_out)
    {
        const auto *in_vec = static_cast<const __m128i *>(_in);
        auto *out_vec = static_cast<__m128i *>(_out);

        w /= 16;

        while (w-- > 0) {
            /*             0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
             * in_vec[0]   Ra Ga Ba Rb Gb Bb Rc Gc Bc Rd Gd Bd Re Ge Be Rf
             * in_vec[1]   Gf Bf Rg Gg Bg Rh Gh Bh Ri Gi Bi Rj Gj Bj Rk Gk
             * in_vec[2]   Bk Rl Gl Bl Rm Gm Bm Rn Gn Bn Ro Go Bo Rp Gp Bp
             */
            __m128i in1, in2, in3;
            __m128i out;

            in1 = in_vec[0];

            out = _mm_shuffle_epi8(in1,
                                   _mm_set_epi8(0xff, 9, 10, 11, 0xff, 6, 7, 8, 0xff, 3, 4, 5, 0xff, 0, 1, 2));
            out = _mm_or_si128(out,
                               _mm_set_epi8(0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0));
            out_vec[0] = out;

            in2 = in_vec[1];

            in1 = _mm_and_si128(in1,
                                _mm_set_epi8(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0, 0, 0, 0, 0, 0, 0, 0));
            out = _mm_and_si128(in2,
                                _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff));
            out = _mm_or_si128(out, in1);
            out = _mm_shuffle_epi8(out,
                                   _mm_set_epi8(0xff, 5, 6, 7, 0xff, 2, 3, 4, 0xff, 15, 0, 1, 0xff, 12, 13, 14));
            out = _mm_or_si128(out,
                               _mm_set_epi8(0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0));
            out_vec[1] = out;

            in3 = in_vec[2];
            in_vec += 3;

            in2 = _mm_and_si128(in2,
                                _mm_set_epi8(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0, 0, 0, 0, 0, 0, 0, 0));
            out = _mm_and_si128(in3,
                                _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff));
            out = _mm_or_si128(out, in2);
            out = _mm_shuffle_epi8(out,
                                   _mm_set_epi8(0xff, 1, 2, 3, 0xff, 14, 15, 0, 0xff, 11, 12, 13, 0xff, 8, 9, 10));
            out = _mm_or_si128(out,
                               _mm_set_epi8(0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0));
            out_vec[2] = out;

            out = _mm_shuffle_epi8(in3,
                                   _mm_set_epi8(0xff, 13, 14, 15, 0xff, 10, 11, 12, 0xff, 7, 8, 9, 0xff, 4, 5, 6));
            out = _mm_or_si128(out,
                               _mm_set_epi8(0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0));
            out_vec[3] = out;

            out_vec += 4;
        }
    }
    
    Image& CvtColor(Image& src, Image& dst, ColorConversionCodes conversion_codes)
    {
        switch(conversion_codes) {
        case ColorConversionCodes::COLOR_RGB2RGBA:
            rgb_to_bgrx_sse(src.size(), &src.at(0), &dst.at(0));
            break;
        case ColorConversionCodes::COLOR_RGBA2RGB: break;
        case ColorConversionCodes::COLOR_RGB2GRAY:
            cv::RGB2GRAY(src, dst);
            break;
        case ColorConversionCodes::COLOR_RGBA2GRAY: break;
        case ColorConversionCodes::COLOR_RGB2HSV:
            RGB2HSV(src, dst);
            break;
        case ColorConversionCodes::COLOR_HSV2RGB:
            HSV2RGB(src, dst);
            break;
        case ColorConversionCodes::COLOR_RGB2YUV:
            RGB2YUV(src, dst);
            break;
        case ColorConversionCodes::COLOR_YUV2RGB:
            YUV2RGB(src, dst);
            break;
        }
        return src;
    }
}
