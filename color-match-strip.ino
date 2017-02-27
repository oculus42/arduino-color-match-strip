#include <Wire.h>
#include "RGBdriver.h"
#include "Adafruit_TCS34725.h"
#include <IRLibRecvPCI.h>
#include <IRLibDecodeBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLibCombo.h>

// Structs and data for passing around colors
struct color_s {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct irColor_s {
  uint32_t key;
  color_s color;
};

irColor_s remoteColors [20] = {
  {0xFF1AE5, {255, 0, 0} }, // Red
  {0xFF2AD5, {255, 63, 0} }, // Red-Orange (Red 2)
  {0xFF0AF5, {255, 127, 0} }, // Orange (Red 3)
  {0xFF38C7, {255, 191, 0} }, // Yellow-Orange (Red 4)
  {0xFF18E7, {255, 255, 0} }, // Yellow (Red 5)
  {0xFF9A65, {0, 255, 0} }, // Green
  {0xFFAA55, {0, 255, 63} }, // Green-Aqua (Green 2)
  {0xFF8A75, {0, 255, 127} }, // Aqua (Green 3)
  {0xFFB847, {0, 255, 191} }, // Aqua-Cyan (Green 4)
  {0xFF9867, {0, 255, 255} }, // Cyan (Green 5)
  {0xFFA25D, {0, 0, 255} }, // Blue
  {0xFF926D, {63, 0, 255} }, // Blue-Purple (Blue 2)
  {0xFFB24D, {127, 0, 255} }, // Purple (Blue 3)
  {0xFF7887, {191, 0, 255} }, // Purple-Magenta (Blue 4)
  {0xFF58A7, {255, 0, 255} }, // Magenta (Blue 5) 
  {0xFF22DD, {255, 255, 255} }, // White
  {0xFF12ED, {255, 191, 191} }, // White 2
  {0xFF32CD, {255, 191, 191} }, // White 3
  {0xFFF807, {191, 191, 255} }, // White 4
  {0xFFD827, {191, 191, 255} } // White 5
};


// RGB Driver
#define RGB_CLK A3        
#define RGB_DATA A2

RGBdriver Driver(RGB_CLK, RGB_DATA);

// RGB Sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// IR Recevier
#define IR_PIN 3

#define RECV_BLINK_LEN 10

IRrecvPCI myReceiver(IR_PIN); //pin number for the receiver
IRdecode myDecoder;

// Wire Setup
#define WIRE_ADDR_ME 9
#define WIRE_ADDR_HOST 8

// ******** Globals

bool poweredOn = true;
bool getSensorData = true;
bool hasWireData = false;

uint8_t brightness = 127;

const color_s offColor PROGMEM = color_s {0, 0, 0};
color_s currentColor = offColor;
color_s nextColor = offColor;
color_s storedColor;
color_s wireColor;

// Get Color from RGB Sensor
uint16_t clear, red, green, blue;

uint32_t sum;
float r, g, b, offset;
int16_t r1, g1, b1;

uint8_t i, len;
uint32_t currentInput;

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

// LED Strip can start in unexpected state so we must turn it off.
void setupLEDStrip() {
  // Turn off strip
  Driver.begin();
  Driver.SetColor(0, 0, 0);
  Driver.end();
}

void setupIR() {
  myReceiver.enableIRIn();
}

void setupWire() {
  Wire.begin(WIRE_ADDR_ME);
  Wire.onReceive(receiveMessage);
}

void setup() {
  setupWarningLight();
  setupLEDStrip();
  setupRGBSensor();
  setupIR();
  setupWire();
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
    for(i = 1; i < times; ++i) {
      delay(delayLength);
      blinkLED(delayLength);
    }
  }
}

void setNextColorFromRemoteInput() {
  for (i = 0; i < 20; ++i) {
    if (remoteColors[i].key == currentInput) {
      nextColor = remoteColors[i].color;
      return;
    }
  }
  nextColor = offColor;
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

  nextColor.red = byte(r);
  nextColor.green = byte(g);
  nextColor.blue = byte(b);
}

