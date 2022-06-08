///*
//* ComputerVision Library (https://github.com/BubblyBunny2/ComputerVision).
//*
//* Copyright (c) 2022 Tanish.
//*/
//
//#include "SimdEnable.h"
//#include "SimdCPU.h"
//
//#if defined(_MSC_VER)
//#include <windows.h>
//#endif
//
//namespace Simd
//{
//#ifdef CV_SIMD_SSE2_ENABLE
//    namespace Sse2
//    {
//        CV_INLINE bool SupportedByCPU()
//        {
//            return Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::SSE2);
//        }
//
//        CV_INLINE bool SupportedByOS()
//        {
//#if defined(_MSC_VER)
//            __try
//            {
//                int value = _mm_testz_si128(_mm_set1_epi8(0), _mm_set1_epi8(-1)); // try to execute of SSE41 instructions;
//                uint32_t crc = _mm_crc32_u8(0, 1); // try to execute of SSE42 instructions;
//                return true;
//            }
//            __except (EXCEPTION_EXECUTE_HANDLER)
//            {
//                return false;
//            }
//#else
//            return true;
//#endif
//        }
//
//        bool GetEnable()
//        {
//            return SupportedByCPU() && SupportedByOS();
//        }
//    }
//#endif
//}