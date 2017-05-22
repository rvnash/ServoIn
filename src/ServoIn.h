#pragma once
//
//    ServoIn.h
//    Purpose: Read servo signals
//
//    Author: Richard Nash
//    Version: 1.0.1

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
  const static int MAX_CHANNELS = 6;

  ServoIn();
  bool attach(int pin);
  int read();
  int readMicroseconds();
  bool attached();
  void detach();

private:
  void ISR();
  static ServoIn *attachedObjects[MAX_CHANNELS];
  // If you change MAX_CHANNELS you need to add more static
  // definitions of ISR routines
  static void ISR0() { ServoIn::attachedObjects[0]->ISR(); }
  static void ISR1() { ServoIn::attachedObjects[1]->ISR(); }
  static void ISR2() { ServoIn::attachedObjects[2]->ISR(); }
  static void ISR3() { ServoIn::attachedObjects[3]->ISR(); }
  static void ISR4() { ServoIn::attachedObjects[4]->ISR(); }
  static void ISR5() { ServoIn::attachedObjects[5]->ISR(); }

  int pin;
  volatile int currentUS;
  volatile unsigned long readingValidAtUS;
  volatile unsigned long riseTimeUS;
};
