# Hyperloop Firmware [![Build Status](https://travis-ci.org/ParadigmHyperloop/hyperloop-firmware.svg?branch=master)](https://travis-ci.org/ParadigmHyperloop/hyperloop-firmware)

This controller implements a generic controller for the Paradigm node boards 
which act in a federation directed by the flight computer. The module receives
control requests and sends telemetry over UDP.

# Getting Started

First let's setup the [Platformio IDE](http://platformio.org/), which is a set 
of extensions for embedded development in Atom.

1. Download & Install Atom from [atom.io](https://atom.io)
2. Open the package manager (CMD + SHIFT + P) and type `platformio` in the search box
3. Install all 4 official plaformio packages and follow any additional instructions (like installing LLVM & clang)

Now that you have atom and platformio, you can clone this repo and then open the repo in atom

1. Clone this repo however you desire
2. Open the folder in atom (File > Open Folder) or use the `atom` shell command
3. The platformio sidebar should show up, click the `Verify` button (check box icon, second one down)
4. Ensure that the project builds successfully.

Now for the fun part, grab an arduino nano (Also supports M0's you will need to uncomment some stuff [here](platformio.ini) though)

1. Plug in the arduino
2. Click the Upload button to build and upload to the arduino
3. Click the SerialUSB Console button (looks like a wall plug), it should autodetect the port, click start
4. The SerialUSB console will start spewing `debug()` output

```
[DEBUG] [loop in src\main.cpp:268] --- Starting Network
[DEBUG] [net_recv_request in src\network.cpp:19] Requesting Packet
[DEBUG] [net_recv_request in src\network.cpp:21] Packet Back
[DEBUG] [loop in src\main.cpp:272] --- Read SerialUSB
[DEBUG] [loop in src\main.cpp:275] --- Starting Sensors
[DEBUG] [loop in src\main.cpp:279] --- Starting States
[DEBUG] [loop in src\main.cpp:283] --- Starting Actuators
```

# Dev Notes

This project uses the Arduino Bootloader for now and is written in Arduino 
C++ with a focus on object oriented programming to enable modularity.

# License

See the [LICENSE](LICENSE) for full licensing details.

In summary, (you still need to read the whole thing), these boards are for Paradigm,
and Paradigm only. It is shared with the world for the benefit of observers and
potential Paradigm team members. If you wish to utilize these designs in any
way, you must contact us first and receive written permission to utilize the
designs for the purpose you require.

We don't want to be restrictive, and we will almost certainly grant you permission,
but we want to know what you are using our designs for, and we want to make sure
you are using them correctly and safely.
