#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char **argv) {
  // Open the serial port.
  using namespace LibSerial;
  SerialStream serial_port;
  char serial_port_file[] = "/dev/pts/6";
  serial_port.Open(serial_port_file);
  if (!serial_port.good()) {
    std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
              << "Error: Could not open serial port " << serial_port_file << "."
              << std::endl;
    exit(1);
  }

#define SET_AND_CHECK(func, value, msg)         \
  serial_port. func (SerialStreamBuf:: value ); \
  if (!serial_port.good()) { std::cerr << "Error: " << msg << std::endl; exit(1); }
  // Set the baud rate of the serial port
  // Set the number of data bits
  // Disable parity
  SET_AND_CHECK( SetBaudRate, BAUD_115200, "Could not set the baud rate."      );
  SET_AND_CHECK( SetCharSize, CHAR_SIZE_8, "Could not set the character size." );
  SET_AND_CHECK( SetParity  , PARITY_NONE, "Could not disable the parity."     );
  // Set the number of stop bits
  serial_port.SetNumOfStopBits(1);
  if (!serial_port.good()) {
    std::cerr << "Error: Could not set the number of stop bits." << std::endl;
    exit(1);
  }
  // Turn off hardware flow control.
  SET_AND_CHECK(SetFlowControl, FLOW_CONTROL_NONE,"Could not use hardware flow control." );
#undef SET_AND_CHECK
  //
  // Do not skip whitespace characters while reading from the
  // serial port.
  //
  // serial_port.unsetf( std::ios_base::skipws ) ;
  //
  // Wait for some data to be available at the serial port.
  while (serial_port.rdbuf()->in_avail() == 0) { usleep(100); }
 
  // Keep reading data from serial port and
  // print it to the screen.
  while (serial_port.rdbuf()->in_avail() > 0) {
    char next_byte;
    serial_port.get(next_byte);
    std::cerr << next_byte;
    usleep(100);
  }
  // std::cerr << std::endl ;
  return EXIT_SUCCESS;
}
