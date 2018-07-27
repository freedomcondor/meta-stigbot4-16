cmake_minimum_required(VERSION 3.1)

project(cameraTest)

include(${CMAKE_SOURCE_DIR}/FindAprilTag.cmake)

add_executable(cameraTest
				src/main.cpp
				src/function.cpp
              )

target_link_libraries(cameraTest ${APRILTAG_LIBRARY})

set_property(TARGET cameraTest PROPERTY CXX_STANDARD 11)

install(TARGETS cameraTest DESTINATION bin)




