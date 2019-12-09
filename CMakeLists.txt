cmake_minimum_required(VERSION 3.14)
project(main)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp)
set(INCLUDING_LIBS dependencies/lib)
set (CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG}")
find_package(OpenGL REQUIRED)

include_directories(dependencies/include)
find_package(glfw3 REQUIRED)
add_executable(main main.cpp)

target_include_directories(main PRIVATE ${OPENGL_INCLUDE_DIR})

add_library(glew STATIC IMPORTED)
set_property(TARGET glew PROPERTY
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/dependencies/lib/libGLEW.a)
target_link_libraries(main LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw glew)