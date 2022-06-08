/*
* ComputerVision Library (https://github.com/BubblyBunny2/ComputerVision).
*
* Copyright (c) 2022 Tanish.
*/

#if defined(SIMD_ALLOCATE_ERROR_MESSAGE)
#include <iostream>
#endif

#include <memory>
#include <iostream>

#include "SimdDefs.h"

namespace Simd {
    CV_INLINE size_t DivHi(size_t value, size_t divider) {
        return (value + divider - 1) / divider;
    }

    CV_INLINE size_t Pow2Hi(size_t value) {
        size_t pow2 = 1;
        for (; pow2 < value; pow2 *= 2);
        return pow2;
    }

    CV_INLINE size_t AlignHiAny(size_t size, size_t align) {
        return (size + align - 1) / align * align;
    }

    CV_INLINE size_t AlignLoAny(size_t size, size_t align) {
        return size / align * align;
    }

    CV_INLINE size_t AlignHi(size_t size, size_t align) {
        return (size + align - 1) & ~(align - 1);
    }

    CV_INLINE void *AlignHi(const void *ptr, size_t align) {
        return (void *) ((size_t) ptr + align - 1 & ~(align - 1));
    }

    CV_INLINE size_t AlignLo(size_t size, size_t align) {
        return size & ~(align - 1);
    }

    CV_INLINE void *AlignLo(const void *ptr, size_t align) {
        return (void *) (((size_t) ptr) & ~(align - 1));
    }

    CV_INLINE bool Aligned(size_t size, size_t align) {
        return size == AlignLo(size, align);
    }

    CV_INLINE bool Aligned(const void *ptr, size_t align) {
        return ptr == AlignLo(ptr, align);
    }

    CV_INLINE void *Allocate(size_t size, size_t align = CV_SIMD_ALIGN) {
#ifdef SIMD_NO_MANS_LAND
        size += 2 * SIMD_NO_MANS_LAND;
#endif
        void *ptr = NULL;
#if defined(_MSC_VER)
        ptr = _aligned_malloc(size, align);
#elif defined(__MINGW32__) || defined(__MINGW64__)
        ptr = __mingw_aligned_malloc(size, align);
#elif defined(__GNUC__)
        align = AlignHi(align, sizeof(void *));
        size = AlignHi(size, align);
        int result = ::posix_memalign(&ptr, align, size);
        if (result != 0)
            ptr = NULL;
#else
        ptr = malloc(size);
#endif
#ifdef SIMD_ALLOCATE_ERROR_MESSAGE
        if (ptr == NULL)
            std::cout << "The function posix_memalign can't allocate " << size << " bytes with align " << align << " !"
                      << std::endl << std::flush;
#endif
#ifdef SIMD_ALLOCATE_ASSERT
        assert(ptr);
#endif
#ifdef SIMD_NO_MANS_LAND
        if (ptr)
            ptr = (char *) ptr + SIMD_NO_MANS_LAND;
#endif
        return ptr;
    }

    template<class T>
    T *Allocate(uint8_t *&buffer, size_t size, size_t align = CV_SIMD_ALIGN) {
        T *ptr = (T *) buffer;
        buffer = buffer + AlignHi(size * sizeof(T), align);
        return ptr;
    }

    CV_INLINE void Free(void *ptr) {
#ifdef SIMD_NO_MANS_LAND
        if (ptr)
            ptr = (char *) ptr - SIMD_NO_MANS_LAND;
#endif
#if defined(_MSC_VER)
        _aligned_free(ptr);
#elif defined(__MINGW32__) || defined(__MINGW64__)
        return __mingw_aligned_free(ptr);
#else
        free(ptr);
#endif
    }

//---------------------------------------------------------------------------------------------

    struct Deletable {
        virtual ~Deletable() {}
    };

//---------------------------------------------------------------------------------------------

#if defined(SIMD_CPP_2011_ENABLE)
    template<class T> using Holder = std::unique_ptr<T>;
#else

    template<class T>
    class Holder {
        T *_ptr;

    public:
        Holder(T *ptr)
                : _ptr(ptr) {
        }

        ~Holder() {
            if (_ptr)
                delete _ptr;
        }

        T &operator*() {
            return *_ptr;
        }

        const T &operator*() const {
            return *_ptr;
        }

        T *operator->() {
            return _ptr;
        }

        const T *operator->() const {
            return _ptr;
        }

        operator bool() const {
            return _ptr != NULL;
        }
    };

#endif

//---------------------------------------------------------------------------------------------


#ifdef SIMD_SSE2_ENABLE
    namespace Sse2
        {
            SIMD_INLINE bool Aligned(size_t size, size_t align = sizeof(__m128))
            {
                return Simd::Aligned(size, align);
            }

            SIMD_INLINE bool Aligned(const void * ptr, size_t align = sizeof(__m128))
            {
                return Simd::Aligned(ptr, align);
            }
        }
#endif// SIMD_SSE2_ENABLE

#ifdef SIMD_SSE41_ENABLE
    namespace Sse41
        {
            using Sse2::Aligned;
        }
#endif// SIMD_SSE41_ENABLE

#ifdef SIMD_AVX_ENABLE
    namespace Avx
        {
            SIMD_INLINE bool Aligned(size_t size, size_t align = sizeof(__m256))
            {
                return Simd::Aligned(size, align);
            }

            SIMD_INLINE bool Aligned(const void * ptr, size_t align = sizeof(__m256))
            {
                return Simd::Aligned(ptr, align);
            }
        }
#endif// SIMD_AVX_ENABLE

#ifdef SIMD_AVX2_ENABLE
    namespace Avx2
        {
            using Avx::Aligned;
        }
#endif// SIMD_AVX2_ENABLE
}