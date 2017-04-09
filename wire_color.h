#include "settings.h"
#include "color_set.h"
#include <Wire.h>


#ifndef WIRE_access_h
#define WIRE_access_h

namespace WireColor {
  void setup(void);
  void beginvoid(void);
  void receiveColor(void);
  void receiveBrightness(void);
  void receiveMessage(int bytes);
  void sendColor(color_s outColor);
  color_s getColor(void);
  void sendBrightnews(uint8_t brightness);
  void setHasData(bool has);
  bool hasData(void);
};

#endif
