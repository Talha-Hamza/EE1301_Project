#include "Particle.h"
#include "HC-SR04.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// Sensor 1 pins
int trigPin1 = A0;
int echoPin1 = D0;

// Sensor 2 pins
int trigPin2 = A1;
int echoPin2 = D1;

int tmp36Pin = A4;

int maxDist = 200;
int minDist = 0;

double distCm1 = 0.0;
double distIn1 = 0.0;

double distCm2 = 0.0;
double distIn2 = 0.0;

double tempC = 0.0;


void waitForEcho(int pin, int value, long timeout);
void sendTriggerPulse(int pin);
double getDistanceInch(int trigPin, int echoPin);
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

double getDistanceInch(int trigPin, int echoPin)
{
    double distCM = getDistanceCM(trigPin, echoPin);
    if (distCM == -1)
    {
        return -1;
    }
    return (distCM / 2.5);
}

void sendTriggerPulse(int pin)
{
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);
}

void waitForEcho(int pin, int value, long timeout)
{
    long giveupTime = millis() + timeout;
    while (digitalRead(pin) != value && millis() < giveupTime)
    {
    }
}

void setup()
{
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);

    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);

    pinMode(tmp36Pin, INPUT);   
}

void loop()
{
    distCm1 = getDistanceCM(trigPin1, echoPin1);
    distIn1 = getDistanceInch(trigPin1, echoPin1);

    distCm2 = getDistanceCM(trigPin2, echoPin2);
    distIn2 = getDistanceInch(trigPin2, echoPin2);

    
    tempC = getTemperature(tmp36Pin);
    
    // Particle.publish("Distance-CM-Sensor1", String(distCm1));
    // Particle.publish("Distance-IN-Sensor1", String(distIn1));

    // Particle.publish("Distance-CM-Sensor2", String(distCm2));
    // Particle.publish("Distance-IN-Sensor2", String(distIn2));

    Serial.print("Distance-CM-Sensor1: ");
    Serial.println(distCm1);
    Serial.print("Distance-IN-Sensor1: ");
    Serial.println(distIn1);

    Serial.print("Distance-CM-Sensor2: ");
    Serial.println(distCm2);
    Serial.print("Distance-IN-Sensor2: ");
    Serial.println(distIn2);

        Serial.print("Temperature-C: ");
    Serial.println(tempC);


    delay(1000);
}

double getTemperature(int tmp36Pin)
{
    int ADCreading = analogRead(tmp36Pin);
    tempC = (ADCreading - 620) * 0.0806;
    Serial.print(ADCreading);
    Serial.print(tempC);
    return tempC;
}