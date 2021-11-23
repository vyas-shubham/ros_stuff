cmake_minimum_required(VERSION 3.5)
project(reaction_wheel)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include Directories
include_directories(include/reaction_wheel)

# For restclient-cpp library
link_directories(/usr/local/lib)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

find_library(restclient-cpp REQUIRED)

add_executable(reactionwheel_node src/reactionwheelnode.cpp)

ament_target_dependencies(reactionwheel_node rclcpp)
target_link_libraries(reactionwheel_node restclient-cpp)

target_include_directories(reactionwheel_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>) 

ament_export_dependencies(restclient-cpp)

install(TARGETS reactionwheel_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
