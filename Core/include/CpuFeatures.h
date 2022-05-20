#pragma once

#include <cpu_features/cpuinfo_x86.h>

static constexpr cpu_features::X86Features features = cpu_features::GetX86Info().features;
static constexpr bool hasSSE    = features.sse;
static constexpr bool hasSSE2   = features.sse2;
static constexpr bool hasSSE3   = features.sse3;
static constexpr bool hasSSE4_1 = features.sse4_1;
static constexpr bool hasSSE4_2 = features.sse4_2;
static constexpr bool hasSSE4A  = features.sse4a;
static constexpr bool hasAVX    = features.avx;
static constexpr bool hasAVX2   = features.avx2;
