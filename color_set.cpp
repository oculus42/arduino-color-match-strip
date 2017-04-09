#include "color_set.h"

float offset;
int16_t r1, g1, b1;
uint8_t i, len;

color_s currentColor = offColor;
uint8_t brightness = 127;

namespace LEDStrip {

  RGBdriver Driver(RGB_CLK, RGB_DATA);

  color_s nextColor = offColor;
  color_s storedColor;

  // LED Strip can start in unexpected state so we must turn it off.
  void setup() {
    // Turn off strip
    Driver.begin();
    Driver.SetColor(0, 0, 0);
    Driver.end();
  }

  void implement() {
    offset = (float)brightness / 255.0;
    Driver.begin(); // begin
    Driver.SetColor(
      (uint8_t)(currentColor.red * offset),
      (uint8_t)(currentColor.green * offset),
      (uint8_t)(currentColor.blue * offset)
    );
    Driver.end();
  }

  void increaseBrightness() {
    if (brightness == 0) {
      brightness == 7;
      implement();
    } else if (brightness <= 247) {
      brightness += 8;
      implement();
    }
  }

  void decreaseBrightness() {
    if (brightness >= 7) {
      brightness -= 8;
      implement();
    } else if (brightness != 0) {
      brightness = 0;
      implement();
    }
  }

  void setBrightness(uint8_t inBright) {
    brightness = inBright;
  }
  
  void fadeColor() {
    color_s oldColor = currentColor;
    
    r1 = (nextColor.red - currentColor.red);
    g1 = (nextColor.green - currentColor.green);
    b1 = (nextColor.blue - currentColor.blue);
  
    len = max(abs(r1), max(abs(g1), abs(b1)));
  
    for (i = 1; i < len; ++i) {
      currentColor.red = oldColor.red + (1.0 * r1 * i) / len;
      currentColor.green = oldColor.green + (1.0 * g1 * i) / len;
      currentColor.blue = oldColor.blue + (1.0 * b1 * i) / len;
  
      implement();
      delay(5);
    }
  
    currentColor = nextColor;
    implement();
  }
  
  void setColor() {  
    // Fade to new color
    fadeColor();
    // Update the data;
  }
  
  void setColor(bool immediate) {
    if (immediate) {
      currentColor = nextColor;
      implement();
      return;
    }
    fadeColor();
  }

  void setColor(color_s next) {
    nextColor = next;
    setColor();
  }

  color_s getColor() {
    return currentColor;
  }
  
}

