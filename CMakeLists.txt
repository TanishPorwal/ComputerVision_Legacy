cmake_minimum_required(VERSION 3.28)
project(ComputerVision)

add_executable(ComputerVision main.cpp
        main.cpp
        Imgproc/ColorConversion.h
        Imgproc/ColorConversion.cuh
        Imgproc/ColorConversion.cu
        Imgproc/Convolutions.h
        Imgproc/PixelTransforms.h
        Imgproc/Canny.h
        Imgproc/HarrisCornerDetector.h
        Imgproc/SIFT.h)

set(CMAKE_CXX_STANDARD 17)  # For C++ standard
set(CMAKE_CUDA_STANDARD 17)  # For CUDA standard
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CUDA_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CUDA_EXTENSIONS OFF)


set(OpenCV_DIR "E:/opencv/build")
set(CMAKE_PREFIX_PATH "E:/libtorch")  # Change this to your LibTorch path

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)

set_target_properties(ComputerVision PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(ComputerVision "${TORCH_LIBRARIES}" "${OpenCV_LIBS}")
target_include_directories(ComputerVision PRIVATE ${TORCH_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})


set_property(TARGET ComputerVision PROPERTY CUDA_STANDARD 17)
set_property(TARGET ComputerVision PROPERTY CUDA_STANDARD_REQUIRED ON)
set_property(TARGET ComputerVision PROPERTY CUDA_EXTENSIONS OFF)