//
//    ServoIn.cpp
//    Purpose: Implementation of read servo signals
//
//    Author: Richard Nash
//    Version: 1.0.0

#include "ServoIn.h"

const int PIN_DETACHED = -99999;
const unsigned long TIMEOUT = 25000; // Should have a period of 20 ms
ServoIn *ServoIn::attachedObjects[MAX_CHANNELS] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};

ServoIn::ServoIn()
{
  pin = PIN_DETACHED;
  currentUS = NO_SIGNAL;
}

bool ServoIn::attach(int pin)
{
  for (int i = 0; i < MAX_CHANNELS; i++) {
    if (attachedObjects[i] == nullptr) {
      attachedObjects[i] = this;
      this->pin = pin;
      pinMode(pin,INPUT);
      // If you change MAX_CHANNELS you need to add more switch statements
      switch (i) {
        case 0: attachInterrupt(pin,ISR0,CHANGE); break;
        case 1: attachInterrupt(pin,ISR1,CHANGE); break;
        case 2: attachInterrupt(pin,ISR2,CHANGE); break;
        case 3: attachInterrupt(pin,ISR3,CHANGE); break;
        case 4: attachInterrupt(pin,ISR4,CHANGE); break;
        case 5: attachInterrupt(pin,ISR5,CHANGE); break;
      }
      return true;
    }
  }
  return false;
}

int ServoIn::read()
{
  int us = readMicroseconds();
  if (us == NO_SIGNAL) return NO_SIGNAL;
  return map(us, 1000, 2000, 0, 180);
}

int ServoIn::readMicroseconds()
{
  // All of these variables can change int he interrupt routines
  // between checking them and returning them, so block out all
  // interrupts
  ATOMIC_BLOCK() {
    if (currentUS == NO_SIGNAL) return currentUS;
    if (micros() - readingValidAtUS > TIMEOUT) {
      currentUS = NO_SIGNAL;
    }
  }
  return currentUS;
}

bool ServoIn::attached()
{
  return (pin != PIN_DETACHED);
}

void ServoIn::detach()
{
  if (attached()) {
    detachInterrupt(pin);
    pin = PIN_DETACHED;
    currentUS = NO_SIGNAL;
    for (int i = 0; i < MAX_CHANNELS; i++) {
      if (attachedObjects[i] == this) {
        attachedObjects[i] = nullptr;
      }
    }
  }
}

void ServoIn::ISR()
{
  unsigned long now = micros();
  if (pinReadFast(pin) == HIGH) {
    riseTimeUS = now;
  } else {
    currentUS = now - riseTimeUS;
    // Determine if this pulse is a "reasonable" width
    // Normally this should be between 1000 and 2000 microseconds
    // Because some systems go a little longer or shorter for
    // longer throw, go down to 200 and up to 2800 microseconds
    if (currentUS > 200 && currentUS < 2800) {
      readingValidAtUS = now;
    } else {
      currentUS = NO_SIGNAL;
    }
  }
}
