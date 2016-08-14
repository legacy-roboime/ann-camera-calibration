#include <SerialStream.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include <getopt.h>
#include <strings.h>
#include <limits.h>

#define CHECK_S(msg) \
  if (!serial_port.good()) { std::cerr << "Error: " << msg << std::endl; exit(1); }

#define SET_AND_CHECK(func, value, msg)                    \
  serial_port. func (LibSerial::SerialStreamBuf:: value ); \
  CHECK_S(msg)

#define ROBOT_STOP send_data(   0,  0,  0, n); printf(" stop ");
#define ROBOT_TURN send_data(   0,  0, 25, n); printf(" turn ");
#define ROBOT_GO   send_data( 127,  0,  0, n); printf(" Go Forest!! ");

LibSerial::SerialStream serial_port;
int delay=1000000, // one second
    num_pkg, n;
const char *const SERIAL_PORT_DEVICE = "/dev/ttyACM0";

void init_serial() {
  // Open the serial port.

  serial_port.Open(SERIAL_PORT_DEVICE);
  CHECK_S("Could not open serial port")
  SET_AND_CHECK( SetBaudRate, BAUD_9600,   "Could not set the baud rate."      );
  SET_AND_CHECK( SetCharSize, CHAR_SIZE_8, "Could not set the character size." );
  SET_AND_CHECK( SetParity  , PARITY_NONE, "Could not disable the parity."     );

  // Set the number of stop bits
  serial_port.SetNumOfStopBits(1);
  CHECK_S("Could not set the number of stop bits")
  // Turn on hardware flow control
  serial_port.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_SOFT);
  CHECK_S("Could not use hardware flow control")
}

void send_data(char v_n, char v_t, char w, int n_pkg) {
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

  printf(" %i: %s\n", n_pkg, package);
}

#define PRINT_USAGE \
          printf(" Usage: %s <delay> <num_pkg>\n", argv[0]); \
          exit( EXIT_FAILURE);

// XXX: Fix this in the future..
#define PKG_BUG_FIX        \
    if (n % 700 == 0) {    \
      serial_port.Close(); \
      init_serial();       \
    }

void go_turn(void) {
  n = 0;
  ROBOT_STOP

  while ( n++ < num_pkg ) {
    if ( n % 10 == 0 ) {
      ROBOT_STOP
      ROBOT_TURN
    }
    else { ROBOT_GO }

    PKG_BUG_FIX
  }

  ROBOT_STOP
}

// This example reads the contents of a file and writes the entire
// file to the serial port one character at a time.
int main(int argc, char **argv) {
  /*  decode asgvs, made easy with the getopt function
   *  one argugent is decoded in each step
   */
  if ( argc < 2 ) {
    PRINT_USAGE
    exit( EXIT_FAILURE );
  }

  init_serial();

  switch ( getopt( argc, argv, "p:g:i:t" )  ) {
    case 'p': ROBOT_STOP break;
    case 'g': ROBOT_GO   break;
    case 't':
      if (argc < 4) { printf(" need 2 args\n"); exit( EXIT_FAILURE ); }
      delay   = atoi(argv[2]);
      num_pkg = atoi(argv[3]);
      go_turn();

      break;

    case 'i':
      if (argc < 3) { printf(" need 1 arg\n"); exit( EXIT_FAILURE ); }

      delay   = atoi(argv[2]);

      while (1) {
        send_data( n++ % 100,  0,  0, n);
        PKG_BUG_FIX
      }

    default:
      PRINT_USAGE
      exit( EXIT_FAILURE );
  }

  return EXIT_SUCCESS;
}

#undef ROBOT_STOP
#undef ROBOT_TURN
#undef ROBOT_GO
#undef SET_AND_CHECK
#undef CHECK_S
