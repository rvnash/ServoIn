#pragma once
//
//    ServoIn.h
//    Purpose: Read servo signals
//
//    Author: Richard Nash

// This class provides interrupt driven read of Servo pulses from
// a remote control device, like for RC vehicles.
// It is designed to operate exactly like the Servo class in
// the Arduino library, with these exceptions. There are no
// "write" functions, as this is for reading only. There is the
// additional "readMicroseconds" function which returns the current
// pulse width in microseconds. If the value "NO_SIGNAL" is returned
// then the pulses either are absent, or don't conform to normal RC
// Servo encoding.

#include "Particle.h"

class ServoIn
{
public:
  const static int NO_SIGNAL = -99999;

  // Servo Functions
  
  // Constructor
  ServoIn();
  
  // Attaches this ServoIn object to a pin.
  // pin - Pin number (eg. D1, D2, ...) attached pin must be able to attachInterrupts
  // Returns true if the attach worked, false otherwise.
  bool attach(int pin);
  
  // Returns true if the attach worked, false otherwise.
  bool attached();
  
  // Detaches pin from the interrupt and puts it in the detached state.
  void detach();
  
  // Returns the servo "angle" read from the pin. Generally it will be in the range 0..180,
  // with 90 being the "center" position. But for some RC systems values can go outside
  // this range.
  // Returns NO_SIGNAL, if a valid servo signal is not recieved.
  int read();
  
  // Returns the number of microseconds of the last pulse recieved. Generally it will
  // be in the range 1000..2000, with 1500 being the center position. But for some RC systems
  // the values may go outside the range.
  // Returns NO_SIGNAL, if a valid servo signal is not recieved.
  int readMicroseconds();

  // Interrupt service routine, not meant to be called publicly
  void ISR();

private:
  int pin;
  int attachedISRIndex;
  volatile int currentUS;
  volatile unsigned long readingValidAtUS;
  volatile unsigned long riseTimeUS;
};
