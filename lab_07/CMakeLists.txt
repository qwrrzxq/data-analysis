cmake_minimum_required(VERSION 3.10)
project(Lab7_OpenCV_FaceDetection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(lab7 ${SOURCES})

target_link_libraries(lab7 ${OpenCV_LIBS} Threads::Threads)
