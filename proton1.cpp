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

int tmp36Pin = A4;

int maxDist = 200;
int minDist = 0;

double dist_upper = 0.0;
double dist_lower = 0.0;


double tempC = 0.0;


void waitForEcho(int pin, int value, long timeout);
void sendTriggerPulse(int pin);
double getDistanceCM(int trigPin, int echoPin);
double getTemperature(int tmp36Pin);
bool activity_lower();
bool activity_upper();
bool activity_both();

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

// if there is activity at the lower sensor for 5 seconds, return true, withouy using delays
bool activity_lower()
{
    unsigned long startTime = millis();
    unsigned long endTime = startTime + 5000;
    while (millis() < endTime)
    {
        if (dist_lower < 50)
        {
            return true;
        }
    }
    return false;
}

// if there is activity at the upper sensor for 5 seconds, return true, withouy using delays
bool activity_upper()
{
    unsigned long startTime = millis();
    unsigned long endTime = startTime + 5000;
    while (millis() < endTime)
    {
        if (dist_upper < 50)
        {
            return true;
        }
    }
    return false;
}

// if there is activity at both sensors for 5 seconds, return true, withouy using delays
bool activity_both()
{
    unsigned long startTime = millis();
    unsigned long endTime = startTime + 5000;
    while (millis() < endTime)
    {
        if (dist_upper < 50 && dist_lower < 50)
        {
            return true;
        }
    }
    return false;
}



void setup()
{
    pinMode(upper_trigPin, OUTPUT);
    pinMode(upper_echoPin, INPUT);

    pinMode(lower_trigPin, OUTPUT);
    pinMode(lower_echoPin, INPUT);

    pinMode(tmp36Pin, INPUT);   
    Particle.variable("temperature", tempC);

}

void loop()
{
    dist_upper = getDistanceCM(upper_trigPin, upper_echoPin);
    dist_lower = getDistanceCM(lower_trigPin, lower_echoPin);
    tempC = getTemperature(tmp36Pin);

    Serial.print("Distance at 2ft: ");
    Serial.println(dist_upper);
    Serial.print("Distance at ground: ");
    Serial.println(dist_lower);
    Serial.print("Temperature: ");
    Serial.println(tempC);

    String activity = "none"; // Default value

    if (activity_lower())
    {
        Serial.println("Activity at lower sensor");
        if (activity != "lower")
        {
            Particle.publish("p1_activity", "lower");
            activity = "lower";
        }
    }
    else if (activity_upper())
    {
        Serial.println("Activity at upper sensor");
        if (activity != "upper")
        {
            Particle.publish("p1_activity", "upper");
            activity = "upper";
        }
    }
    else if (activity_both())
    {
        Serial.println("Activity at both sensors");
        if (activity != "both")
        {
            Particle.publish("p1_activity", "both");
            activity = "both";
        }
    }
    else
    {
    Serial.println("No activity");
        if (activity != "none")
        {
            Particle.publish("p1_activity", "none");
            activity = "none";
        }

    Serial.println(activity);
    // Particle.publish("p1_activity", activity);

    delay(1000);
    }


double getTemperature(int tmp36Pin){
    int ADCreading = analogRead(tmp36Pin);
    tempC = (ADCreading - 620) * 0.0806;
    // Serial.print(ADCreading);
    // Serial.print(tempC);
    return tempC;
}