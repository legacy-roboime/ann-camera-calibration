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
  if(argc < 3) {
    printf(" Must provide at least two arg\n");
    exit(1);
  }

  init_serial();

  // Do not skip whitespace characters while reading from the
  // serial port.
  //
  // serial_port.unsetf( std::ios_base::skipws ) ;
  //
  char package[5];
  package[0] = 'a';
  int delay   = atoi(argv[1]),
      num_pkg = atoi(argv[2]), n = 0;
  int vel[3];
  int num = 0;
  while (n++ < num_pkg) {
    std::cin >> package[1] >> package[2] >> package[3];
    //input_file.read(&next_byte, 1);
    for(int i = 0; i < 5; i++){
      serial_port.write(&package[i], 1);
    }
    // Print a '.' for every character read from the input file.
    //std::cerr <<  << " " << num++ << std::endl;
    usleep(delay);
  }

  return EXIT_SUCCESS;
}

#undef SET_AND_CHECK
#undef CHECK_S
