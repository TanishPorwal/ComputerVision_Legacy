#pragma once

#include "Image.h"
#include "Convolution.h"
#include "MathConstants.h"

#include <array>

namespace cv
{
    /**
     * \brief Blurs an image using the normalized box filter.
     * \param src Input Image
     * \param dst Output Image
     * \param size Size of the kernel to be generated, input in the form (width, height)
     */
    void blur(Image& src, Image& dst, std::array<int, 2> size)
    {
        auto kernel = new double[size[0] * size[1]];
        for(int i = 0; i < size[0] * size[1]; i++)
            kernel[i] = 1.0 / (size[0] * size[1]);
        convolve2D(src, dst, kernel, size[0], size[1]);
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
        double r, s = 2 * sigma * sigma;
        double sum = 0.0;
        for(int i = -cr; i < kernel_height - cr; i++)
            for(int j = -cc; j < kernel_width - cc; j++)
            {
                r = i*i + j*j;
                kernel[center + i * kernel_width + j] = exp(-r / s) / (M_PI * s);
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
        convolve2D(src, dst, gaussian_kernel, kernelX, kernelY);
        delete[] gaussian_kernel;
    }
}