# Calibration

The main objective of this project is to create
a way to calibrate the control system.

## Roadmap

  - Read data from sensors using USB-VCP get string
  - Save in a database
  - Display in a graph: (maybe using _jpgraph.net_)

    - Four wheel speeds;
    - Resultant speed;
    - Waste speed (omni equation);
    - Force direction;
    - Energy consumption;
    - Battery voltage;
    - Time till battery dies;

  - Get data from RF
  - Use time Multiplexation to pass all data
  - Treat Data (filters)
  - **Calibrate Controler params (probably PID) using data**

## State of the hardware/firmware

### Package Description

Currently the package to control the robot is as follows:


     +---+-----+-----+---+----------------------------+
     |'a'| v_n | v_t | w | reserved for the future... |
     +---+-----+-----+---+----------------------------+

where:

 - v_n: normal speed
 - v_t: tangential speed
 - w: angular speed

### Bizare transmission problem

There is a problem in transmission device that makes it
stop after a fixed number of packages. This is probably
a problem in the USB driver/linux stuff... who knows...

### Usage

To test a set of commands to be sent to the robot, just
do as follows:


` ` ` `

` ` ` `
