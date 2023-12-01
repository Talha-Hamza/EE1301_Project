#include "Particle.h"
#include "HC-SR04.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// Sensor 1 pins -- upper sensor
int upper_trigPin = A0;
int upper_echoPin = D0;

// Sensor 2 pins -- lower sensor
int lower_trigPin = A1;
int lower_echoPin = D1;

int maxDist = 200;
int minDist = 0;

double dist_upper = 0.0;
double dist_lower = 0.0;

// Particle variables
double p1_distance_upper;
double p1_distance_lower;


void waitForEcho(int pin, int value, long timeout);
void sendTriggerPulse(int pin);
double getDistanceCM(int trigPin, int echoPin);

void setup()
{
    pinMode(upper_trigPin, OUTPUT);
    pinMode(upper_echoPin, INPUT);

    pinMode(lower_trigPin, OUTPUT);
    pinMode(lower_echoPin, INPUT);

    // Register Particle variables
    Particle.variable("p1_distance_upper", p1_distance_upper);
    Particle.variable("p1_distance_lower", p1_distance_lower);
}

void loop()
{
    dist_upper = getDistanceCM(upper_trigPin, upper_echoPin);
    dist_lower = getDistanceCM(lower_trigPin, lower_echoPin);

    // Update Particle variables
    p1_distance_upper = dist_upper;
    p1_distance_lower = dist_lower;

    // No need to use Particle.publish here
    delay(1000);

}


double getDistanceCM(int trigPin, int echoPin)
{
    sendTriggerPulse(trigPin);
    waitForEcho(echoPin, HIGH, 100);
    long startTime = micros();
    waitForEcho(echoPin, LOW, 100);
    long endTime = micros();
    long duration = endTime - startTime;
    double distance = duration / 29.0 / 2.0;
    if (distance < minDist || distance > maxDist)
    {
        return -1;
    }
    return distance;
}

void sendTriggerPulse(int pin)
{
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);
}

void waitForEcho(int pin, int value, long timeout)
{ 
    unsigned long giveupTime = millis() + timeout;
    while (digitalRead(pin) != value && millis() < giveupTime)
    {
    }
}
