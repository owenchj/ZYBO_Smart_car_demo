cmake_minimum_required(VERSION 2.8)

project( DisplayImage )

find_package( OpenCV REQUIRED )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
SET(GCC_COVERAGE_LINK_FLAGS    "-std=c++11")

add_executable( DisplayImage send_pic.c send_rgb_tcp.c DisplayImage.cpp )

target_link_libraries( DisplayImage ${OpenCV_LIBS} )
