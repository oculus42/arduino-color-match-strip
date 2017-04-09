#include "Wire.h"

#ifndef IRCOLOR_data_h
#define IRCOLOR_data_h

// Structs and data for passing around colors
struct color_s {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

typedef struct color_s color_s;

struct irColor_s {
  uint32_t key;
  color_s color;
};

typedef struct irColor_s irColor_s;

const irColor_s remoteColors [20] PROGMEM = {
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

const color_s offColor PROGMEM = color_s {0, 0, 0};

#endif
