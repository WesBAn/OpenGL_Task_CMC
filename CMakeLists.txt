cmake_minimum_required(VERSION 3.14)
project(main)

set(CMAKE_CXX_STANDARD 14)
#set(SOURCE_FILES main.cpp)
set(INCLUDING_LIBS dependencies/lib)
set (CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG}")
find_package(OpenGL REQUIRED)

include_directories(dependencies/include)
find_package(glfw3 REQUIRED)
add_executable(main main.cpp mc_shaders.cpp dependencies/include/objectPositions.h camera.cpp materials.cpp)

target_include_directories(main PRIVATE ${OPENGL_INCLUDE_DIR})

add_library(glew STATIC IMPORTED)
set_property(TARGET glew PROPERTY
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/dependencies/lib/libGLEW.a)

add_library(soil2 SHARED IMPORTED)
set_property(TARGET soil2 PROPERTY
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/dependencies/lib/libSOIL2.so)

target_link_libraries(main LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw glew soil2)

#find_library(GLEW_LIB glew ${CMAKE_SOURCE_DIR}/${INCLUDING_LIBS}/libGLEW.a)
#find_library(SOIL2 SOIL2 HINTS ${CMAKE_SOURCE_DIR}/dependencies/lib/libSOIL2.a)
#target_link_libraries(main LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw ${GLEW_LIB} ${SOIL2})

#link_directories(${CMAKE_SOURCE_DIR}/dependencies/lib)
#target_link_libraries(main LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw glew)