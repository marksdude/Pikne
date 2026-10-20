#include <Arduino.h>
#include <wire.h>
#include <ArduinoSMBus.h>
#include "display.h"



#define BQ25792_STAT_PIN 20
#define BQ25792_INT_PIN 7

#define TPS259461_FLT_PIN 8
#define TPS259461_SPLYGD_PIN 10
#define TPS259461_ILM_PIN A2
#define TPS259461_EN_PIN 6

#define BMS_ALERT_PIN 17

#define BUTTON_PIN 2

ArduinoSMBus battery(0x16);

uint8_t button_value;
uint8_t last_button_value = 0;
uint8_t menu;
uint8_t last_menu;
long menu_draw_delay = millis();
float battery_temp;



void setup() {
  pinMode(0, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BQ25792_STAT_PIN, INPUT);
  pinMode(BQ25792_INT_PIN, INPUT);
  pinMode(TPS259461_FLT_PIN, INPUT);
  pinMode(TPS259461_SPLYGD_PIN, INPUT);
  pinMode(TPS259461_ILM_PIN, INPUT);
  pinMode(TPS259461_EN_PIN, INPUT);
  pinMode(BMS_ALERT_PIN, INPUT);
  analogReference(INTERNAL);
  display_initialize();
  digitalWrite(TPS259461_EN_PIN, HIGH);
}

uint8_t bq25792_status(){
  static uint8_t status;
  static bool stat_value;
  static bool last_stat_value;
  static long blink_time;
  static int blink_count = 0;
  stat_value = digitalRead(BQ25792_STAT_PIN);
  if(stat_value != last_stat_value){
    blink_time = millis();
    blink_count++;
    if(blink_count > 2){
      status = 3;
      blink_count = 0;
    }
  }
  if(millis() - blink_time > 1050){
    blink_count = 0;
    if(stat_value == HIGH && last_stat_value == HIGH){
      status = 1;
  }
    else if(stat_value == LOW && last_stat_value == LOW){
      status = 2;
  }
  }

  last_stat_value = stat_value;
  return status;
}

float usb_current(){
  float voltage = 1.1/1024*analogRead(TPS259461_ILM_PIN);
  float current = (voltage*1000000.0f)/182000.0f;
  return current;
}

void loop() {
  button_value = menu_button(BUTTON_PIN);
  menu = menu_control(button_value);
  if(button_value != last_button_value || menu == 1 || 2 || 3){  
    if(millis() - menu_draw_delay > 200 || menu != last_menu){
      battery_temp = battery.temperatureC()/10;
      menu_draw(menu_control(button_value), battery.voltage(), battery.relativeStateOfCharge(), battery_temp, battery.current()/1000, 0, usb_current(), bq25792_status(), battery.runtimetoempty());
      menu_draw_delay = millis();
    }
    last_button_value = button_value;
    last_menu = menu;
  }
}
