cmake_minimum_required(VERSION 3.20)

project(dualsense_interpreter LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(hidapi CONFIG REQUIRED)

add_executable(
  dualsense_interpreter
  src/main.cpp
)


target_link_libraries(
  dualsense_interpreter
  PRIVATE
      hidapi::hidapi
)

set_target_properties(
  dualsense_interpreter
  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

