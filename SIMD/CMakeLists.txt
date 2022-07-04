project(SIMD)

add_library(${PROJECT_NAME} SimdAllocator.cpp include/SimdDefs.h include/SimdCPU.h SimdCPUBase.cpp include/SimdMemory.h include/SimdMath.h include/SimdAlignment.h include/SimdAVX2Enable.h include/SimdAVXEnable.h include/SimdSSE2Enable.h include/SimdSSE41Enable.h)
add_library(cv::simd ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

