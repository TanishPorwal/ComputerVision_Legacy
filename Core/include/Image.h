#pragma once
#include <iostream>
#include <array>
#include <cmath>

#define BYTE_BOUND(value) value < 0 ? 0 : ((value) > 255 ? 255 : (value))

enum class ImageFileExtension
{
    PNG,
    JPG,
    BMP,
    TGA
};

class Image
{
public:
    Image() = default;
    Image(const char* filename);
    Image(int width, int height, int channels);
    Image(int width, int height, int channels, std::array<uint8_t, 3> color);
    Image(const Image& src);
    Image(Image&& src);
    Image& operator=(const Image& src);
    Image& operator=(Image&& src);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);

    ImageFileExtension getFileType(const char* filename);

    Image& open(const char* filename);

    /**
     * \brief sets all the pixels in the image to the given value
     */
    Image& set(uint8_t value);
    Image& clear();
    
    uint8_t& at(uint64_t index) const;
    size_t size() const;
    int width() const;
    int height() const;
    int channels() const;

    void allocate(int width, int height, int channels);
    void simdAllocate(int width, int height, int channels, size_t alignment);

private:
    uint8_t* m_Data = nullptr;
    size_t m_Size = 0;
    int m_Width = 0, m_Height = 0, m_Channels = 0;
};
