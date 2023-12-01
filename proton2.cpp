#include "Particle.h"

// Function prototypes
void handleDistanceUpper(const char *event, const char *data);
void handleDistanceLower(const char *event, const char *data);
void handleTemperature(const char *event, const char *data);

// Particle variables
double p1_distance_upper = 0.0;
double p1_distance_lower = 0.0;
double temperature = 0.0;

void setup() {
  Serial.begin(9600);
  Particle.subscribe("p1_distance_upper", handleDistanceUpper, MY_DEVICES);
  Particle.subscribe("p1_distance_lower", handleDistanceLower, MY_DEVICES);
  Particle.subscribe("temperature", handleTemperature, MY_DEVICES);
}

void loop() {
  // print the values to the serial monitor
  Serial.print("Distance Upper: ");
  Serial.println(p1_distance_upper);
  Serial.print("Distance Lower: ");
  Serial.println(p1_distance_lower);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  delay(1000);
  
}

void handleDistanceUpper(const char *event, const char *data) {
  p1_distance_upper = atof(data);
  Serial.print("Distance Upper: ");
  Serial.println(p1_distance_upper);
}

void handleDistanceLower(const char *event, const char *data) {
  p1_distance_lower = atof(data);
  Serial.print("Distance Lower: ");
  Serial.println(p1_distance_lower);
}

void handleTemperature(const char *event, const char *data) {
  temperature = atof(data);
  Serial.print("Temperature: ");
  Serial.println(temperature);
}




// #include "Particle.h"
// #include "HC-SR04.h"

// SYSTEM_MODE(AUTOMATIC);
// SYSTEM_THREAD(ENABLED);
// SerialLogHandler logHandler(LOG_LEVEL_INFO);

// int leftTrigPin1 = A0;
// int leftEchoPin1 = D0;
// int leftTrigPin2 = A1;
// int leftEchoPin2 = D1;

// double leftDistCm1 = 0.0;
// double leftDistCm2 = 0.0;
// void waitForEcho(int pin, int value, long timeout);
// void sendTriggerPulse(int pin);
// double getDistanceInch(int trigPin, int echoPin);
// double getDistanceCM(int trigPin, int echoPin);
// double getTemperature(int tmp36Pin);

// void motionHandler(const char *event, const char *data);
// void triggerPackageAlert();
// void triggerPersonAlert();

// bool rightPackageDetected = false;
// bool motionDetected = false;
// unsigned long motionAlertStartTime = 0;
// unsigned long packageAlertStartTime = 0;
// const unsigned long packageAlertDuration = 5000; // 5 seconds


// void setup()
// {

//     pinMode(leftTrigPin1, OUTPUT);
//     pinMode(leftEchoPin1, INPUT);
//     pinMode(leftTrigPin2, OUTPUT);
//     pinMode(leftEchoPin2, INPUT);


//     Particle.subscribe("motion", motionHandler, MY_DEVICES);
// }

// void loop()
// {


//     if (leftDistCm1 < 50.0 && leftDistCm2 < 50.0)
//     {
//         rightPackageDetected = true;
//         motionDetected = false;
//         motionAlertStartTime = 0;

//         if (packageAlertStartTime == 0)
//         {
//             packageAlertStartTime = millis();
//         }
//         else if (millis() - packageAlertStartTime >= packageAlertDuration)
//         {
//             triggerPackageAlert();
//             packageAlertStartTime = 0;
//         }
//     }
//     else
//     {
//         rightPackageDetected = false;
//         packageAlertStartTime = 0;
//     }

//     if (rightPackageDetected)
//     {
//         triggerPackageAlert();
//     }
//     else if (motionDetected)
//     {
//         triggerPersonAlert();
//     }
//     delay(1000);

//     leftDistCm1 = getDistanceCM(leftTrigPin1, leftEchoPin1);
//     leftDistCm2 = getDistanceCM(leftTrigPin2, leftEchoPin2);

//     Serial.print("Left-Distance-CM-Sensor1: ");
//     Serial.println(leftDistCm1);
//     Serial.print("Left-Distance-CM-Sensor2: ");
//     Serial.println(leftDistCm2);
//     delay(1000);
// }

// void waitForEcho(int pin, int value, long timeout)
// {
//     long unsigned giveupTime = millis() + timeout;
//     while (digitalRead(pin) != value && millis() < giveupTime)
//     {
//     }
// }

// void sendTriggerPulse(int pin)
// {
//     digitalWrite(pin, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(pin, LOW);
// }

// double getDistanceCM(int trigPin, int echoPin)
// {
//     sendTriggerPulse(trigPin);
//     waitForEcho(echoPin, HIGH, 100);
//     long startTime = micros();
//     waitForEcho(echoPin, LOW, 100);
//     long endTime = micros();
//     long duration = endTime - startTime;
//     double distance = duration / 29.0 / 2.0;
//     if (distance < 0.0 || distance > 200.0)
//     {
//         return -1;
//     }
//     return distance;
// }

// double getDistanceInch(int trigPin, int echoPin)
// {
//     double distCM = getDistanceCM(trigPin, echoPin);
//     if (distCM == -1)
//     {
//         return -1;
//     }
//     return (distCM / 2.54);
// }

// double getTemperature(int tmp36Pin)
// {
//     int ADCreading = analogRead(tmp36Pin);
//     double tempCelsius = (ADCreading - 620) * 0.0806;
//     return tempCelsius;
// }

// void motionHandler(const char *event, const char *data)
// {
//     motionDetected = true;
//     motionAlertStartTime = millis();
// }

// void triggerPackageAlert()
// {
//     Particle.publish("package_alert", "Package detected");
// }

// void triggerPersonAlert()
// {
//     Particle.publish("person_alert", "Person detected");
// }

