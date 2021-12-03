cmake_minimum_required(VERSION 3.0)

project(restclientcpp_test)

if(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(WIN32)


set (CMAKE_CXX_STANDARD 11)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${INCLUDE_DIR})

set(HEADER_FILES ${INCLUDE_DIR}/ReactionWheelDriver.hpp)

# 2 Ways to link to restclient-cpp once its available in /usr/local
# Method-1
# include_directories(/usr/local/include/restclient-cpp/)
# find_library(restclient-cpp /usr/local/lib/)
# Method-2
link_directories(/usr/local/lib)

find_library(restclient-cpp REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_DIR}/ReactionWheelTest.cpp ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} restclient-cpp)
