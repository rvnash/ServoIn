//
//    ServoIn.cpp
//    Purpose: Implementation of read servo (RC) signals
//
//    Author: Richard Nash

#include "ServoIn.h"

const static int PIN_DETACHED = -99999; // Sentinal value that the pin is not attached to an interrupt
const static unsigned long TIMEOUT = 25000; // Should have a period of 20 ms, if I haven't heard a pulse in 25ms then I've lost a signal

// Static definitions to handle up to MAX_CHANNELS Interrupt Service Routines
// Note if you increase MAX_CHANNELS, these need to be modified
const static int MAX_CHANNELS = 6;

// Array of Servo objects attached on each channel
// nullptr indicates that slot is empty
static ServoIn *attachedObjects[MAX_CHANNELS] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};

// Create an array of MAX_CHANNELS lambdas that take no parameters, and return no values,
// however they call the ISR member function of the object stored in the attachedObjects
// array at the same index.
static void (* ISR_array [MAX_CHANNELS])() = {
    []() -> void { attachedObjects[0]->ISR(); },
    []() -> void { attachedObjects[1]->ISR(); },
    []() -> void { attachedObjects[2]->ISR(); },
    []() -> void { attachedObjects[3]->ISR(); },
    []() -> void { attachedObjects[4]->ISR(); },
    []() -> void { attachedObjects[5]->ISR(); }
};

ServoIn::ServoIn()
{
  pin = PIN_DETACHED;
  currentUS = NO_SIGNAL;
  attachedISRIndex = -1;
}

bool ServoIn::attach(int pin)
{
  for (int i = 0; i < MAX_CHANNELS; i++) {
    // Find and open slot to attache the ISR to this object
    if (attachedObjects[i] == nullptr) {
      this->pin = pin;
      this->attachedISRIndex = i;
      attachedObjects[this->attachedISRIndex] = this;
      pinMode(pin,INPUT);
      attachInterrupt(pin,ISR_array[this->attachedISRIndex],CHANGE);
      return true;
    }
  }
  // Didn't find an open slot.
  return false;
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
    attachedObjects[attachedISRIndex] = nullptr;
    attachedISRIndex = -1;
  }
}

int ServoIn::read()
{
  int us = readMicroseconds();
  if (us == NO_SIGNAL) return NO_SIGNAL;
  return map(us, 1000, 2000, 0, 180);
}

int ServoIn::readMicroseconds()
{
  noInterrupts();
    // Checking for timeout (lost signals). Variables can change in the interrupt routine
    // between checking and assigning, so protect from interrupts
    if (micros() - readingValidAtUS > TIMEOUT) {
      currentUS = NO_SIGNAL;
    }
  interrupts();
  return currentUS;
}

void ServoIn::ISR()
{
  // This channel has been triggered by rising or falling edge of a pulse
  unsigned long now = micros();
  if (pinReadFast(pin) == HIGH) { // Rising Edge, just record the time
    riseTimeUS = now;
  } else { // Falling edge, remember the pulse width
    int pulseWidth = now - riseTimeUS;
    // Determine if this pulse is a "reasonable" width
    // Normally this should be between 1000 and 2000 microseconds
    // Because some systems go a little longer or shorter for
    // longer throw, go down to 200 and up to 2800 microseconds
    if (pulseWidth > 200 && pulseWidth < 2800) {
      readingValidAtUS = now;
      currentUS = pulseWidth;
    } else {
      currentUS = NO_SIGNAL;
    }
  }
}
