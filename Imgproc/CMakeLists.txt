project(cv_imgproc)

add_library(${PROJECT_NAME} INTERFACE HarrisCorner.h)
add_library(cv::imgproc ALIAS ${PROJECT_NAME})
