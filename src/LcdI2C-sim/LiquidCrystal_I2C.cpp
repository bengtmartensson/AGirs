// Dummy version of LiquidCrystal_I2C.c for generic C++.
// Copyright Bengt Martensson 2015.
// License: GPL3.

#ifdef ARDUINO
#error This file is not for the Arduino.
#endif

#include "LiquidCrystal_I2C.h"
#include <Arduino.h>

LiquidCrystal_I2C::LiquidCrystal_I2C(unsigned char address, unsigned char width, unsigned char height, unsigned char charsize) {
    std::cout << "LCD constructed" << std::endl;
}

void LiquidCrystal_I2C::begin() {
    std::cout << "LCD.begin()" << std::endl;
}

void LiquidCrystal_I2C::clear() {
    std::cout << "LCD.clear()" << std::endl;
}

void LiquidCrystal_I2C::display() {
    std::cout << "LCD.display()" << std::endl;
}

void LiquidCrystal_I2C::noDisplay() {
    std::cout << "LCD.noDisplay()" << std::endl;
}

void LiquidCrystal_I2C::setCursor(uint8_t x, uint8_t y) {
    std::cout << "LCD.setCursor(" << (int) x << ", " << (int) y << ")" << std::endl;
}

void LiquidCrystal_I2C::backlight() {
    std::cout << "LCD.backlight()" << std::endl;
}

void LiquidCrystal_I2C::noBacklight() {
    std::cout << "LCD.noBacklight()" << std::endl;
}

void LiquidCrystal_I2C::print(const String& str) {
    std::cout << "LCD,print(\"" << str << "\")"<< std::endl;
};

size_t LiquidCrystal_I2C::write(uint8_t) {
    return 0;
}
