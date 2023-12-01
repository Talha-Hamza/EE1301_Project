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

String prevActivity = "none"; // Variable to store previous activity status

unsigned long startTime = 0;

void waitForEcho(int pin, int value, long timeout);
void sendTriggerPulse(int pin);
double getDistanceCM(int trigPin, int echoPin);
bool activity_lower();
bool activity_upper();

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

void publishActivity(String currentActivity)
{
    if (currentActivity != prevActivity)
    {
        Particle.publish("p1_activity", currentActivity);
        prevActivity = currentActivity;
    }
}

// if there is activity at the lower sensor, return true
bool activity_lower()
{
    return dist_lower < 50;
}

// if there is activity at the upper sensor, return true
bool activity_upper()
{
    return dist_upper < 50;
}

void setup()
{
    pinMode(upper_trigPin, OUTPUT);
    pinMode(upper_echoPin, INPUT);

    pinMode(lower_trigPin, OUTPUT);
    pinMode(lower_echoPin, INPUT);

    startTime = millis();
}

void loop()
{
    dist_upper = getDistanceCM(upper_trigPin, upper_echoPin);
    dist_lower = getDistanceCM(lower_trigPin, lower_echoPin);

    Serial.print("Distance at 2ft: ");
    Serial.println(dist_upper);
    Serial.print("Distance at ground: ");
    Serial.println(dist_lower);

    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - startTime;

    if (elapsedTime >= 5000)
    {
        if (activity_lower() && activity_upper())
        {
            Serial.println("Activity at both sensors");
            publishActivity("both");
        }
        else if (activity_lower())
        {
            Serial.println("Activity at lower sensor");
            publishActivity("lower");
        }
        else if (activity_upper())
        {
            Serial.println("Activity at upper sensor");
            publishActivity("upper");
        }
        else
        {
            Serial.println("No activity");
            publishActivity("none");
        }

        // Reset the timer
        startTime = millis();
    }

    delay(1000);
}
