project(Core)

add_library(${PROJECT_NAME} Image.cpp Window.cpp include/Image.h include/Window.h)
add_library(cv::core ALIAS ${PROJECT_NAME})

if (UNIX)
find_package(X11 REQUIRED)
message(STATUS "X11_FOUND = ${X11_FOUND}")
target_include_directories(${PROJECT_NAME} PUBLIC ${X11_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
endif (UNIX)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/3rdParty)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/SIMD/include)
