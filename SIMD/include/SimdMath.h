/*
* ComputerVision Library (https://github.com/BubblyBunny2/ComputerVision).
*
* Copyright (c) 2022 Tanish.
*/

#include "SimdDefs.h"

namespace Simd
{
    template<class T>
    CV_INLINE void Swap(T& a, T& b)
    {
        T t = a;
        a = b;
        b = t;
    }

    template<class T>
    CV_INLINE T Min(T a, T b)
    {
        return a > b ? b : a;
    }

    template<class T>
    CV_INLINE T Max(T a, T b)
    {
        return a > b ? a : b;
    }

    template<class T>
    CV_INLINE T Abs(T a)
    {
        return a > 0 ? a : -a;
    }

    template<class T>
    CV_INLINE T Square(T a)
    {
        return a * a;
    }

    template<typename T>
    CV_INLINE T RestrictRange(T a);
}