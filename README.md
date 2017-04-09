# Arduino Color Match Strip
Control an RGB LED Strip with an RGB light sensor, and other random features.

This originated with a goal of matching natural light color from a window to a light strip in a windowless hallway/stairway.

## Features
* Detect color and set the RGB strip to match
* Maximum brightness limit
* Fade between colors to prevent sudden brightness/color changes
* Support for the IR Remote:
 * Provide On/Off function
 * Allow colors to be set and held until further input
 * Resume using sensor input
 * Adjust brightness
* I<sup>2</sup>C Interface to:
 * Accept colors from another controller

## Project Components
I'm using the following components with this project:
* [Adafruit Trinket Pro 5V](https://www.adafruit.com/product/2000)
* [Adafruit TCS34725 RGB Color Sensor](https://www.adafruit.com/products/1334)
* [Next LED Strip Driver Deep Blue ARD0689](https://www.amazon.com/gp/product/B00ORMR22G/)
* [LED Wholesaler's RGB LED Strip](https://www.amazon.com/dp/B0040FJ27S/)

## Plans
* Code Cleanup
* Update IR Support to:
 * Adjust sensor check frequency
 * Save colors
 * Add flashing and fading functions
* Update I<sup>2</sup>C Interface to:
 * Send colors to another controller
 * Send on/off state to another controller
 * Receive brightness changes
 * Support immediate color change rather than triggering the fade logic
* And PIR sensor to support motion activated "low light" when in "Off" mode
* Add a Fritzing diagram - WIP
* Add a second sensor to go with the LED strip.
 * Adjust output based on the variation between the sensors

