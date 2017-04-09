#include "settings.h"

#include "RGBdriver.h"

#ifndef COLOR_SET_h
#define COLOR_SET_h

namespace LEDStrip {

  void setup(void);
  void implement(void);
  
  void fadeColor(void);
  void setColor(void);
  void setColor(bool immediate);
  void setColor(color_s next);
  
  color_s getColor(void);

  void increaseBrightness(void);
  void decreaseBrightness(void);
  void setBrightness(uint8_t inBright);

}

#endif
