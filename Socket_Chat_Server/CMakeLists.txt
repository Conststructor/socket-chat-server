
cmake_minimum_required(VERSION 3.27.1)

project(socket_chat_server)
#${PROJECT_NAME}

#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDART_17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

file(GLOB CPPS "*.cpp")

add_executable(${PROJECT_NAME} ${CPPS})