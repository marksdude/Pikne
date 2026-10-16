#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>

void display_initialize();
uint8_t menu_control(uint8_t buttonevent);
uint8_t menu_button(uint8_t pin);
void menu_draw(uint8_t menu, float pack_voltage, uint8_t level, float battery_temp, float battery_current, float output_voltage, float output_current, uint8_t bq25792_status, uint16_t time_to_empty);


#endif