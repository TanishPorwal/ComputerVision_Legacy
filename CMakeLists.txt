# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("CV")

# Include sub-projects.
add_subdirectory ("Core")
add_subdirectory("Imgproc")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} cv::core cv::imgproc)
if (UNIX)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/3rdParty/GLFW/lib/libgl3w_d.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/3rdParty/GLFW/lib/libglfw3_d.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/3rdParty/imgui/lib/libimgui_d.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
endif (UNIX)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Core/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

if(NOT MSVC)
  set(PThreadLib -pthread)
  target_link_libraries(${PROJECT_NAME} ${PThreadLib})
endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()