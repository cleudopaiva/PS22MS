cmake_minimum_required(VERSION 3.20)

project(dualsense_interpreter LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(
  HIDAPI 
  REQUIRED
  IMPORTED_TARGET
  hidapi-hidraw
)

add_executable(
  dualsense_interpreter
  src/main.cpp
)

target_link_libraries(
  dualsense_interpreter
  PRIVATE
    PkgConfig::HIDAPI
)

