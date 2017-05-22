# ServoIn

Class to read servo signals from a typical RC system.

## Usage

This class is very similar to the Arduino Servo library for reading servo
signals. It uses interrupts on all pins attached to a signal, so make sure
all pins you use are using support interrupts and are independent from
each other.

See the example in examples/usage/usage.ino for an example of how to use
this library with 6 channels simultaneously.

## Wiring with a Particle Photon

### Power
Most RC servo system operate on 5V, so if you are powering the receiver directly
from your Photon, connect it to the VIN pin while on USB power. Or connect
5V from some other regulator in your system to the VIN pin and DO NOT connect
the USB port.

### Receiver Outputs
All that is needed to get the signal from the receiver is to plug the receive
pins from the channels you want directly into any 5 volt tolerant pin on the
Photon. See the documentation for which ones are 5V tolerant. Also, see the
documentation on which pins support interrupts. D1-D7 are good choices as they
meet both of these requirements. See the photo for the wiring that works with
the code example given here.

## LICENSE

See LICENSE
