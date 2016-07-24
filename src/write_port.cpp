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
int delay=1000000; // one second

void init_serial() {
  // Open the serial port.
  const char *const SERIAL_PORT_DEVICE = "/dev/ttyACM0";

  serial_port.Open(SERIAL_PORT_DEVICE);
  CHECK_S("Could not open serial port")
  SET_AND_CHECK( SetBaudRate, BAUD_9600, "Could not set the baud rate."      );
  SET_AND_CHECK( SetCharSize, CHAR_SIZE_8, "Could not set the character size." );
  SET_AND_CHECK( SetParity  , PARITY_NONE, "Could not disable the parity."     );

  // Set the number of stop bits
  serial_port.SetNumOfStopBits(1);
  CHECK_S("Could not set the number of stop bits")
  // Turn on hardware flow control
  serial_port.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_NONE);
  CHECK_S("Could not use hardware flow control")
}

void send_data(char v_n, char v_t, char w) {
  char package[5];
  package[0] = 'a';
  package[1] = v_n;
  package[2] = v_t;
  package[3] = w;
  package[4] = 0;

  serial_port.write(&package[0], 1);

  usleep(delay);
  serial_port.write(&package[1], 1);
  serial_port.write(&package[2], 1);
  serial_port.write(&package[3], 1);
  serial_port.write(&package[4], 1);

  std::cerr << package << std::endl;
}

// This example reads the contents of a file and writes the entire
// file to the serial port one character at a time.
int main(int argc, char **argv) {
  if(argc < 2) {
    printf(" Usage: %s <delay>\n", argv[0]);
    exit(1);
  }

  init_serial();

  // Do not skip whitespace characters while reading from the
  // serial port.
  //
  // serial_port.unsetf( std::ios_base::skipws ) ;
  //
  //

  delay   = atoi(argv[1]);
  //    num_pkg = atoi(argv[2]), n = 0;

  send_data(  0,  0,  0);
  send_data(127,  0,  0);
  send_data(  0,  0,  0);
  send_data(  0, 127, 0);
  send_data(  0,  0,  0);
  send_data(  0,  0, 127);
  send_data(  0,  0,  0);

  return EXIT_SUCCESS;
}

#undef SET_AND_CHECK
#undef CHECK_S
