#include <Wire.h>

// ********** Neopixel
// Neopixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define NEOPIN 6
#define NEOCOUNT 9

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOCOUNT, NEOPIN, NEO_GRB + NEO_KHZ800);

// ********** RGB Strip Controller - Next LED Strip Driver Deep Blue ARD0689
#include "RGBdriver.h"
#define CLK A3 //pins definitions for the driver        
#define DIO A2

RGBdriver Driver(CLK, DIO);

// ********** RGB Sensor - Adafruit TCS34725
#include "Adafruit_TCS34725.h"

#define brightness 128

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// our RGB -> eye-recognized gamma color
byte gammatable[256];

// Simple struct for passing around colors
struct color {
  byte red;
  byte green;
  byte blue;
};

// Set the default at off.
color stripColor = { 0, 0, 0 };

// Gamma Table code from the Adafruit_TCS34725 example
void generateGammaTable() {
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    gammatable[i] = x;
  }
}

// Use the onboard LED as a failure indicator
void setupWarningLight() {
  pinMode(LED_BUILTIN, OUTPUT);
}

// Should something go wrong, provide a way to tell
void indicateError() {
  digitalWrite(LED_BUILTIN, HIGH);
  while (1); // halt!
}

// Check for the sensor
void setupRGBSensor() {
  // If there is an error starting the color reader, turn on the light and hang.
  if (!tcs.begin()) {
    indicateError();
  }
}


void setupNeoPixels() {
  strip.begin();
  // All off
  strip.show();
}

// LED Strip can start in unexpected state so we must turn it off.
void setupLEDStrip() {
  // Turn off strip
  Driver.begin();
  Driver.SetColor(0, 0, 0);
  Driver.end();
}

void setup() {
  // Serial.begin(9600);
  // Serial.println("Color Match");
  setupWarningLight();
  generateGammaTable();
  setupNeoPixels();
  setupLEDStrip();
  setupRGBSensor();
}


color getColor() {
  uint16_t clear, red, green, blue;

  tcs.setInterrupt(false);      // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(true);  // turn off LED

  //  Serial.print("C: "); Serial.print(clear);
  //  Serial.print("\tR: "); Serial.print(red);
  //  Serial.print("\tG: "); Serial.print(green);
  //  Serial.print("\tB: "); Serial.print(blue);

  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= brightness; g *= brightness; b *= brightness;

  color returnColor = { byte(r), byte(g), byte(b) };

  return returnColor;
}

void shiftColorHistory() {
  for (uint8_t i = 8; i > 0; i--) {
    strip.setPixelColor(i, strip.getPixelColor(i - 1));
  }
}

void pushColorHistory(uint32_t newColor) {
  shiftColorHistory();
  strip.setPixelColor(0, newColor);
  strip.show();
}

void setStripColor(color current) {
  Driver.begin(); // begin
  Driver.SetColor(current.red, current.green, current.blue);
  Driver.end();
}

int getMaxColorDifference(color oldColor, color newColor) {
  return max(abs(oldColor.red - newColor.red), max(abs(oldColor.green - newColor.green), abs(oldColor.blue - newColor.blue)));
}

// Fade between colors. Fade time depends on largest R/G/B color difference
void fadeStripColor(color oldColor, color newColor) {
  float redDiff = newColor.red - oldColor.red;
  float greenDiff = newColor.green - oldColor.green;
  float blueDiff = newColor.blue - oldColor.blue;
  byte i = 0;
  byte len = getMaxColorDifference(oldColor, newColor);
  float offset;
  color tween = { 0, 0, 0 };
  
  for (i = 1; i <= len; i++) {
    offset = (float)i / (float)len;
    tween.red = oldColor.red + (int)(redDiff * offset);
    tween.green = oldColor.green + (int)(greenDiff * offset);
    tween.blue = oldColor.blue + (int)(blueDiff * offset);

    setStripColor(tween);
    delay(20);
  }
}

void loop() {
  // Delay up front. The RGB strip can start in a random state. If in a posistion to cause feedback, this should avoid it.
  //
  delay(5000);

  color newColor = getColor();
  color colorGamma = { gammatable[newColor.red], gammatable[newColor.green], gammatable[newColor.blue] };

  // Show gamma on history strip
  pushColorHistory(strip.Color(colorGamma.red, colorGamma.green, colorGamma.blue));

  // Fade to new color
  fadeStripColor(stripColor, newColor);
  // fadeStripColor(stripColor, colorGamma);

  // Set color directly
  // setStripColor(newColor);

  stripColor = newColor;
  //  stripColor = colorGamma;

}