void setStripColor() {
  offset = (float)brightness / 255.0;
  Driver.begin(); // begin
  Driver.SetColor(
    (uint8_t)(currentColor.red * offset),
    (uint8_t)(currentColor.green * offset),
    (uint8_t)(currentColor.blue * offset)
  );
  Driver.end();
}

void fadeStripColor() {
  color_s oldColor = currentColor;
  
  r1 = (nextColor.red - currentColor.red);
  g1 = (nextColor.green - currentColor.green);
  b1 = (nextColor.blue - currentColor.blue);

  len = max(abs(r1), max(abs(g1), abs(b1)));

  for (i = 1; i < len; ++i) {
    currentColor.red = oldColor.red + (1.0 * r1 * i) / len;
    currentColor.green = oldColor.green + (1.0 * g1 * i) / len;
    currentColor.blue = oldColor.blue + (1.0 * b1 * i) / len;

    setStripColor();
    delay(5);
  }

  currentColor = nextColor;
  setStripColor();
}

void pushNewColor() {  
  // Fade to new color
  fadeStripColor();
  // Update the data;
}

void pushNewColor(bool immediate) {
  if (immediate) {
    currentColor = nextColor;
    setStripColor();
    return;
  }
  fadeStripColor();
}

void getActionFromIR() {
  if (currentInput == 0xFFFFFFFF) {
    // Repeated key. Ignore for now
    return;
  }

  if (!poweredOn && currentInput != 0xFF02FD) {
    // Only accept Power Button if it is off
    
  } else if (currentInput == 0xFF827D) {
    // Start/Stop listening to RGB sensor
    // This lets us keep a current RGB setting
    getSensorData = !getSensorData;
    
  } else if (currentInput == 0xFF02FD) {
    // On/Off button
    poweredOn = !poweredOn;
    
    if (poweredOn) {
      nextColor = storedColor;
      fadeStripColor();
    } else {
      storedColor = currentColor;
      nextColor = offColor;
      fadeStripColor();
    }
  } else if (currentInput == 0xFF3AC5) {
    // Brighter
    if (brightness == 0) {
      brightness == 7;
      setStripColor();
    } else if (brightness <= 247) {
      brightness += 8;
      setStripColor();
    }
  } else if (currentInput == 0xFFBA45) {
    // Darker
    if (brightness >= 7) {
      brightness -= 8;
      setStripColor();
    } else if (brightness != 0) {
      brightness = 0;
      setStripColor();
    }
  } else {
    // No more special cases
    setNextColorFromRemoteInput();

    // If any color was specified
    if (nextColor.red != 0 || nextColor.green != 0 || nextColor.blue != 0 ) {
      // Stop checking for a new color
      getSensorData = false; 
      pushNewColor();
    }
  }

  // Reset the currentInput so we don't reuse it.
  currentInput = 0xFFFFFFFF;
}

void receiveColor() {
  if (Wire.available()) wireColor.red = Wire.read();
  if (Wire.available()) wireColor.green = Wire.read();
  if (Wire.available()) wireColor.blue = Wire.read();
  hasWireData = true;
}

void receiveBrightness() {
  if (Wire.available()) brightness = Wire.read();
  // Copy the current color so the brightness setting has something to use.
  wireColor = currentColor;
  hasWireData = true;
}

void receiveMessage(int bytes) {
  // Three bytes for colors
  if (bytes == 3) {
    receiveColor();
  }
}

void sendColor() {
  Wire.beginTransmission(WIRE_ADDR_HOST);
  Wire.write(currentColor.red);
  Wire.write(currentColor.green);
  Wire.write(currentColor.blue);
  Wire.endTransmission();
}

void loop() {
  // Delay up front. The RGB strip starts in a random state.
  // If in a position to cause feedback, this should avoid it.
  delay(200);

  if (hasWireData) {
    
    getSensorData = false;
    poweredOn = true;
    hasWireData = false;
    nextColor = wireColor;
    pushNewColor();
    
  } else if (myReceiver.getResults()) {
        
    myDecoder.decode();
    currentInput = myDecoder.value; 

    // Indicate we received IR input
    blinkLED(20);
    
    getActionFromIR();
    
    //Restart receiver
    myReceiver.enableIRIn();
    
  } else if (poweredOn && getSensorData) {
    getNextColorFromSensor();
    pushNewColor();
  }
}
