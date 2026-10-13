# Pikne
A modular and repairable power bank
  
![Example.jpg](https://github.com/marksdude/Pikne/blob/main/Images/Example.jpg)
Picture of rev 1.0 nonworking build
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
# How does it work?
Core functionality comes from the [TPS25751](https://www.ti.com/product/TPS25751) and [BQ25792](https://download.mikroe.com/documents/datasheets/BQ25792_datasheet.pdf) combination. TPS25751 has integrated I2C control for TI battery chargers, including the BQ25792, which simplified firmware development. The TPS25751 handles USB-C PD negotiations and manages the BQ25792. The BQ25792 charges the battery and generates higher voltages for the USB-C output. So for example, if you connect a device you want to charge, first your device negotiates its needed voltage and current with the TPS25751. Then, the TPS25751 asks the BQ25792 to generate the requested voltage (only if the requested voltage is above 5V, for 5V there is a separate regulator) once the BQ25792 is outputting the right voltage, the TPS25751 starts sourcing (outputting) and your device starts charging.
# Why did I make this?
It all started because I was tired of changing the AA batteries in my mom's battery-powered lights. I wanted to make the process faster and less wasteful, so I thought it would be a good idea to design an easily removable, rechargeable battery pack for the lights and also integrate them with Home Assistant. The original idea was going to have 2 parts the battery pack and a controller board. They were supposed to be held together with magnets, connected with pogo pins and communicate over UART. Then I thought, why not turn the battery pack into a power bank so it could be used for things other than the lights? That's what I did. The idea of making the battery pack easily removable led me to make the power bank modular. That's how Pikne was born.
