/*
* ComputerVision Library (https://github.com/BubblyBunny2/ComputerVision).
*
* Copyright (c) 2022 Tanish.
*/

#if defined(_MSC_VER) || defined(__CODEGEARC__)

#define CV_INLINE __forceinline

#elif defined(__GNUC__)

#define CV_INLINE inline __attribute__ ((always_inline))

#else

#error This platform is unsupported!

#endif

#if defined(__GNUC__) || (defined(_MSC_VER) && (_MSC_VER >= 1600)) || (defined(__CODEGEARC__) && (__CODEGEARC__ >= 1840))
#include <cstdint>
#else
#  if (_MSC_VER < 1300)
typedef signed char       int8_t;
typedef signed short      int16_t;
typedef signed int        int32_t;
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
#  else
typedef signed __int8     int8_t;
typedef signed __int16    int16_t;
typedef signed __int32    int32_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
#  endif
typedef signed __int64    int64_t;
typedef unsigned __int64  uint64_t;
#endif

#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <memory.h>
#include <climits>
#include <cfloat>
#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>

#if defined(CV_SIMD_SSE2_DISABLE) && !defined(CV_SIMD_SSE41_DISABLE)
#define CV_SIMD_SSE41_DISABLE
#endif

#if defined(CV_SIMD_SSE41_DISABLE) && !defined(CV_SIMD_AVX_DISABLE)
#define CV_SIMD_AVX_DISABLE
#endif

#if defined(CV_SIMD_AVX_DISABLE) && !defined(CV_SIMD_AVX2_DISABLE)
#define CV_SIMD_AVX2_DISABLE
#endif

#if defined(_MSC_VER) && defined(_MSC_FULL_VER)

#define CV_SIMD_ALIGNED(x) __declspec(align(x))

#define CV_SIMD_NOINLINE __declspec(noinline)

#ifdef _M_IX86
#define CV_SIMD_X86_ENABLE
#endif

#if defined(_M_X64) || defined(_M_AMD64)
#define CV_SIMD_X64_ENABLE
#endif

#if defined(CV_SIMD_X64_ENABLE) || defined(CV_SIMD_X86_ENABLE)

#if !defined(CV_SIMD_SSE2_DISABLE) && _MSC_VER >= 1300
#define CV_SIMD_SSE2_ENABLE
#endif

#if !defined(CV_SIMD_SSE41_DISABLE) && _MSC_VER >= 1500
#define CV_SIMD_SSE41_ENABLE
#endif

#if !defined(CV_SIMD_AVX_DISABLE) && _MSC_FULL_VER >= 160040219
#define CV_SIMD_AVX_ENABLE
#endif

#if !defined(CV_SIMD_AVX2_DISABLE) && _MSC_VER >= 1700
#define CV_SIMD_AVX2_ENABLE
#endif

#endif//defined(CV_SIMD_X64_ENABLE) || defined(CV_SIMD_X86_ENABLE)

#if _MSC_VER >= 1900
#define CV_SIMD_CPP_2011_ENABLE
#endif

#if _MSVC_LANG >= 201402L
#define CV_SIMD_CPP_2014_ENABLE
#endif

#if _MSVC_LANG >= 201703L
#define CV_SIMD_CPP_2017_ENABLE
#endif

#elif defined(__GNUC__)

#define CV_SIMD_ALIGNED(x) __attribute__ ((aligned(x)))

#define CV_SIMD_NOINLINE __attribute__ ((noinline))

#ifdef __i386__
#define CV_SIMD_X86_ENABLE
#endif

#if defined(__x86_64__) || defined(__amd64__)
#define CV_SIMD_X64_ENABLE
#endif

#ifdef __BIG_ENDIAN__
#define CV_SIMD_BIG_ENDIAN
#endif

#if defined(CV_SIMD_X86_ENABLE) || defined(CV_SIMD_X64_ENABLE)

#if !defined(CV_SIMD_SSE2_DISABLE) && defined(__SSE__) && defined(__SSE2__)
#define CV_SIMD_SSE2_ENABLE
#endif

#if !defined(CV_SIMD_SSE41_DISABLE) && defined(__SSE3__) && defined(__SSSE3__) && defined(__SSE4_1__) && defined(__SSE4_2__)
#define CV_SIMD_SSE41_ENABLE
#endif

#if !defined(CV_SIMD_AVX_DISABLE) && defined(__AVX__)
#define CV_SIMD_AVX_ENABLE
#endif

#if !defined(CV_SIMD_AVX2_DISABLE) && defined(__AVX2__)
#define CV_SIMD_AVX2_ENABLE
#endif

#endif

#endif//defined(CV_SIMD_X86_ENABLE) || defined(CV_SIMD_X64_ENABLE)

#if __cplusplus >= 201103L
#define CV_SIMD_CPP_2011_ENABLE
#endif

#if __cplusplus >= 201402L
#define CV_SIMD_CPP_2014_ENABLE
#endif

#if __cplusplus >= 201703L
#define CV_SIMD_CPP_2017_ENABLE
#endif

#define CV_SIMD_FUNCTION __PRETTY_FUNCTION__

#ifdef CV_SIMD_SSE2_ENABLE
#include <emmintrin.h>
#endif

#ifdef CV_SIMD_SSE41_ENABLE
#include <nmmintrin.h>
#endif

#if defined(CV_SIMD_AVX_ENABLE) || defined(CV_SIMD_AVX2_ENABLE)
#include <immintrin.h>
#endif

#if defined(CV_SIMD_AVX_ENABLE) || defined(CV_SIMD_AVX2_ENABLE)
#define CV_SIMD_ALIGN 32
#elif defined(CV_SIMD_SSE2_ENABLE) || defined(CV_SIMD_SSE41_ENABLE)
#define CV_SIMD_ALIGN 16
#else
#define CV_SIMD_ALIGN 4
#endif

#define CV_SIMD_CAT_DO(a, b) a##b
#define CV_SIMD_CAT(a, b) CV_SIMD_CAT_DO(a, b)

#if defined(CV_SIMD_AVX_ENABLE) || defined(CV_SIMD_AVX2_ENABLE)
#define CV_SIMD_ALIGN 32
#elif defined(CV_SIMD_SSE2_ENABLE) || defined(CV_SIMD_SSE41_ENABLE)
#define CV_SIMD_ALIGN 16
#else
#define CV_SIMD_ALIGN 4
#endif

#define CV_SIMD_LOG_ENABLE

#define CV_SIMD_ALLOCATE_ERROR_MESSAGE

#define CV_SIMD_ALLOCATE_ASSERT

#define CV_SIMD_NO_MANS_LAND 64