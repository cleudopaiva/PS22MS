cmake_minimum_required(VERSION 3.20)

project(dualsense_interpreter LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(hidapi CONFIG REQUIRED)

add_executable(
  dualsense_interpreter
  src/main.cpp
  adapter/adapter.cpp
  interpreter/interpreter.cpp
)

add_library(
  serial
  ../../common/serial/serial.cpp
)

target_include_directories(
  serial
  PUBLIC
    ../../common/serial
)

target_include_directories(
  dualsense_interpreter
  PRIVATE
    interpreter
    adapter
    ../../common/master_system_command/
)

target_link_libraries(
  dualsense_interpreter
  PRIVATE
      hidapi::hidapi
      serial
)

set_target_properties(
  dualsense_interpreter
  serial
  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

