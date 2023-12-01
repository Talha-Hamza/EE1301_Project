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

void setup()
{
    pinMode(upper_trigPin, OUTPUT);
    pinMode(upper_echoPin, INPUT);

    pinMode(lower_trigPin, OUTPUT);
    pinMode(lower_echoPin, INPUT);

    pinMode(tmp36Pin, INPUT);   
    // setup a particle variable for temperature
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


    Serial.print("Temperature-C: ");
    Serial.println(tempC);
    Serial.println();

    // publish the data
    Particle.publish("p1_distance_upper", String(dist_upper));
    delay(1000);
    Particle.publish("p1_distance_lower", String(dist_lower));
    delay(1000);
    Particle.publish("temperature", String(tempC));
    delay(1000);
}

double getTemperature(int tmp36Pin)
{
    int ADCreading = analogRead(tmp36Pin);
    tempC = (ADCreading - 620) * 0.0806;
    // Serial.print(ADCreading);
    // Serial.print(tempC);
    return tempC;
}