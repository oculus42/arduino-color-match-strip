#include "ir_lib.h"
#include "color_set.h"

IRrecvPCI myReceiver(IR_PIN); //pin number for the receiver
IRdecode myDecoder;

color_s storedColor;

namespace IR {
  uint32_t currentInput;
  
  void setup() {
    myReceiver.enableIRIn();
  }

  color_s setNextColorFromRemote() {
    for (uint8_t i = 0; i < 20; ++i) {
      if (remoteColors[i].key == currentInput) {
        return remoteColors[i].color;
      }
    }
    return offColor;
  }
  
  void getAction() {
    if (currentInput == 0xFFFFFFFF) {
      // Repeated key. Ignore for now
      return;
    }
  
    if (!poweredOn && currentInput != 0xFF02FD) {
      // Only accept Power Button if it is off
      
    } else if (currentInput == 0xFF827D) {
      // Start/Stop listening to RGB sensor
      // This lets us keep a current RGB setting
      getColorSensorData = !getColorSensorData;
      
    } else if (currentInput == 0xFF02FD) {
      // On/Off button
      poweredOn = !poweredOn;
      
      if (poweredOn) {
        nextColor = storedColor;
        LEDStrip::fadeColor();
      } else {
        storedColor = LEDStrip::getColor();
        nextColor = offColor;
        LEDStrip::fadeColor();
      }
    } else if (currentInput == 0xFF3AC5) {
      LEDStrip::increaseBrightness();
    } else if (currentInput == 0xFFBA45) {
      LEDStrip::decreaseBrightness();
    } else {
      // No more special cases
      color_s next = setNextColorFromRemote();
  
      // If any color was specified
      if (next.red != 0 || next.green != 0 || next.blue != 0 ) {
        // Stop checking for a new color
        getColorSensorData = false; 
        LEDStrip::setColor(next);
      }
    }
  
    // Reset the currentInput so we don't reuse it.
    currentInput = 0xFFFFFFFF;
  }

  bool hasData() {
    return myReceiver.getResults() ? true : false;
  }
  
  void processData() {
    myDecoder.decode();
    currentInput = myDecoder.value;

    // TODO: Get Action from IR
    getAction();
    
    //Restart receiver
    myReceiver.enableIRIn();
  }
  
}
