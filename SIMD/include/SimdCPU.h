/*
* ComputerVision Library (https://github.com/BubblyBunny2/ComputerVision).
*
* Copyright (c) 2022 Tanish.
*/

#pragma once

#include "SimdDefs.h"

typedef enum
{
    SimdFalse = 0, /*!< False value. */
    SimdTrue = 1, /*!< True value. */
} SimdBool;

#if defined(_MSC_VER)

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <intrin.h>

#elif defined(__GNUC__)
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(CV_SIMD_X86_ENABLE) || defined(CV_SIMD_X64_ENABLE)
#include <cpuid.h>
#endif
#endif

namespace Simd {
#if defined(CV_SIMD_X86_ENABLE) || defined(CV_SIMD_X64_ENABLE)
    namespace Cpuid {
        // See http://www.sandpile.org/x86/cpuid.htm for additional information.
        enum Level {
            Ordinary = 1,
            Extended = 7,
        };

        enum Register {
            Eax = 0,
            Ebx = 1,
            Ecx = 2,
            Edx = 3,
        };

        enum Bit {
            // Ordinary:
            // Edx:
            SSE = 1 << 25,
            SSE2 = 1 << 26,

            // Ecx:
            SSE3 = 1 << 0,
            SSSE3 = 1 << 9,
            FMA = 1 << 12,
            SSE41 = 1 << 19,
            SSE42 = 1 << 20,
            OSXSAVE = 1 << 27,
            AVX = 1 << 28,
            F16C = 1 << 29,

            // Extended:
            // Ebx:
            AVX2 = 1 << 5,
            AVX512F = 1 << 16,
            AVX512DQ = 1 << 17,
            AVX512CD = 1 << 28,
            AVX512BW = 1 << 30,
            AVX512VL = 1 << 31,
        };
    }
#endif//defined(CV_SIMD_X86_ENABLE) || defined(CV_SIMD_X64_ENABLE)

    namespace Cpu
    {
        extern const size_t SOCKET_NUMBER;
        extern const size_t CORE_NUMBER;
        extern const size_t THREAD_NUMBER;
        extern const size_t L1_CACHE_SIZE;
        extern const size_t L2_CACHE_SIZE;
        extern const size_t L3_CACHE_SIZE;
    }
    namespace Base
    {
#if defined(CV_SIMD_X86_ENABLE) || defined(CV_SIMD_X64_ENABLE)
        bool CheckBit(Cpuid::Level level, Cpuid::Register index, Cpuid::Bit bit);
#endif
        size_t CpuSocketNumber();

        size_t CpuCoreNumber();

        size_t CpuThreadNumber();

        size_t CpuCacheSize(size_t level);

        CV_INLINE size_t AlgCacheL1()
        {
            return Cpu::L1_CACHE_SIZE;
        }

        CV_INLINE size_t AlgCacheL2()
        {
            return Cpu::L3_CACHE_SIZE ? Cpu::L2_CACHE_SIZE : Cpu::L2_CACHE_SIZE * Cpu::SOCKET_NUMBER / Cpu::CORE_NUMBER;
        }

        CV_INLINE size_t AlgCacheL3()
        {
            return Cpu::L3_CACHE_SIZE ? Cpu::L3_CACHE_SIZE * Cpu::SOCKET_NUMBER / Cpu::CORE_NUMBER : Cpu::L2_CACHE_SIZE;
        }
    }

#ifdef CV_SIMD_SSE2_ENABLE
    namespace Sse2
    {
        const unsigned int SCR_FTZ = 1 << 15;
        const unsigned int SCR_DAZ = 1 << 6;

        CV_INLINE SimdBool GetFastMode()
        {
            return _mm_getcsr() & (SCR_FTZ | SCR_DAZ) ? SimdTrue : SimdFalse;
        }

        CV_INLINE void SetFastMode(SimdBool value)
    {
        if (value)
        _mm_setcsr(_mm_getcsr() | (SCR_FTZ | SCR_DAZ));
        else
        _mm_setcsr(_mm_getcsr() & ~(SCR_FTZ | SCR_DAZ));
    }
}
#endif
}