#include "HC-SR04.h"
#include "Particle.h"

HC_SR04::HC_SR04(int trigPin, int echoPin)
{
    this->trigPin = trigPin;
    this->echoPin = echoPin;
}

void HC_SR04::init()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float HC_SR04::distCM()
{
    // Speed of sound is approx 343 m/s
    // 343 m/s * 100 cm/m * 0.000001 s/us / 2.0 trips
    const float uSecondsToCM = ((340.0f * 100.0 * 0.000001f) /  2.0f);
    unsigned long timeUntilLow = triggerAndMeasurePulse();
    if (timeUntilLow == 0) return NO_SIGNAL;
    return (float)timeUntilLow * uSecondsToCM;
}

float HC_SR04::distInch()
{

    // Speed of sound at 70 degrees F is approximately 1128 ft/s
    // 1128 ft/s * 12 inch/ft * 0.000001 s/us / 2.0 trips
    const float uSecondsToInch = ((1128.0f * 12.0 * 0.000001f) /  2.0f);
    unsigned long timeUntilLow = triggerAndMeasurePulse();
    if (timeUntilLow == 0) return NO_SIGNAL;
    return (float)timeUntilLow * uSecondsToInch;
}

unsigned long HC_SR04::triggerAndMeasurePulse()
{
    // Response pulse usually starts in under 500 uSecs, wait up to 2ms to be sure.
    const unsigned long timeoutHigh = 2000;
    // Response pulse should be shorter than about 29ms (allow 5 meters as the limit)
    const unsigned long timeoutLow = 29000;
    unsigned long start = 0;
    unsigned long duration = 0;

    // Timing is crucial here, so cannot allow other threads or interrupts
    // Maximum amount of time in this block is limited to 10 + 2,000 + 29,000 uS
    // Or 31 milliseconds
    ATOMIC_BLOCK() {
        // Send the 10 uSec pulse
        pinSetFast(trigPin);
        delayMicroseconds(10);
        pinResetFast(trigPin);

        start = micros();
        while (pinReadFast(echoPin) != HIGH) {
            duration = micros() - start;
            if (duration >= timeoutHigh) {
                // Didn't receive a pulse
                return 0;
            }
        }

        start = micros();
        while (pinReadFast(echoPin) != LOW) {
            duration = micros() - start;
            if (duration >= timeoutLow) {
                // Pulse lasted longer than the 5-meter range limit
                return 0;
            }
        }
    }

    return duration; // Return the measured duration
}