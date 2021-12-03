cmake_minimum_required(VERSION 3.0)

project(ReactionWheelDriver)

if(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(WIN32)


set (CMAKE_CXX_STANDARD 17)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
set(lib_INCLUDE_DIR ${INCLUDE_DIR} CACHE INTERNAL "")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${INCLUDE_DIR})


include_directories(
	            ${PROJECT_SOURCE_DIR}/include            
	            ${PROJECT_SOURCE_DIR}/src            
	            )

set(LIBRARY_NAME ${PROJECT_NAME} CACHE INTERNAL "")

set(LIBRARY_SOURCE ${SOURCE_DIR}/ReactionWheelDriver.cpp)

set(LIBRARY_HEADER ${INCLUDE_DIR}/ReactionWheelDriver.hpp)

# For restclient-cpp library
link_directories(/usr/local/lib)
find_library(restclient-cpp REQUIRED)

# Create an Object to Make both Static and Shared Libraries in the same CMakeLists File.
add_library(${PROJECT_NAME}-object OBJECT ${LIBRARY_SOURCE} ${LIBRARY_HEADER})

# this is only needed for older compilers but doesn't hurt either to have it
set_target_properties(${PROJECT_NAME}-object PROPERTIES POSITION_INDEPENDENT_CODE 1)

add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-object>)

add_library(${PROJECT_NAME}-static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}-object>)

target_link_libraries(${PROJECT_NAME} restclient-cpp)
target_link_libraries(${PROJECT_NAME}-static restclient-cpp)

# Add execuatble for a simple on/off/zero position driver test
add_executable(ReactionWheelDriverTest ${SOURCE_DIR}/ReactionWheelDriverTest.cpp)
target_link_libraries(ReactionWheelDriverTest ${PROJECT_NAME}-static ${CMAKE_THREAD_LIBS_INIT})

INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
       )

# INSTALL(FILES ${LIBRARY_HEADER}
#         DESTINATION include/
#         )

# set(PKGCONFIG_REQUIRES_PRIVATE "")
# set(PKGCONFIG_EXTRA_LINK_FLAGS "")
# set(PKGCONFIG_LINK_FLAGS_PRIVATE "")

# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc.in
#     ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY)

# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION lib/pkgconfig)
