# CANPiAuto

Welcome to CANPiAuto project.

The MERG-CANPiAuto is an application that is designed to run on a RaspberryPi attached to a CAN module transceiver.
The app interfaces the MERG-CANBUS protocol and the JMRI Engine Drive application. 
It allows the user to drive model rail locomotives using the Engine Drive without JMRI.
The app also works as a CAN to CANGrid formatter via a tcp connection.

## Building the project on PiOS

### Prerequisites

sudo apt install liblog4cpp-doc liblog4cpp5-dev liblog4cpp5v5

sudo apt install python-webpy-doc python3-webpy

Clone the project.
If developing in a Raspberry pi with copy the file /lib/liblog4cpp.a.pi to /lib/liblog4cpp.a

Type **make all**.

Use the script initial_setup.sh to configure the raspberry pi in AP mode and compile the project.
