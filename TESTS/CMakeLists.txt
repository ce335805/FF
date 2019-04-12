#specefiy required version - this is the newest one
cmake_minimum_required(VERSION 3.11)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
#using icc compiler
set (CMAKE_CXX_COMPILER "mpiicpc")
set (CMAKE_C_COMPILER "mpiicc")


#set as inlcude dir the current directory
#one could devide the project into more directorys later
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(project_sources
		run_all_tests.cpp
		greentest.cpp
		test_FF.cpp
                test_Index.cpp
                test_IndexIBZ.cpp
                ../GreenFunction.cpp
                ../Index.cpp
                ../FF.cpp
        )

#include gtest
include_directories("/home/chris/gtest/googletest/googletest/include")
#include from parent directory to have '.h' files
include_directories("../")
#include MKL
include_directories("/opt/intel/mkl/include")


#add compiler flags inlcuding cblas
set(C_flags "-O3 -Wall -mkl -Wextra -pedantic")
set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} ${C_flags}")

#set project name
project (test_victory)
#add an executable - using variables
add_executable(${PROJECT_NAME} ${project_sources})

#link to gtest
target_link_libraries(${PROJECT_NAME} "/home/chris/gtest/build/lib/libgtest.a")
target_link_libraries(${PROJECT_NAME} "/home/chris/gtest/build/lib/libgtest_main.a")
#link to pthreads
target_link_libraries(${PROJECT_NAME} "pthread")
#place where libfabric is installed - needed for IntelMPI-2019
target_link_libraries(${PROJECT_NAME} "/usr/local/lib/libfabric.so.1")
