#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cmath>
#include <unistd.h>
#include "joystick.hh"

#include <SerialStream.h>
#include <fstream>

//#define DEBUG

//
// Buttons of the DUALSHOCK 3
//
#define TRI_BUTTON 16
#define O_BUTTON   17
#define X_BUTTON   18
#define SQR_BUTTON 19

#define X_LEFT_AXIS 0
#define Y_LEFT_AXIS 1
#define X_RIGHT_AXIS 2
#define Y_RIGHT_AXIS 3

#define D3_MAX_VALUE 32767
#define D3_MIN_VALUE -32767

#define CHECK_S(msg)         \
  if (!serial_port.good()) { \
    fprintf( stderr, "Error in serial: %s\n", msg); \
    exit(EXIT_FAILURE);                             \
  }

#define SET_AND_CHECK(func, value, msg)                  \
  serial_port.func(LibSerial::SerialStreamBuf:: value ); \
  CHECK_S(msg)

#define TRANSMISSION_DELAY 1000000 // one second
LibSerial::SerialStream serial_port;

void init_serial() {
  // Open the serial port.
  const char *const SERIAL_PORT_DEVICE = "/dev/ttyACM0";

  serial_port.Open(SERIAL_PORT_DEVICE);
  CHECK_S("Could not open serial port")
  SET_AND_CHECK( SetBaudRate, BAUD_9600,   "Could not set the baud rate."      );
  SET_AND_CHECK( SetCharSize, CHAR_SIZE_8, "Could not set the character size." );
  SET_AND_CHECK( SetParity  , PARITY_NONE, "Could not disable the parity."     );

  // Set the number of stop bits
  serial_port.SetNumOfStopBits(1);
  CHECK_S("Could not set the number of stop bits")
  // Turn on hardware flow control
  serial_port.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_NONE);
  CHECK_S("Could not use hardware flow control")
}

#undef SET_AND_CHECK
#undef CHECK_S

inline bool check_event(int ev) {
  return ( ev == TRI_BUTTON   ||
           ev == O_BUTTON     ||
           ev == X_BUTTON     ||
           ev == SQR_BUTTON   ||
           ev == X_LEFT_AXIS  ||
           ev == Y_LEFT_AXIS  ||
           ev == X_RIGHT_AXIS ||
           ev == Y_RIGHT_AXIS );
      
}

struct Robot{
  int id, time_stamp;
  float v_ang, v_tan, v_norm;
  bool kick_x, kick_z, spin;
};

inline void get_joy_values(Joystick &joystick, Robot &robot) {
  int x_axis, y_axis, theta_x, theta_y;

  JoystickEvent event;

#define CHECK_BUTTON_EVENT(ev, var) \
  if (event.number == ev) { var = event.value >  D3_MIN_VALUE ? true: false; }
#define CHECK_AXIS_EVENT(ev, var) \
  if ((event.isAxis()) && (event.number == ev)) { var = event.value; }
#define NORMALIZE(dest, var) \
  if (var <= D3_MAX_VALUE) dest = ((float)var)/ D3_MAX_VALUE;
#define SQR(v) (v*v)

  if (joystick.sample(&event) && check_event(event.number)) {
#ifdef DEBUG
    printf("event number: %d value: %i\n", event.number, event.value);
#endif
    CHECK_BUTTON_EVENT( X_BUTTON,   robot.kick_x )
    CHECK_BUTTON_EVENT( TRI_BUTTON, robot.kick_z )
    CHECK_BUTTON_EVENT( SQR_BUTTON, robot.spin   )

    CHECK_AXIS_EVENT( X_LEFT_AXIS, x_axis )
    CHECK_AXIS_EVENT( Y_LEFT_AXIS, y_axis )

    CHECK_AXIS_EVENT( X_RIGHT_AXIS, theta_x )
    CHECK_AXIS_EVENT( Y_RIGHT_AXIS, theta_y )

    //robot.v_ang  = 180 * atan2(theta_y, theta_x) / M_PI;
    NORMALIZE( robot.v_ang  , theta_y )
    NORMALIZE( robot.v_tan  , x_axis  )
    NORMALIZE( robot.v_norm , y_axis  )
  }

#undef SQR
#undef CHECK_BUTTON_EVENT
#undef CHECK_AXIS_EVENT
#undef NORMALIZE
}

void send_commands(Robot&);

int main() {
  // Robot
  Robot robot;

  // Joystick handler
  Joystick joystick("/dev/input/js0");
  // Ensure that it was found and that we can use it
  if (!joystick.isFound()) {
    fprintf(stderr, "No joystick found!!\n");
    return EXIT_FAILURE;
  }

  // transmission
  init_serial();

  // main loop
  while (true) {
    get_joy_values ( joystick, robot );
    send_commands  ( robot );
  }

    return EXIT_SUCCESS;
}

void send_commands( Robot& robot ) {
  char package[5];

  package[0] = 'a';
  package[1] = robot.v_norm;
  package[2] = robot.v_tan;
  package[3] = robot.v_ang;
  package[4] = 0;

  serial_port.write(&package[0], 1);

  usleep(TRANSMISSION_DELAY);
  serial_port.write(&package[1], 1);
  serial_port.write(&package[2], 1);
  serial_port.write(&package[3], 1);
  serial_port.write(&package[4], 1);

#ifndef DEBUG
    printf( "robot: %f, %f, %f, %i, %i, %i\n",
        robot.v_norm,  robot.v_tan, robot.v_ang,
        robot.kick_x, robot.kick_z, robot.spin );
    printf( "package: %c %c %c %c %c\n",
        package[0], package[1], package[2], package[3], package[4] );
#endif
}
