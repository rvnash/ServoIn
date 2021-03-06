#pragma once
//
//    usage.ino
//    Purpose: Example of ServoIn usage
//
//    Author: Richard Nash

// The ServoIn library is intended to be like the Servo library, except
// for reading server values. Please see the documentation on Servo.

// To run this example you need to have a six channel RC receiver hooked up to your
// Photon or Electron. If you want to start smaller, just cut this down to a single channel
// example.
// See the wiring nots in the README.md file on github.

#include "ServoIn.h"

ServoIn channel1;
ServoIn channel2;
ServoIn channel3;
ServoIn channel4;
ServoIn channel5;
ServoIn channel6;

void setup()
{
  Serial.begin(9600);
  channel1.attach(D1);
  channel2.attach(D2);
  channel3.attach(D3);
  channel4.attach(D4);
  channel5.attach(D5);
  channel6.attach(D6);
}

void loop()
{
  Serial.printf("%3d | %3d | %3d | %3d | %3d | %3d\n",
                channel1.read(),
                channel2.read(),
                channel3.read(),
                channel4.read(),
                channel5.read(),
                channel6.read()
              );
  delay(200);
}
