#pragma once
#include <iostream>

#include "Image.h"

template<size_t channels>
class gpuImage
{
public:
    gpuImage() = default;
    gpuImage(Image& src);
    gpuImage(Image&& src) noexcept;
    gpuImage(gpuImage& src);
    gpuImage(gpuImage&& src) noexcept;
    ~gpuImage();

    void load(Image& src);
    void load(uint8_t* data, int cols, int rows, int channel);

    gpuImage& operator=(Image& src);
    gpuImage& operator=(Image&& src) noexcept;
    gpuImage& operator=(gpuImage& src);
    gpuImage& operator=(gpuImage&& src) noexcept;

private:
    struct parameters
    {
        uint8_t r, g, b;
    };

private:
    parameters* m_Data = nullptr;
};
