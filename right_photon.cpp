#include "Particle.h"
#include "HC-SR04.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

int rightTrigPin1 = A0;
int rightEchoPin1 = D0;
int rightTrigPin2 = A1;
int rightEchoPin2 = D1;
int tmp36Pin = A4;

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

void motionHandler(const char *event, const char *data);
void triggerPackageAlert();
void triggerPersonAlert();

bool rightPackageDetected = false;
bool motionDetected = false;
unsigned long motionAlertDuration = 0;

void setup()
{
    pinMode(rightTrigPin1, OUTPUT);
    pinMode(rightEchoPin1, INPUT);
    pinMode(rightTrigPin2, OUTPUT);
    pinMode(rightEchoPin2, INPUT);
    pinMode(tmp36Pin, INPUT);

    Particle.subscribe("motion", motionHandler, MY_DEVICES);
}

void loop()
{
    distCm1 = getDistanceCM(rightTrigPin1, rightEchoPin1);
    distIn1 = getDistanceInch(rightTrigPin1, rightEchoPin1);

    distCm2 = getDistanceCM(rightTrigPin2, rightEchoPin2);
    distIn2 = getDistanceInch(rightTrigPin2, rightEchoPin2);

    tempC = getTemperature(tmp36Pin);

    if (distCm1 < 50.0 && distCm2 < 50.0)
    {
        rightPackageDetected = true;
        motionDetected = false;
        motionAlertDuration = 0;
    }
    else
    {
        rightPackageDetected = false;
    }

    if (rightPackageDetected)
    {
        triggerPackageAlert();
    }
    else if (motionDetected)
    {
        triggerPersonAlert();
    }

    if (tempC > 10.0)
    {
        Particle.publish("outdoor_conditions", "High temperature detected");
    }

    delay(1000);
}
