#pragma once

#include "Image.h"

namespace cv
{
    /**
     * \brief Performs a convolution on a channel of the Image
     * \param src Input Image
     * \param dst Output Image
     * \param channel Channel to be processed
     * \param kernel Desired Filter Kernel
     * \param kernel_width Width of the kernel
     * \param kernel_height Height of the Kernel
     */
    void filter2D(Image& src, Image& dst, uint8_t channel , double* kernel, int kernel_width, int kernel_height)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        int cr = static_cast<int>(floor(kernel_height/2)), cc = static_cast<int>(floor(kernel_width/2));
        int center = cr * kernel_height + cc;
        
        for(uint64_t k = channel; k < src.size(); k+=src.channels())
        {
            double sum = 0.0;
            for(int i = -cr; i < kernel_height-cr; ++i)
            {
                int row = static_cast<int>( k / src.channels() / src.width() - i);
                if(row < 0)
                    row = 0;
                else if(row > src.height() - 1)
                    row = src.height() - 1;
                for(int j = -cc; j < kernel_width - cc; ++j)
                {
                    int col = k / src.channels() % src.width() - j;
                    if(col < 0)
                        col = 0;
                    else if(col > src.width() - 1)
                        col = src.width() - 1;
                    sum += kernel[center + i * kernel_width + j] * src.at((row * src.width() + col)*src.channels() + channel);
                }
            }
            dst.at(k/src.channels()) = static_cast<uint8_t>(BYTE_BOUND(round(sum)));
        }
        for(uint64_t i = channel; i < src.size(); i+=src.channels())
            src.at(i) = dst.at(i/src.channels());
    }

    /**
     * \brief Performs a convolution on all the channels of the Image
     * \param src Input Image
     * \param dst Destination Image
     * \param kernel Desired Filter Kernel
     * \param kernel_width Width of the kernel
     * \param kernel_height Height of the Kernel
     */
    void filter2D(Image& src, Image& dst, double* kernel, int kernel_width, int kernel_height)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());

        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        
        int cr = static_cast<int>(floor(kernel_height/2)), cc = static_cast<int>(floor(kernel_height/2));
        int center = cr * kernel_width + cc;

        for(uint64_t k = 0; k < src.size(); k+=src.channels())
        {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for(int i = -cr; i < kernel_height - cr; ++i)
            {
                int row = static_cast<int>(k / src.channels() / src.width() - i);
                if(row < 0)
                    row = 0;
                else if(row > src.height() - 1)
                    row = src.height() - 1;
                for(int j = -cc; j < kernel_width - cc; ++j)
                {
                    int col = static_cast<int>(k / src.channels() % src.width() - j);
                    if(col < 0)
                        col  = 0;
                    else if(col > src.width() - 1)
                        col = src.width() - 1;
                    sumR += kernel[center + i * kernel_width + j] * src.at((row * src.width() + col) * src.channels() + 0);
                    sumG += kernel[center + i * kernel_width + j] * src.at((row * src.width() + col) * src.channels() + 1);
                    sumB += kernel[center + i * kernel_width + j] * src.at((row * src.width() + col) * src.channels() + 2);
                }
            }
            dst.at(k)   = static_cast<uint8_t>(BYTE_BOUND(round(sumR)));
            dst.at(k+1) = static_cast<uint8_t>(BYTE_BOUND(round(sumG)));
            dst.at(k+2) = static_cast<uint8_t>(BYTE_BOUND(round(sumB)));
        }
    }

    // TODO Optimization
    void filter2D_test(Image& src, Image& dst, double* kernel, int kernel_width, int kernel_height)
    {
        if(src.height() != dst.height() || src.width() != dst.width() || src.channels() != dst.channels())
            dst.allocate(src.width(), src.height(), src.channels());
        int cr = static_cast<int>(floor(kernel_height/2)), cc = static_cast<int>(floor(kernel_height/2));
        int center = cr * kernel_width + cc;

        for(uint64_t k = 0; k < src.size(); k++)
        {
            double sum = 0.0;
            for(int i = -cr; i < kernel_height- cr; ++i)
            {
                int row = static_cast<int>(k / src.channels() / src.width() - i);
                if(row < 0)
                    row = 0;
                else if(row > src.height() - 1)
                    row = src.height() - 1;
                for(int j = -cc; j < kernel_width - cc; ++j)
                {
                    int col =  static_cast<int>(k / src.channels() % src.width() - j);
                    if(col < 0)
                        col = 0;
                    else if(col > src.height() - 1)
                        col = src.height() - 1;
                    sum += kernel[center + i * kernel_width + j] * src.at(row * src.width() + col);
                }
            }
            dst.at(k) = static_cast<uint8_t>(BYTE_BOUND(round(sum)));
        }
    }
    
    /**
     * \brief Generates a Gaussian Kernel of desired height and width.
     * \param kernel Array in which the Gaussian Filter values has to filled.
     * \param kernel_width Desired width of the Gaussian Kernel.
     * \param kernel_height Desired height of the Gaussian Kernel.
     * \param sigma Gaussian kernel standard deviation.
     */
    void generateGaussianKernel(double* kernel, int kernel_width, int kernel_height, uint32_t sigma)
    {
        int cc = static_cast<int>(floor(kernel_width/2)), cr = static_cast<int>(floor(kernel_height/2));
        int center = cr * kernel_height + cc;
        constexpr double pi = 3.14159265;
        double r, s = 2 * sigma * sigma;
        double sum = 0.0;
        for(int i = -cr; i < kernel_height - cr; i++)
            for(int j = -cc; j < kernel_width - cc; j++)
            {
                r = i*i + j*j;
                kernel[center + i * kernel_width + j] = exp(-r / s) / (pi * s);
                sum += kernel[center + i * kernel_width + j];
            }
        for(int i = 0; i < kernel_height; i++)
            for(int j = 0; j < kernel_width; j++)
                kernel[i*kernel_width + j] /= sum;
    }

    // TODO Implement the function for optimization of Harris Corner Detector  
    void generate1DGaussianKernel(double* kernel, int kernel_width, int kernel_height, uint32_t sigma)
    {
        
    }

    void gaussianBlur(Image& src, Image& dst, int kernelX, int kernelY, uint32_t sigma)
    {
        auto* gaussian_kernel = new double[kernelX * kernelY];
        generateGaussianKernel(gaussian_kernel, kernelX, kernelY, sigma);
        filter2D(src, dst, gaussian_kernel, kernelX, kernelY);
        delete[] gaussian_kernel;
    }

}
