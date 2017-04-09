#include "wire_color.h"

bool hasWireData = false;

color_s color = offColor;

namespace WireColor {
  
  void setup() {
    Wire.begin(WIRE_ADDR_ME);
    Wire.onReceive(receiveMessage);
  }
  
  void receiveColor() {
    if (Wire.available()) color.red = Wire.read();
    if (Wire.available()) color.green = Wire.read();
    if (Wire.available()) color.blue = Wire.read();
    hasWireData = true;
  }

  void receiveBrightness() {
    if (Wire.available()) {
      LEDStrip::setBrightness(Wire.read());
    }
    // Copy the current color so the brightness setting has something to use.
    color = LEDStrip::getColor();
    hasWireData = true;
  }
  
  void receiveMessage(int bytes) {
    // Three bytes for colors
    if (bytes == 3) {
      receiveColor();
    }
  }

  void sendColor() {
    color_s outColor = LEDStrip::getColor();
    Wire.beginTransmission(WIRE_ADDR_HOST);
    Wire.write(outColor.red);
    Wire.write(outColor.green);
    Wire.write(outColor.blue);
    Wire.endTransmission();
  }

  color_s getColor() {
    return color;
  }
  
  void setHasData(bool has) {
    hasWireData = has;
  }
  
  bool hasData() {
    return hasWireData;
  }

}
