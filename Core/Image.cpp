#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Image.h"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#if defined(_MSC_VER)
#include "Imgproc/MathConstants.h"
#endif

#include "SimdAllocator.h"

Image::Image(const char* filename)
{
    if(read(filename))
    {
        std::cout << "Successfully Opened " << filename << "\n";
        m_Size = m_Width * m_Height * m_Channels;
    }
    else std::cout << "Could not open" << filename << ".\n" << "Reason: " << stbi_failure_reason() << "\n";
}

Image::Image(int width, int height, int channels) : m_Width(width), m_Height(height), m_Channels(channels)
{
    m_Size = m_Width * m_Height * m_Channels;
    m_Data = new uint8_t[m_Size];
}

Image::Image(int width, int height, int channels, std::array<uint8_t, 3> color) : m_Width(width), m_Height(height), m_Channels(channels)
{
    assert(m_Channels == 3);
    m_Size = m_Width * m_Height * m_Channels;
    m_Data = new uint8_t[m_Size];
    for(uint64_t i = 0; i < m_Size; i++)
    {
        m_Data[i]   = color[0];
        m_Data[i+1] = color[1];
        m_Data[i+2] = color[2];
    }
}

Image::Image(const Image& src) : Image(src.m_Width, src.m_Height, src.m_Channels)
{
    memcpy(m_Data, src.m_Data, src.m_Size);
}

Image::Image(Image&& src) : Image(src.m_Width, src.m_Height, src.m_Channels)
{
    m_Data = src.m_Data;
    src.m_Width = 0;
    src.m_Height = 0;
    src.m_Channels = 0;
    src.m_Size = 0;
    src.m_Data = nullptr;
    delete src.m_Data;
}

Image& Image::operator=(const Image& src)
{
    if(&src != this)
    {
        m_Width = src.m_Width;
        m_Height = src.m_Height;
        m_Channels = src.m_Channels;
        m_Size = src.m_Size;
        m_Data = new uint8_t[m_Size];
        memcpy(m_Data, src.m_Data, src.m_Size);
    }
    return *this;
}

Image& Image::operator=(Image&& src)
{
    m_Width = src.m_Width;
    m_Height = src.m_Height;
    m_Channels = src.m_Channels;
    m_Size = src.m_Size;
    m_Data = new uint8_t[m_Size];
    m_Data = src.m_Data;
    src.m_Width = 0;
    src.m_Height = 0;
    src.m_Channels = 0;
    src.m_Size = 0;
    src.m_Data = nullptr;
    delete src.m_Data;
    return *this;
}

Image::~Image()
{
    stbi_image_free(m_Data);
}

Image& Image::open(const char* filename)
{
    Image tmp(filename);
    *this = std::move(tmp);
    return *this;
}


Image& Image::clear()
{
    for(uint32_t i = 0; i < m_Size; i++)
        m_Data[i] = 0;
    return *this;
}

Image& Image::set(uint8_t value)
{
    for(uint32_t i = 0; i < m_Size; i++)
        m_Data[i] = value;
    return *this;
}

bool Image::read(const char* filename)
{
    m_Data = stbi_load(filename, &m_Width, &m_Height, &m_Channels, 0);
    return m_Data != nullptr;
}

bool Image::write(const char* filename)
{
    ImageFileExtension type = getFileType(filename);
    int success = -1;
    switch(type)
    {
    case ImageFileExtension::PNG: success = stbi_write_png(filename, m_Width, m_Height, m_Channels, m_Data, m_Width * m_Channels);
        break;
    case ImageFileExtension::JPG: success = stbi_write_jpg(filename, m_Width, m_Height, m_Channels, m_Data, 100);
        break;
    case ImageFileExtension::BMP: success = stbi_write_bmp(filename, m_Width, m_Height, m_Channels, m_Data);
        break;
    case ImageFileExtension::TGA: success = stbi_write_tga(filename, m_Width, m_Height, m_Channels, m_Data);
        break;
    }
    return success != 0;
}

ImageFileExtension Image::getFileType(const char* filename)
{
    const char* ext = strrchr(filename, '.');
    if(ext != nullptr)
    {
        if(strcmp(ext, ".png") == 0) return ImageFileExtension::PNG;
        if(strcmp(ext, ".jpg") == 0) return ImageFileExtension::JPG;
        if(strcmp(ext, ".bmp") == 0) return ImageFileExtension::BMP;
        if(strcmp(ext, ".tga") == 0) return ImageFileExtension::TGA;
    }
    return ImageFileExtension::JPG;
}

uint8_t& Image::at(uint64_t index) const
{
    return m_Data[index];
}

size_t Image::size() const
{
    return m_Size;
}

int Image::width() const
{
    return m_Width;
}

int Image::height() const
{
    return m_Height;
}

int Image::channels() const
{
    return m_Channels;
}

void Image::allocate(int width, int height, int channels)
{
    m_Width = width;
    m_Height = height;
    m_Channels = channels;
    m_Size = width*height*channels;
    delete m_Data;
    m_Data = new uint8_t[m_Size];
}

void Image::simdAllocate(int width, int height, int channels, size_t alignment)
{
    if(m_Data == nullptr)
    {
        m_Width = width;
        m_Height = height;
        m_Channels = channels;
        m_Size = width * height * channels;
//        m_Data = static_cast<uint8_t*>(Simd::SimdAllocator<uint8_t>(m_Size, CV_SIMD_ALIGN));
        m_Data = static_cast<uint8_t*>(Simd::SimdAllocator<uint8_t>::Allocate(m_Size, CV_SIMD_ALIGN));
    }
}
