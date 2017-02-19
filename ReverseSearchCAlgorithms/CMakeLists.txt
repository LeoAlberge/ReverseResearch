set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
cmake_minimum_required(VERSION 2.6)
file(TO_CMAKE_PATH "$ENV{IMAGINEPP_ROOT}" d)
if(NOT EXISTS "${d}")
    message(FATAL_ERROR "Error: environment variable IMAGINEPP_ROOT=" "${d}")
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${d}/CMake")
find_package(Imagine)

project(MOPSI_Algo)
add_executable(MOPSI_Algo
	Imagine/SIFT_VL.cpp

         descriptor.cpp global.h tools.h
	Imagine/vl/generic.c Imagine/vl/host.c Imagine/vl/imop.c Imagine/vl/sift.c Imagine/vl/mser.c
main.cpp 
         )

ImagineUseModules(MOPSI_Algo Graphics LinAlg Images)
