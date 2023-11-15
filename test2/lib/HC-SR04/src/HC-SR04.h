//
//    HC-SR04.h
//    Purpose: HC-SR04 driver
//
//    Author: Richard Nash
//    Version: 1.0.1

// This class provides the trigger and timing to use the range
// finder. See the usage example in this library for how to use it.

#pragma once

 class HC_SR04
 {
 public:
   // No default Constructor, need to specify the pins
   HC_SR04() = delete;

   // Constructor
   // Parameters:
   // trigPin : The pin the Trig signal is connected to
   // echoPin : The pin the Echo signal is connected to
   HC_SR04(int trigPin, int echoPin);

   // Call this function in the setup() routine to set the pin modes.
   void init();

   // NOTE: Both distCM() and distInch() block threading AND interrupts for a
   // short period of time. That time can be up to approximately 31 milliseconds

   // Returns the range distance in centimeters OR NO_SIGNAL if range finding
   // failed.
   float distCM();

   // Returns the range distance in inches OR NO_SIGNAL if range finding
   // failed.
   float distInch();

   // Sentinal value for no distance returned
   const float NO_SIGNAL = -1.0f;

 private:
   int trigPin;
   int echoPin;
   unsigned long triggerAndMeasurePulse();
 };
