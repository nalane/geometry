cmake_minimum_required(VERSION 3.7)
project(geometry)

include_directories(inc)

find_package(CGAL)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(geometry ${SOURCES})

if(NOT WIN32)
  set(GCC_COMPILE_FLAGS "-g -std=c++11")
  add_definitions(${GCC_COMPILE_FLAGS})
endif()

target_link_libraries(geometry CGAL::CGAL)
