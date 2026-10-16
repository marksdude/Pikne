#include "display.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1, 400000, 100000);

void display_initialize(){
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0, false, false);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
}


uint8_t menu_control(uint8_t buttonevent){
  static uint8_t menu;
  static bool menu_forward_ok;
  static long menu_timeout_time;
  static bool advanced_menu;
  static bool back_ok;
  static bool advanced_menu_ok;
  if(buttonevent == 2 && advanced_menu == false){
    menu = 1;
    menu_timeout_time = millis();
  }
  else if(buttonevent == 1 && advanced_menu != true && advanced_menu_ok == true){
    advanced_menu = true;
    back_ok = false;
    menu = 2;
    menu_timeout_time = millis();
  }
  else if(buttonevent == 0 && advanced_menu_ok == false && advanced_menu == false){
    advanced_menu_ok = true;
  }
  else if(advanced_menu == true){
    if(buttonevent == 2 && menu_forward_ok == true){
      menu++;
      menu_forward_ok = false;
    }
    else if(buttonevent == 0){
      menu_forward_ok = true;
      if(back_ok == false){
        back_ok = true;
      }
    }
    else if(buttonevent == 1 && back_ok == true){
      menu_timeout_time = millis();
      advanced_menu = false;
      advanced_menu_ok = false;
      menu = 1;
    }
    if(menu > 3){
      menu = 2;
    }
  }
  if((menu == 1 || menu == 2 || menu == 3 || menu == 4) && millis()-menu_timeout_time >= 10000 && advanced_menu != true){
    menu = 0;
      }
  return menu;
}

uint8_t menu_button(uint8_t pin){
  static bool menu_button_value;
  static bool last_menu_button_value;
  static long downtime;
  static long button_release;
  uint8_t buttonevent;
  menu_button_value = digitalRead(pin);
  if(menu_button_value == LOW && last_menu_button_value == HIGH){
    downtime = millis();
    }
  if(menu_button_value == LOW && last_menu_button_value == LOW && millis()-downtime >= 1000){
    buttonevent = 1;
    button_release = millis();
  }
  else if(menu_button_value == LOW && last_menu_button_value == HIGH && millis()-downtime < 1000){
    buttonevent = 2;
    button_release = millis();
  }
  else if(menu_button_value == HIGH && (buttonevent == 1 || buttonevent == 2) && millis()-button_release >= 50){
    buttonevent = 0;
  }
  last_menu_button_value = menu_button_value;
  return buttonevent;
}

void menu_draw(uint8_t menu, float pack_voltage, uint8_t level, float battery_temp, float battery_current, float output_voltage, float output_current, uint8_t bq25792_status, uint16_t time_to_empty){
  display.clearDisplay();
  if(menu == 1){
    display.setTextSize(2);
    if(time_to_empty > 60){
      time_to_empty = time_to_empty/60;
      if(time_to_empty > 99){
        time_to_empty = 99;
      }
      display.setCursor(80, 0);
      display.print(time_to_empty);
      display.print("h+");
    }
    else{
      display.setCursor(66, 0);
      display.print(time_to_empty);
      display.print("min");
    }
    display.setTextSize(1);
    display.setCursor(58, 24);
    display.print("OUT: ");
    display.print(output_current*output_voltage, 1);
    display.print("W");
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print(level);
    display.print("%");
    if(bq25792_status == 2){
      display.setCursor(0, 24);
      display.setTextSize(1);
      display.print("Charging");
    }
    else if(bq25792_status == 3){
      display.setCursor(0, 24);
      display.setTextSize(1);
      display.print("FAULT");
    }

  }
  else if(menu == 2){
    display.setTextSize(1);
    display.setCursor(60, 0);
    display.print("PACK: ");
    display.print(pack_voltage, 1);
    display.print("V");
    display.setCursor(60, 10);
    display.print("TEMP: ");
    display.print(battery_temp, 1);
    display.print("C");
    display.setCursor(0, 22);
    display.setTextSize(1);
    display.print(battery_current);
    display.print("A");
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print(battery_current*pack_voltage, 1);
    display.print("W");
    display.setCursor(86, 23);
    display.setTextSize(1.8);
    display.print("battery");
  }
  else if(menu == 3){
    display.setTextSize(1);
    display.setCursor(40, 22);
    display.print("VOLTAGE: ");
    display.print(output_voltage, 1);
    display.print("V");
    display.setCursor(0, 22);
    display.print(output_current);
    display.print("A");
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print(output_current*output_voltage, 1);
    display.print("W");
    display.setCursor(90, 0);
    display.setTextSize(1);
    display.print("output");
  }
  display.display();
}