cmake_minimum_required(VERSION 3.27)
project(CSCI_561_HW2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
add_executable(CSCI_561_HW2
        utils.cpp
        )
add_executable(new_target calibrate.cpp)
