#pragma once
#include <iostream>

#include "Image.h"

// Image is assumed to be 3 channels
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
    gpuImage& operator=(const gpuImage& src);
    gpuImage& operator=(gpuImage&& src) noexcept;

private:
    uint8_t* m_Data = nullptr;
};
