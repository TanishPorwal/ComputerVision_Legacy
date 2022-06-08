/*
* ComputerVision Library (https://github.com/BubblyBunny2/ComputerVision).
*
* Copyright (c) 2022 Tanish.
*/


#pragma once

#include "SimdDefs.h"
#include "SimdSSE2Enable.h"
#include "SimdSSE41Enable.h"
#include "SimdAVXEnable.h"
#include "SimdAVX2Enable.h"
// TODO implement the functions
namespace Simd
{
//#ifdef CV_SIMD_SSE2_ENABLE
//namespace Sse2
//    {
//        const bool Enable = GetEnable();
//    }
//#endif

#ifdef CV_SIMD_SSE41_ENABLE
namespace Sse41
    {
        const bool Enable = GetEnable();
    }
#endif

#ifdef CV_SIMD_AVX_ENABLE
namespace Avx
    {
        const bool Enable = GetEnable();
    }
#endif

#ifdef CV_SIMD_AVX2_ENABLE
namespace Avx2
    {
        const bool Enable = GetEnable();
    }
#endif
}