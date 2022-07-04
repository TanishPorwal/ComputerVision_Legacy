#include <iostream>

#include "Image.h"
#include "Imgproc/ColorConversion.h"
#include "Imgproc/Convolution.h"

#include "Timer.h"

#include "Imgproc/ArithmeticOperations.h"
#include "Imgproc/Blur.h"
#include "Imgproc/ImageOperations.h"

#include "Imgproc/Canny.h"
#include "Window.h"

#include "Imgproc/DrawShapes.h"

#include "SimdAllocator.h"

int main()
{
//    double emboss[9] = { -2,  -1,  0,
//                         -1,   1,  1,
//                          0,   1,  2};
//
//    double sharpen[9] = { 0, -1,  0,
//                         -1,  5, -1,
//                          0, -1,  0};
//
//    double raised[9] = {  0,  0, -2,
//                          0,  2,  0,
//                          1,  0,  0};
//
//    double edge_detect[9] = {  0, -1,  0,
//                              -1,  4, -1,
//                               0, -1,  0};
//
//    double outline[9] = { -1/3.0, -1/3.0, -1/3.0,
//                          -1/3.0,  8/3.0, -1/3.0,
//                          -1/3.0, -1/3.0, -1/3.0};
//
//    double denoise[9] = {1/9.0, 1/9.0, 1/9.0,
//                         1/9.0, 1/9.0, 1/9.0,
//                         1/9.0, 1/9.0, 1/9.0};

    Timer timer;
    Image test1("Resources/Lena.png"), test2;
    cv::canny(test1, test2, 100, 200);
    test1.write("Resources/Lena_canny.png");
//    std::cout << "Alignment Value: " << CV_SIMD_ALIGN << "\n";
//    std::cout << "Alignment: " << Simd::GetAlignment() << "\n";
//    auto a = static_cast<uint8_t*>(Simd::SimdAllocator<uint8_t>::Allocate(test1.size(), CV_SIMD_ALIGN)), b = static_cast<uint8_t*>(Simd::SimdAllocator<uint8_t>::Allocate(test1.size(), CV_SIMD_ALIGN));
//    Simd::Free(a);
//    Simd::Free(b);
    //    cv::CvtColor(test1, test1, ColorConversionCodes::COLOR_RGB2RGBA);

//    bool b = ((unsigned long)a & CV_SIMD_ALIGN) == 0;
//    std::cout << std::boolalpha << b << "\n";



    return 0;
}
