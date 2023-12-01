#include "Particle.h"
#include "HC-SR04.h"

int leftTrigPin1 = A0;
int leftEchoPin1 = D0;
int leftTrigPin2 = A1;
int leftEchoPin2 = D1;

double leftDistCm1 = 0.0;
double leftDistIn1 = 0.0;
double leftDistCm2 = 0.0;
double leftDistIn2 = 0.0;

void setup()
{
    pinMode(leftTrigPin1, OUTPUT);
    pinMode(leftEchoPin1, INPUT);
    pinMode(leftTrigPin2, OUTPUT);
    pinMode(leftEchoPin2, INPUT);
}

void loop()
{
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
}
