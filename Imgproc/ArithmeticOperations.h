#pragma once
#include "Image.h"

namespace cv
{
    ////////////            Arithmetic Operators            ////////////
    /**
     * \brief Linear Blending 2 images where size of src1 must be equal to size of src2 i.e height, width and channels must be same
     * \param src1 Input Image 1
     * \param alpha By varying α from 0 → 1 this operator can be used to perform a temporal cross-dissolve between two images or videos, as seen in slide shows and film productions
     * \param src2 Input Image 2
     * \param beta 1-alpha
     * \param gamma .
     * \param dst Output Image
     */
    void addWeighted(Image& src1, double alpha, Image& src2, double beta, double gamma, Image& dst)
    {
        if(src1.height() != dst.height() || src1.width() != dst.width() || src1.channels() != dst.channels())
            dst.allocate(src1.width(), src1.height(), src1.channels());

        for(uint64_t i = 0; i < src1.size(); i++)
            dst.at(i) = static_cast<uint8_t>(BYTE_BOUND(alpha * src1.at(i) + beta * src2.at(i) + gamma));
    }

    void add(Image& src1, Image& src2, Image& dst)
    {
        if(src1.height() != dst.height() || src1.width() != dst.width() || src1.channels() != dst.channels())
            dst.allocate(src1.width(), src1.height(), src1.channels());

        for(uint64_t i = 0; i < src1.size(); i++)
            dst.at(i) = static_cast<uint8_t>(BYTE_BOUND(src1.at(i) + src2.at(i)));
    }

    void subtract(Image& src1, Image& src2, Image& dst)
    {
        if(src1.height() != dst.height() || src1.width() != dst.width() || src1.channels() != dst.channels())
            dst.allocate(src1.width(), src1.height(), src1.channels());

        for(uint64_t i = 0; i < src1.size(); i++)
            dst.at(i) = static_cast<uint8_t>(BYTE_BOUND(src1.at(i) - src2.at(i)));
    }


    ////////////            Bitwise Operators            ////////////
    /**
     * \brief Performs bitwise not on every element of the image
     * \param src Input Image
     * \param dst Output Image
     */
    void bitwise_not(Image& src, Image& dst)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        for(uint32_t i = 0; i < src.size(); i++)
            dst.at(i) = ~src.at(i);
    }

    void bitwise_and(Image& src1, Image& src2, Image& dst)
    {
        if(src1.height() != dst.height() || src1.width() != dst.width() || src1.channels() != dst.channels())
            dst.allocate(src1.width(), src1.height(), src1.channels());

        for(uint64_t i = 0; i < src1.size(); i++)
            dst.at(i) = src1.at(i) & src2.at(i);
    }

    void bitwise_or(Image& src1, Image& src2, Image& dst)
    {
        if(src1.height() != dst.height() || src1.width() != dst.width() || src1.channels() != dst.channels())
            dst.allocate(src1.width(), src1.height(), src1.channels());

        for(uint64_t i = 0; i < src1.size(); i++)
            dst.at(i) = src1.at(i) | src2.at(i);
    }

    void bitwise_xor(Image& src1, Image& src2, Image& dst)
    {
        if(src1.height() != dst.height() || src1.width() != dst.width() || src1.channels() != dst.channels())
            dst.allocate(src1.width(), src1.height(), src1.channels());

        for(uint64_t i = 0; i < src1.size(); i++)
            dst.at(i) = src1.at(i) ^ src2.at(i);
    }
   
}
