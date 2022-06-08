#pragma once

#include "Image.h"

namespace cv
{
    void ShiftImage(Image& img, Image& dst, int channel, uint8_t value)
    {
        dst = img;
        for(uint64_t i = channel; i < img.size(); i += img.channels())
            BYTE_BOUND(dst.at(i) += value);
    }

    void Threshold(Image& src, Image& dst, uint8_t value)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(uint32_t i = 0; i < src.size(); i++) dst.at(i) = (src.at(i) < value) ? 0 : 255;
    }

    void LinearTransform(Image& src, Image& dst, double dFa, double dFb)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(uint32_t i = 0; i < src.size(); i++)
        {
            double val = src.at(i);
            val = val * dFa + dFb;
            dst.at(i) = std::min(std::max(static_cast<int>(val), 0), 255);
        }
    }

    void LogTransform(Image& src, Image& dst, double dC)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(uint32_t i = 0; i < src.size(); i++)
        {
            double val = src.at(i);
            val = dC * log(val + 1);
            dst.at(i) = std::min(std::max(static_cast<int>(val), 0), 255);
        }
    }

    void GammaTransform(Image& src, Image& dst, double gamma, double comp)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(uint32_t i = 0; i < src.size(); i++)
        {
            double val = src.at(i);
            val += comp; // compensate
            val /= 255.0; // normalize
            val = pow(val, gamma);
            dst.at(i) = static_cast<uint8_t>(BYTE_BOUND(val * 255.0));
        }
    }

    void HorizontalMirror(Image& src, Image& dst)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(int i = 0; i < src.height(); i++)
            for(int j = 0; j < src.width(); j++)
                for(int k = 0; k < src.channels(); k++)
                    dst.at((i * src.width() + j) * src.channels() + k) = src.at(((i + 1) * src.width() - j) * src.channels() + k);
    }

    void VerticalMirror(Image& src, Image& dst)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        
        for(int i = 0; i < src.height(); i++)
            for(int j = 0; j < src.width(); j++)
                for(int k = 0; k < src.channels(); k++)
                    dst.at((i * src.width() + j) * src.channels() + k) = src.at(((src.height() - i - 1) * src.width() + j) * src.channels() + k);
    }

    void Sepia(Image& src, Image& dst)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(uint64_t i = 0; i < src.size(); i+=src.channels())
        {
            dst.at(i) = static_cast<uint8_t>(BYTE_BOUND(0.393 * src.at(i) + 0.769 * src.at(i+1) + 0.189 * src.at(i+2)));
            dst.at(i + 1) = static_cast<uint8_t>(BYTE_BOUND(0.394 * src.at(i) + 0.686 * src.at(i+1) + 0.168 * src.at(i+2)));
            dst.at(i + 2) = static_cast<uint8_t>(BYTE_BOUND(0.272 * src.at(i) + 0.534 * src.at(i+1) + 0.131 * src.at(i+2)));
        }
    }

    // @TODO doesn't work as intended, fix it
    void ExtractChannel(Image& src, Image& dst, int channel)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || dst.channels() != 1)
            dst.allocate(src.width(), src.height(), 1);

        for(uint32_t i = 0; i < src.size(); i+=3)
            dst.at(i) = src.at(i);
    }
}
