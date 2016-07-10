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
