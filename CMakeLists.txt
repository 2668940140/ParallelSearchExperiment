cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenMP REQUIRED) # Find the OpenMP package

add_executable(main main.cpp
                    utils.cpp
                    dense_parallel.cpp)

if(OpenMP_CXX_FOUND)
    target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX) # Link OpenMP to your target
endif()