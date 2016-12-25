# Arduino Color Match Strip
Control an RGB LED Strip with an RGB light sensor, and other random features.

This originated with a goal of matching natural light color from a window to a light strip in a windowless hallway/stairway.

## Features
* Detect color and set the RGB strip to match
* Maximum brightness limit
* Optional gamma table to adjust color output
* Fade between colors to prevent sudden brightness/color changes
* Color history readout on some NeoPixels, mostly for testing

## Project Components
I'm using the following components with this project:
* [Adafruit Trinket Pro 5V](https://www.adafruit.com/product/2000)
* [Adafruit TCS34725 RGB Color Sensor](https://www.adafruit.com/products/1334)
* [Next LED Strip Driver Deep Blue ARD0689](https://www.amazon.com/gp/product/B00ORMR22G/)
* [LED Wholesaler's RGB LED Strip](https://www.amazon.com/dp/B0040FJ27S/)
* [Adafruit NeoPixel RGB LED Strip](https://www.adafruit.com/products/1376)

## Plans
* Add a second sensor to go with the LED strip.
 * Adjust output based on the variation between the sensors
* Add IR to use the LED Strip remote to:
 * Provide On/Off function
 * Allow colors to be set and held until further input
 * Resume using sensor input
 * Adjust sensor check frequency
 * Provide On/Off for NeoPixels history
 * Save colors
* Add a Fritzing diagram

