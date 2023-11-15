//
//    usage.h
//    Purpose: Demonstration of how to use the HC-SR04 driver
//
//    Author: Richard Nash
//    Version: 1.0.1

#include "HC-SR04.h"

/*
 The HC-SR04 device is a 5V device. So, VIN (which is 5V when connected to a USB power supply)
 is used to power it. Also, the "Echo" pin will present a 5V pulse, which can be connected
 to any of the D* GPIO pins, as they are 5V tolerant. However, they cannot be connected to
 non-5V tolerant pins, like the A* pins, even if in digitial mode.

 This example expects the wiring to be as follows:
    Photon  HC-SR04
    GND     GND
    VIN     VCC
    A0      Trig
    D0      Echo
*/

// trigger / echo pins
const int triggerPin = A0;
const int echoPin = D0;
HC_SR04 rangefinder = HC_SR04(triggerPin, echoPin);

void setup()
{
    Serial.begin(9600);
    rangefinder.init();
}

void loop()
{
    unsigned long start = micros();
    float inch = rangefinder.distInch();
    unsigned long calcTime = micros() - start;
    Serial.print("Reached here");
    // Serial.printf("Range finding duration: %lu | Distance in inches: %.2f\n", calcTime, inch);
    delay(500);
}


