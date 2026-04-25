cmake_minimum_required(VERSION 3.10)
project(Lab6_OpenCV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Знаходимо OpenCV
find_package(OpenCV REQUIRED)

# Включаємо header-и
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Збираємо всі .cpp файли з src/
file(GLOB SOURCES "src/*.cpp")

# Створюємо виконуваний файл
add_executable(lab6 ${SOURCES})

# Лінкуємо OpenCV
target_link_libraries(lab6 ${OpenCV_LIBS})
