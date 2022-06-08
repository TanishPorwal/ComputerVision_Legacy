/*
* ComputerVision Library (https://github.com/BubblyBunny2/ComputerVision).
*
* Copyright (c) 2022 Tanish.
*/

#include "SimdCPU.h"

#if defined(_MSC_VER)
#include <windows.h>
#endif

namespace Simd
{
#ifdef CV_SIMD_AVX2_ENABLE
    namespace Avx2
    {
        CV_INLINE bool SupportedByCPU()
        {
            return
                Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::OSXSAVE) &&
                Base::CheckBit(Cpuid::Extended, Cpuid::Ebx, Cpuid::AVX2) &&
                Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::FMA) &&
                Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::F16C);
        }

        CV_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                __m256i value = _mm256_abs_epi8(_mm256_set1_epi8(1));// try to execute of AVX2 instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        bool GetEnable()
        {
            return SupportedByCPU() && SupportedByOS();
        }
    }
#endif
}
