// #include <cuda.h>
// #include <device_launch_parameters.h>
// #include <cuda_runtime_api.h>
// #include <cudnn.h>
// #include <cudnn_ops_infer.h>
//
// #include <iostream>
//
// #define checkCUDNN(expression)                               \
//   {                                                          \
//     cudnnStatus_t status = (expression);                     \
//     if (status != CUDNN_STATUS_SUCCESS) {                    \
//       std::cerr << "Error on line " << __LINE__ << ": "      \
//                 << cudnnGetErrorString(status) << std::endl; \
//       std::exit(EXIT_FAILURE);                               \
//     }                                                        \
//   }
//
//
// namespace cv
// {
//     __global__ void edge_detect_cuda(uint8_t& d_data, int rows, int cols)
//     {
//       cudnnHandle_t cudnnHandle;
//       checkCUDNN(cudnnCreate(&cudnnHandle));
//       
//       cudnnTensorDescriptor_t input_descriptor;
//       checkCUDNN(cudnnCreateTensorDescriptor(&input_descriptor));
//       checkCUDNN(cudnnSetTensor4dDescriptor(input_descriptor, CUDNN_TENSOR_NHWC, CUDNN_DATA_FLOAT, 1, 3, rows, cols));
//
//       cudnnTensorDescriptor_t output_descriptor;
//       checkCUDNN(cudnnCreateTensorDescriptor(&output_descriptor));
//       checkCUDNN(cudnnSetTensor4dDescriptor(output_descriptor, CUDNN_TENSOR_NHWC, CUDNN_DATA_FLOAT, 1, 3, rows, cols));
//
//       cudnnFilterDescriptor_t kernel_descriptor;
//       checkCUDNN(cudnnCreateFilterDescriptor(&kernel_descriptor));
//       checkCUDNN(cudnnSetFilter4dDescriptor(kernel_descriptor, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, 3, 3, 3, 3));
//
//       
//     }
// }
