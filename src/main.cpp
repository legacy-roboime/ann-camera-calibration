#include <iostream>
#include <vector>
#include <cmath>
#include "joystick.hh"
#include <unistd.h>
#include <SerialStream.h>
#include <fstream>
#include <cstdlib>

#define CHECK_S(msg) \
  if (!serial_port.good()) {                    \
    std::cerr << "Error: " << msg << std::endl; \
    exit(1);                                    \
  }

#define SET_AND_CHECK(func, value, msg)                    \
  serial_port. func (LibSerial::SerialStreamBuf:: value ); \
  CHECK_S(msg)

LibSerial::SerialStream serial_port;

void init_serial() {
  // Open the serial port.
  const char *const SERIAL_PORT_DEVICE = "/dev/ttyACM0";

  serial_port.Open(SERIAL_PORT_DEVICE);
  CHECK_S("Could not open serial port")
  //SET_AND_CHECK( SetBaudRate, BAUD_9600, "Could not set the baud rate."      );
  SET_AND_CHECK( SetCharSize, CHAR_SIZE_8, "Could not set the character size." );
  SET_AND_CHECK( SetParity  , PARITY_NONE, "Could not disable the parity."     );

  // Set the number of stop bits
  serial_port.SetNumOfStopBits(1);
  CHECK_S("Could not set the number of stop bits")
  // Turn on hardware flow control
  serial_port.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_NONE);
  CHECK_S("Could not use hardware flow control")
}



int main() {
  init_serial();

  char package[5];
  package[0] = 'a';
  package[4] = 0;
  int v[3];



  Joystick joystick("/dev/input/js0");
  if (!joystick.isFound()) {
    std::cerr << "no joystick found" << std::endl;
    return 1;
  }

  while (true) {
    int axis0_pos, axis1_pos, axis3_pos;
    float v_ang, v_tan, v_norm;
    JoystickEvent event;
    if (joystick.sample(&event)) {
      if ((event.isAxis()) && (event.number == 1)) {
        axis1_pos = event.value;
        if (axis1_pos > 4000 || axis1_pos < -4000) {
          v_tan = (-(float)axis1_pos / 1000.0);
        } else {
          v_tan = 0;
        }
        v[0]=v_tan;
        std::cout << "v_tan"<< v_tan << std::endl;
      }
      if ((event.isAxis()) && (event.number == 0)) {
        axis0_pos = event.value;
        if (axis0_pos > 4000 || axis0_pos < -4000) {
          v_norm = (-(float)axis0_pos / 1000.0);
        } else {
          v_norm = 0;
        }
        v[1]=v_norm;
        std::cout << "v_norm" <<v_norm << std::endl;
      }
      if ((event.isAxis()) && (event.number == 3)) {
        axis3_pos = event.value;
        if ((axis3_pos > 7000) || (axis3_pos < -1000)) {
          v_ang = (-(float)axis3_pos / 1000.0);
        } 
        else {
          v_ang = 0;
        }
        v[2]=v_ang;
        std::cout << "v_ang" << v_ang << std::endl;
      }
      for(int i = 0; i< 3; i++){
        package[i+1] = v[i];
      }
      for(int i = 0; i < 5; i++){
        serial_port.write(&package[i], 1);
        std::cerr << package[i] << " ";
      }
      sleep(0.01);
    }
  }
  return EXIT_SUCCESS;
}
