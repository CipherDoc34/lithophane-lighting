## Lithophane Lighting
A custom embedded (esp8266) software program to control RGB leds to light up 3d printed lithophane.
Implements a custom library with a class to represent a single RGB led to have addressable control over multiple LEDs.

### rgbled.hpp / rgbled.cpp
A class that represents a single LED holding the relevant attributes of a single RGB LED (common anode or common cathode) or 3 leds (red, green, and blue)
Instantiated by calling the RgbLed constructor, providing the pin number to the led and the led type (common anode or common cathode)

### addressable_leds.hpp / addressable_leds.cpp
A class that represents 2 RgbLeds to represent the top and bottom LED.
Implements the Static colour, Pulse or Breathing, and Cycle colour modes.
These colour modes are updated in a non-blocking fashion to allow for other functions to execute in a single threaded environment.

## V1
Connects to a wifi network with a static SSID and Password that is set constantly at the compile time. 
Renders a static html webpage at http://\<esp8266 ip address\>, to control LED colours and modes.

## Next: V2
1. Create an android app to facilitate setup to allow for easy portability, when roaming different WiFi networks.
2. Save LED settings to non-volitile storage for continuation after power interrupt.
