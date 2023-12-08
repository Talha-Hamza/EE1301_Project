/* 
 * Project IoT Doorman
 * Authors: Juliana Nyberg (nyber197), Talha Hamza (hamza050), Shivam Vashishtha (vashi025)
 * Date: Fall 2023
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "neopixel.h"
#include <cstring>

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(DISABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

int PIXEL_PIN = D4;
int PIXEL_COUNT = 1;
int PIXEL_TYPE = WS2812;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int speakerPIN = D0;
int buttonPIN = D3;

// setting tracking variables for button state and siren state
bool ButtonNow = false;
bool ButtonLast = false;
int ButtonCount = 0;
bool buttonState = false;
bool sirenPlaying = false;

// setting variables for siren timing to help with tone changes
unsigned long sirenStartTime = 0;
unsigned long sirenDuration = 0;
unsigned long sirenToneChangeTime = 0;
bool isHighTone = true;

// declare local variables to get data from other photons
double lDistanceLower = 0.0;
double lTemperature = 0.0;
String lDistanceUpdate;

/*
This function controls the siren sound. 
It takes a boolean parameter, play, which determines whether the siren should be played or stopped. 
Inside the function: currentTime is used to track the current time using millis().
If play is true and the siren is not currently playing, it starts the siren with alternating high and low tones, changing every 500 ms.
If the siren is already playing, it checks the elapsed time and changes the tone every 500 ms.
If play is false, it immediately stops the siren if it's currently playing.
*/

void siren(bool play) {
    unsigned long currentTime = millis();

    if (play) {
        if (!sirenPlaying) {
            // Start playing the siren
            tone(speakerPIN, isHighTone ? 880 : 440);
            isHighTone = !isHighTone;
            sirenToneChangeTime = currentTime;
            sirenPlaying = true;
        } else if (currentTime - sirenToneChangeTime >= 500) {
            tone(speakerPIN, isHighTone ? 880 : 440);
            isHighTone = !isHighTone;
            sirenToneChangeTime = currentTime;
        }
    } else if (sirenPlaying) {
        noTone(speakerPIN); // Stop the siren immediately if button state is off
        sirenPlaying = false;
    }
}

void distanceHandlerUpdate(const char *event, const char *data){
  Serial.print("Updated motion was detected at ");
  Serial.println(data);
  lDistanceUpdate = data;
}

void tempHandler(const char *event, const char *data){
  Serial.print("Current temp: ");
  Serial.println(data);
  lTemperature = strtod(data, NULL);
}

void setup() {
  strip.begin();
  
  pinMode(buttonPIN, INPUT_PULLDOWN);
  Serial.begin(9600);


  // subscribe to cloud events for distance sensor
  Particle.subscribe("p1_activity", distanceHandlerUpdate);
  Particle.subscribe("p2_activity", distanceHandlerUpdate);
  
}

/*
Notes about the function of the button code:
The loop function continuously checks the state of the button using digitalRead.
If the button transitions from low to high, it registers the button press, increments ButtonCount, toggles buttonState, and logs the button state ("Button On" or "Button Off"). 
It also updates ButtonLast to track the previous button state.
Finally, it calls the siren function with the current buttonState, controlling whether the siren plays or stops.
*/
void loop() {
  int PixelColorCyan = strip.Color(255, 0, 255);
  int PixelColorRed = strip.Color(0, 80, 0);

  if(lTemperature > 21.0) {
    strip.setPixelColor(0, PixelColorRed);
  } else {
    strip.setPixelColor(0, PixelColorCyan);
  }
  strip.show();

  // start of button code
  ButtonNow = digitalRead(buttonPIN);
  if (ButtonNow == HIGH && ButtonLast == LOW) {
    // Button was pressed (buzzer can be tested manually by pressing the button)
    ButtonCount++;
    buttonState = !buttonState; // Toggle button state
    Serial.print("Button Count = ");
    Serial.print(ButtonCount);
    Serial.print(", Button State = ");
    if (buttonState) {
      Serial.println("Button On");
    } else {
      Serial.println("Button Off");
    }

    ButtonLast = HIGH;
  } else if (ButtonNow == LOW) {
    ButtonLast = LOW;
  }
  siren(buttonState);

  // if one of the lower distance sensors detects a package, activate buzzer
  if(lDistanceUpdate == "lower") {
    buttonState = true;
    siren(buttonState);
    lDistanceUpdate = "none"; // reset variable to default
  }
  lDistanceUpdate = "none"; // reset variable to default

}
