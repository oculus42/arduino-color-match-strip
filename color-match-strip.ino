#include <Wire.h>
#include "Adafruit_TCS34725.h"

#include "color_data.h"

#include "color_set.h"

// RGB Sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

#include "wire_color.h"

// IR Setup
#include "ir_lib.h"

// ******** Globals

bool poweredOn = true;
bool getColorSensorData = true;

color_s nextColor = offColor;

// Get Color from RGB Sensor
uint16_t clear, red, green, blue;

uint32_t sum;
float r, g, b;

// ******** SETUP

// Use the onboard LED as a failure indicator
void setupWarningLight() {
  pinMode(LED_BUILTIN, OUTPUT);
}

// Check for the sensor
void setupRGBSensor() {
  // If there is an error starting the color reader, turn on the light and hang.
  if (!tcs.begin()) {
    indicateError();
  }
}


void setup() {
  setupWarningLight();
  LEDStrip::setup();
  setupRGBSensor();
  IR::setup();
  WireColor::setup();
}

// ******** Support Functions

// Should something go wrong, provide a way to tell
void indicateError() {
  digitalWrite(LED_BUILTIN, HIGH);
  while (1); // halt!
}

void blinkLED(uint8_t delayLength) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(delayLength);
  digitalWrite(LED_BUILTIN, LOW);
}

void blinkLED(uint8_t delayLength, uint8_t times) {
  blinkLED(delayLength);
  if (times > 0) {
    for(uint8_t i = 1; i < times; ++i) {
      delay(delayLength);
      blinkLED(delayLength);
    }
  }
}

void getNextColorFromSensor() {

  tcs.setInterrupt(false);      // turn on LED
  delay(60);  // takes 50ms to read
  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(true);  // turn off LED

  // Figure out some basic hex code for visualization
  sum = clear;

  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 255; g *= 255; b *= 255;

  LEDStrip::setColor({byte(r), byte(g), byte(b)});
}

void loop() {
  // Delay up front. The RGB strip starts in a random state.
  // If in a position to cause feedback, this should avoid it.
  delay(200);

  if (WireColor::hasData()) {
    
    getColorSensorData = false;
    poweredOn = true;
    WireColor::setHasData(false);
    nextColor = WireColor::getColor();
    LEDStrip::setColor();
    
  } else if (IR::hasData()) {
    IR::processData();
  } else if (poweredOn && getColorSensorData) {
    getNextColorFromSensor();
    LEDStrip::setColor();
  }
}
