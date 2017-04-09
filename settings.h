#include "color_data.h"

#ifndef _SETTINGS_h
#define _SETTINGS_h

// Wire Setup
#define WIRE_ADDR_ME 9
#define WIRE_ADDR_HOST 8

// Color Set
#define RGB_CLK A3        
#define RGB_DATA A2

// IR Recevier
#define IR_PIN 3
#define RECV_BLINK_LEN 10

extern bool getColorSensorData;
extern bool poweredOn;
extern color_s nextColor;

#endif
