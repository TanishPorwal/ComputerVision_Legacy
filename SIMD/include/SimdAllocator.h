/*
* ComputerVision Library (https://github.com/BubblyBunny2/ComputerVision).
*
* Copyright (c) 2022 Tanish.
*/

#pragma once

#include <cstddef>

#include "SimdDefs.h"
#include "SimdMemory.h"
#include "SimdAlignment.h"

namespace Simd {
    template<class T>
    struct SimdAllocator {
        static CV_INLINE void* Allocate(size_t size, size_t align)
        {
                return Simd::Allocate(size, align);
        }

        static CV_INLINE void Free(void* ptr)
        {
            Simd::Free(ptr);
        }

        static CV_INLINE size_t Align(size_t size, size_t align)
        {
            return Simd::AlignHi(size, align);
        }

        /*!
            \fn void * Align(void * ptr, size_t align);

            \short Gets aligned address.

            \param [in] ptr - an original pointer.
            \param [in] align - a required alignment.

            \return an aligned address.
        */
        static CV_INLINE void * Align(void * ptr, size_t align)
        {
            return Simd::AlignHi(ptr, align);
        }

        /*!
            \fn size_t Alignment();

            \short Gets memory alignment required for the most productive work.

            \return a required memory alignment.
        */
        static CV_INLINE size_t Alignment()
        {
            return Simd::Alignment();
        }

        //---------------------------------------------------------------------
        // STL allocator interface implementation:

        typedef T value_type;
        typedef T * pointer;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T & reference;
        typedef const T & const_reference;
        typedef const T * const_pointer;

        template <typename U>
        struct rebind
        {
            typedef SimdAllocator<U> other;
        };

        CV_INLINE SimdAllocator()
        {
        }

        template <typename U> CV_INLINE SimdAllocator(const SimdAllocator<U> & a)
        {
        }

        CV_INLINE const_pointer address(const_reference value) const
        {
#if defined(CV_SIMD_CPP_2011_ENABLE)
            return std::addressof(value);
#else
            return (reinterpret_cast<const_pointer>(&const_cast<char&>(reinterpret_cast<const volatile char&>(value))));
#endif
        }

        CV_INLINE pointer address(reference value) const
        {
#if defined(CV_SIMD_CPP_2011_ENABLE)
            return std::addressof(value);
#else
            return (reinterpret_cast<pointer>(&const_cast<char&>(reinterpret_cast<const volatile char&>(value))));
#endif
        }

        CV_INLINE pointer allocate(size_type size, const void * ptr = nullptr)
        {
            return static_cast<pointer>(Allocate(size * sizeof(T), Alignment()));
        }

        CV_INLINE size_type max_size() const
        {
            return ~static_cast<std::size_t>(0) / sizeof(T);
        }

        CV_INLINE void deallocate(pointer ptr, size_type size)
        {
            Free(ptr);
        }

        template<class U, class V> CV_INLINE void construct(U * ptr, const V & value)
        {
            ::new((void*)ptr) U(value);
        }

#if defined(CV_SIMD_CPP_2011_ENABLE)
        template<class U, class... Args> CV_INLINE void construct(U * ptr, Args &&... args)
        {
            ::new((void*)ptr) U(std::forward<Args>(args)...);
        }
#endif

        template<class U> CV_INLINE void construct(U * ptr)
        {
            ::new((void*)ptr) U();
        }

        template<class U> CV_INLINE void destroy(U * ptr)
        {
            ptr->~U();
        }
    };

    template<typename T1, typename T2> CV_INLINE bool operator == (const SimdAllocator<T1> & a1, const SimdAllocator<T2> & a2)
    {
        return true;
    }

    template<typename T1, typename T2> CV_INLINE bool operator != (const SimdAllocator<T1> & a1, const SimdAllocator<T2> & a2)
    {
        return false;
    }
}  // end Simd namespace
