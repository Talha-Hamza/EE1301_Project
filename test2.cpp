/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "HC-SR04.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);


int trigPin = A0;
int echoPin = D0;

int maxDist = 200;
int minDist = 0;

double distCm = 0.0;
double distIn = 0.0;

void waitForEcho(int pin, int value, long timeout);
void sendTriggerPulse(int pin);
double getDistanceInch();
double getDistanceCM();

double getDistanceCM()
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

double getDistanceInch()
{
    double distCM = getDistanceCM();
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
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop()
{
    distCm = getDistanceCM();
    distIn = getDistanceInch();
    Particle.publish("Distance-CM", String(distCm));
    Particle.publish("Distance-IN", String(distIn));
    Serial.print("Distance-CM: ");
    Serial.println(distCm);
    Serial.print("Distance-IN: ");
    Serial.println(distIn);
    delay(1000);
}

// void setup()
// {
//     Serial.begin(9600);
// }

// void loop()
// {
//     Serial.println("Hello");
//     delay(1000);  // Adjust the delay if needed
// }