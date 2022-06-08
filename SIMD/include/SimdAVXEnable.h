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
#ifdef CV_SIMD_AVX_ENABLE
    namespace Avx
    {
        CV_INLINE bool SupportedByCPU()
        {
            return
                    Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::AVX) &&
                    Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::OSXSAVE);
        }

        CV_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
                __try
            {
                __m256d value = _mm256_set1_pd(1.0);// try to execute of AVX instructions;
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
