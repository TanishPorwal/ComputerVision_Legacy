#pragma once

#include <algorithm>

#include "Image.h"

#include "ColorConversion.h"

namespace cv
{
    Image& cvtColor(Image& src, Image& dst, ColorConversionCodes conversion_codes)
    {
        switch(conversion_codes) {
        case ColorConversionCodes::COLOR_RGB2RGBA: break;
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
