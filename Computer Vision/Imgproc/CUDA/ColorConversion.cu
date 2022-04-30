// #include <cuda.h>
// #include <device_launch_parameters.h>
// #include <cuda_runtime_api.h>
//
// #include <iostream>
//
// __global__ void grayscale(uint8_t* d_src, uint8_t* d_dst, int rows, int cols, int channels)
// {
//     size_t i = threadIdx.x + blockIdx.x * blockDim.x;
//     size_t j = threadIdx.y + blockIdx.y * blockDim.y;
//
//     if(i >= rows || j >= cols)
//         return;
//
//     if(channels == 3)
//     {
//         // if()
//         uchar3 img = {d_src[i*cols + j], d_src[i*cols + j + 1], d_src[i*cols + j + 2]};
//     }
// }