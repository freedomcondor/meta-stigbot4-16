cmake_minimum_required(VERSION 3.1)

project(speedTest)

#find_package(OpenGL REQUIRED)
#${OPENGL_INCLUDE_DIRS}
#${OPENGL_LIBRARIES}
#find_package(GLUT REQUIRED)
#${GLUT_INCLUDE_DIRS}
#${GLUT_LIBRARIES}


add_executable(speedTest
#main.cc
				src/main.cpp
		src/function_opengl.cpp
               	src/packet_control_interface.cc
               	src/uart_socket.cc
              )

set_property(TARGET speedTest PROPERTY CXX_STANDARD 11)

install(TARGETS speedTest DESTINATION bin)




