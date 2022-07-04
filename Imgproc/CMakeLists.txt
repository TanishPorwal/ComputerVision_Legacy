project(Imgproc)

add_library(${PROJECT_NAME} INTERFACE HarrisCorner.h Blur.h ArithmeticOperations.h Canny.h ColorConversion.h Convolution.h DrawShapes.h
        ImageOperations.h MathConstants.h Ransac.h)
add_library(cv::imgproc ALIAS ${PROJECT_NAME})
