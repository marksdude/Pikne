# Pikne
A modular and repairable power bank
  
![Example.jpg](https://github.com/marksdude/Pikne/blob/main/Images/Example.jpg)
# Features:
- USB PD output up to 60W
- 0.91 inch OLED for status monitoring
- Attachable modules
- Repairable design
- 80WH capacity
- Based on the BQ25792 and TPS25751
- One USB-C output
# Modules:
- LED controller - Allows you to control LEDs with Home Assistant.  
  - Features:
    - Up to 2.5A output
    - 5V output
    - Home Assistant integration
    - Power monitoring on each output
    - 2 monochrome outputs, 1 RGB output and 1 addressable LED output 
      - **Note:** RGB and the monochrome outputs can't be used simultaneously.
    - ESP32-C3 based
- Flashlight
  - Features:
    - 6600lm max brightness
      - **Note:** The LEDs probably won't be able to run at max brightness for very long.
    - PWM dimmable
    - STM32L011F3 based
# Coming modules
- LiPo charger - Allows you to charge up to 4S LiPos from your power bank
