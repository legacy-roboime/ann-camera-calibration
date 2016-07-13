#include <SerialStream.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

#define CHECK_S(msg) \
  if (!serial_port.good()) { std::cerr << "Error: " << msg << std::endl; exit(1); }

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

// This example reads the contents of a file and writes the entire
// file to the serial port one character at a time.
int main(int argc, char **argv) {
  init_serial();

  char package[5];
  package[0] = 'a';
  package[4] = 0;
  int v[3];
 
  while (true) {
    std::cin >> v[1] >> v[2] >> v[3];
    //input_file.read(&next_byte, 1);
    for(int i = 0; i< 3; i++){
      package[i+1] = v[i];
    }
    for(int i = 0; i < 5; i++){
      serial_port.write(&package[i], 1);
      std::cerr << package[i] << " ";
    }
    std::cerr << "\n";
    // Print a '.' for every character read from the input file.
    usleep(100000);
  }

  return EXIT_SUCCESS;
}

#undef SET_AND_CHECK
#undef CHECK_S
