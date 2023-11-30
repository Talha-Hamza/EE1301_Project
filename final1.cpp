#include "Particle.h"
#include "HC-SR04.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// Right Photon - Outdoor with HC-SR04 and TMP36
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

// Left Photon - Outdoor with HC-SR04
int leftTrigPin1 = A0;
int leftEchoPin1 = D0;
int leftTrigPin2 = A1;
int leftEchoPin2 = D1;

double leftDistCm1 = 0.0;
double leftDistIn1 = 0.0;
double leftDistCm2 = 0.0;
double leftDistIn2 = 0.0;

// Inside Photon - Indoor with Buzzer
int insideBuzzerPin = A1;

void setup()
{
    pinMode(rightTrigPin1, OUTPUT);
    pinMode(rightEchoPin1, INPUT);
    pinMode(rightTrigPin2, OUTPUT);
    pinMode(rightEchoPin2, INPUT);
    pinMode(tmp36Pin, INPUT);

    pinMode(leftTrigPin1, OUTPUT);
    pinMode(leftEchoPin1, INPUT);
    pinMode(leftTrigPin2, OUTPUT);
    pinMode(leftEchoPin2, INPUT);

    pinMode(insideBuzzerPin, OUTPUT);

    Particle.subscribe("motion", motionHandler, MY_DEVICES);
}

void loop()
{
    // Right Photon Loop
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

    if (tempC > 25.0)
    {
        Particle.publish("outdoor_conditions", "High temperature detected");
    }

    delay(1000);

    // Left Photon Loop
    leftDistCm1 = getDistanceCM(leftTrigPin1, leftEchoPin1);
    leftDistIn1 = getDistanceInch(leftTrigPin1, leftEchoPin1);

    leftDistCm2 = getDistanceCM(leftTrigPin2, leftEchoPin2);
    leftDistIn2 = getDistanceInch(leftTrigPin2, leftEchoPin2);

    Particle.publish("Left-Distance-CM-Sensor1", String(leftDistCm1));
    Particle.publish("Left-Distance-IN-Sensor1", String(leftDistIn1));

    Particle.publish("Left-Distance-CM-Sensor2", String(leftDistCm2));
    Particle.publish("Left-Distance-IN-Sensor2", String(leftDistIn2));

    Serial.print("Left-Distance-CM-Sensor1: ");
    Serial.println(leftDistCm1);
    Serial.print("Left-Distance-IN-Sensor1: ");
    Serial.println(leftDistIn1);

    Serial.print("Left-Distance-CM-Sensor2: ");
    Serial.println(leftDistCm2);
    Serial.print("Left-Distance-IN-Sensor2: ");
    Serial.println(leftDistIn2);

    delay(1000);

    // Inside Photon Loop
    // inside Photon here
    delay(1000);
}

void waitForEcho(int pin, int value, long timeout)
{
    long giveupTime = millis() + timeout;
    while (digitalRead(pin) != value && millis() < giveupTime)
    {
    }
}

void sendTriggerPulse(int pin)
{
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);
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
    if (distance < 0.0 || distance > 200.0)
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
    return (distCM / 2.54);
}

double getTemperature(int tmp36Pin)
{
    int ADCreading = analogRead(tmp36Pin);
    double tempCelsius = (ADCreading - 620) * 0.0806;
    return tempCelsius;
}

void motionHandler(const char *event, const char *data)
{
    motionDetected = true;
    motionAlertDuration = millis();
}

void triggerPackageAlert()
{
    Particle.publish("package_alert", "Package detected");
}

void triggerPersonAlert()
{
    Particle.publish("person_alert", "Person detected");
}
