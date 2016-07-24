cmake_minimum_required(VERSION 2.8.10)

project(roboime-camera-calibration)
include_directories("${PROJECT_SOURCE_DIR}")
add_executable(demo-joystick
    main.cpp
    joystick.cc
)
target_link_libraries(demo-joystick serial pthread)
