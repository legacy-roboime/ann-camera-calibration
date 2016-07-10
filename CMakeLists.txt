cmake_minimum_required(VERSION 2.8.10)

project(roboime-camera-calibration)

set(roboime-camera-calibration_VERSION_MAJOR_0)
set(roboime-camera-calibration_VERSION_MINOR_1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#
# DEP
#

include_directories("${PROJECT_SOURCE_DIR}/src")

#
# BIN
#

# serial r/w data
add_executable(read_port src/read_port.cpp) 
target_link_libraries(read_port serial pthread)

add_executable(write_port src/write_port.cpp) 
target_link_libraries(write_port serial pthread)

# calibration test

add_executable(position_capture_train src/position_capture_train.c)
target_link_libraries(position_capture_train "-lfann -lm")

add_executable(position_capture_test src/position_capture_test.c)
target_link_libraries(position_capture_test "-lfann -lm")

# vim: et sw=2 ts=2 sts=2
