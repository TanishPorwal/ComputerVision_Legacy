/*
* ComputerVision Library (https://github.com/BubblyBunny2/ComputerVision).
*
* Copyright (c) 2022 Tanish.
*/


#pragma once

#include "SimdEnable.h"

namespace Simd
{
    CV_INLINE size_t GetAlignment()
    {
#ifdef CV_SIMD_AVX2_ENABLE
        if (Avx2::Enable)
            return sizeof(__m256i);
        else
#endif
#ifdef CV_SIMD_AVX_ENABLE
        if (Avx::Enable)
            return sizeof(__m256);
        else
#endif
#ifdef CV_SIMD_SSE41_ENABLE
        if (Sse41::Enable)
            return sizeof(__m128i);
        else
#endif
//#ifdef CV_SIMD_SSE2_ENABLE
//        if (Sse2::Enable)
//            return sizeof(__m128i);
//        else
//#endif
        return sizeof(void *);
    }

    extern const size_t ALIGNMENT;

    CV_INLINE size_t Alignment()
    {
#if defined(WIN32)
        return GetAlignment();
#else
        return ALIGNMENT;
#endif
    }
}
