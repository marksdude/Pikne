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

![LED controller.png](https://github.com/marksdude/Pikne/blob/main/Images/LED%20controller.png)
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
An ATmega328P acts as the main controller. It monitors everything, handles faults, displays battery information and communicates with the modules.
<p float="left">
  <img src="/Images/Schematic.png" width="400" />
  <img src="/Images/Main PCB.png" width="400" /> 
</p>
The BMS is based on the BQ41Z50, a 2-4S battery manager IC. It has all the necessary protections needed for a lithium-ion battery pack and monitors its SoC (State of Charge). The ATmega328P requests battery information from the BMS over SMBus.

<p float="left">
  <img src="/Images/BMS schematic.png" width="400" />
  <img src="/Images/BMS.png" width="400" /> 
</p>


Modules can communicate with the power bank over UART through the pogo pins above the USB-C port. A connected module is idendified with an ID number sent by the module (1 for LED controller, 2 for Flashlight). The module can then request battery information or use the button on the power bank for additional control.
### LED controller
The LED controller is based on an ESP32-C3-WROOM-02U that is running ESPHome. There are 3 external MOSFETs that are used for the two monochrome outputs and one RGB output. An RGB output needs three MOSFETs, one for each color channel. Since there are only three, the monochrome outputs have to be connected together with two of the RGB channels. This is why the monochrome and RGB outputs can't be used simultaneously. INA219s are used for monitoring power draw of each output.
<p float="left">
  <img src="/Images/Led controller schematic.png" width="400" />
  <img src="/Images/Led controller.png" width="400" /> 
</p>

# Wiring
All of the smaller connectors are JST-ZH, while the battery connectors are JST-VH.  
The main PCB has 4 connectors. One goes to the pogo connector, two to the BMS and one is for programming.  

![Main PCb connectors.png](https://github.com/marksdude/Pikne/blob/main/Images/Main%20PCB%20connectors.png)

On the BMS two connectors go to the main PCB and two to the battery pack.  

![BMS connectors.png](https://github.com/marksdude/Pikne/blob/main/Images/BMS%20connectors.png)

The battery is connected in a 2S2P configuration. There are cutouts in the bottom of the battery holder for routing the wires and for the thermistors. BAT+ and BAT- connect to the BMS input, thermistors and the balance lead connect to the BMS battery sense connector.

<p float="left">
  <img src="/Images/battery top.png" width="400" />
  <img src="/Images/battery bottom.png" width="400" /> 
</p>

The button has to be soldered to the main PCB. The pogo connector is connected to the main PCB.

<p float="left">
  <img src="/Images/Button wiring.png" width="500" />
  <img src="/Images/Pogo.png" width="300" /> 
</p>

### LED controller
Not much wiring to do here just connect the pogo connector to the PCB.

<p float="left">
  <img src="/Images/LED controller connectors.png" width="450" />
  <img src="/Images/LED controller pogo.png" width="350" /> 
</p>

# Firmware
Firmware is currently very limited. Version 0.1 has a basic menu to check some battery information, that's about it. It relies on the [ArduinoSMBus](https://github.com/osh-labs/ArduinoSMBus) library for communicating with the BMS and the [Adafruit_SSD1306](https://github.com/adafruit/adafruit_ssd1306) and [Adafruit GFX Library](https://github.com/adafruit/adafruit-gfx-library) libraries for drawing on the OLED. I plan to move away from these libraries in the future.  
  
There is still a lot left to be implemented.

# Why did I make this?
It all started because I was tired of changing the AA batteries in my mom's battery-powered lights. I wanted to make the process faster and less wasteful, so I thought it would be a good idea to design an easily removable, rechargeable battery pack for the lights and also integrate them with Home Assistant. The original idea was going to have 2 parts the battery pack and a controller board. They were supposed to be held together with magnets, connected with pogo pins and communicate over UART. Then I thought, why not turn the battery pack into a power bank so it could be used for things other than the lights? That's what I did. The idea of making the battery pack easily removable led me to make the power bank modular. That's how Pikne was born.
