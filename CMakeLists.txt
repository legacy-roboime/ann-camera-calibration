cmake_minimum_required(VERSION 2.8.10)

project(roboime-camera-calibration)
#include_directories("${PROJECT_SOURCE_DIR}")
include_directories("src")

#
# DEPS
#

#
# BIN
#
set(COMMON_LIBRARIES
  serial
  pthread
  )

# Joystic test
#add_executable(demo-joystick src/main.cpp src/joystick.cc)
#target_link_libraries(demo-joystick ${COMMON_LIBRARIES})

# My Joystic test
add_executable(dualshock3-ctl src/dualshock3_ctl.cpp src/joystick.cc)
target_link_libraries(dualshock3-ctl ${COMMON_LIBRARIES})

# Transmission test
add_executable(raw-tx src/write_port.cpp)
target_link_libraries(raw-tx serial)
