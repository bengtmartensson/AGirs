// Comment out if LCD is not wanted or needed. Except for needing some memory,
// it has otherwise no disadvantages.
// Remove for minimal memory footprint.

// This should be taken from config.h, but the Arduino IDE does not seem
// to allow for this. User friendlyness taken to a new level...
// Adjust if needed
#if ! defined(ARDUINO_AVR_MICRO) & ! defined(ARDUINO_AVR_NANO)
#define LCD
#endif

#include "LedLcdManager.h"
#include <string.h>
#include <avr/pgmspace.h>

#ifdef LCD
LiquidCrystal_I2C *LedLcdManager::lcd;
#endif

unsigned int LedLcdManager::row = 0U;
unsigned int LedLcdManager::column = 0U;

milliseconds_t LedLcdManager::blinkTime = defaultBlinkTime;
unsigned long LedLcdManager::turnOffTime;
unsigned int LedLcdManager::lcdRows = 0;
unsigned int LedLcdManager::lcdColumns = 0;

pin_t LedLcdManager::physicalLeds[maxLeds];
led_t LedLcdManager::logicalLeds[maxLeds];
bool LedLcdManager::shouldTimeOut[maxLeds];

void LedLcdManager::setup(int8_t i2cAddress, uint8_t columns, uint8_t rows,
        const pin_t physicalLeds_[], const led_t logicalLeds_[], const bool shouldTimeOut_[]) {
    setupLcdI2c(i2cAddress, columns, rows);
    setupPhysicalLeds(physicalLeds_);
    setupLogicLeds(logicalLeds_);
    setupShouldTimeOut(shouldTimeOut_);
    disableTurnOffTime();
}

bool LedLcdManager::setPhysicalLed(led_t physicalLed, LedState state) {
    pin_t pin = physicalLeds[physicalLed-1];
    if (pin == invalidPin)
        return false;

    digitalWrite(pin, state == off ? LOW : HIGH);
    if (state == blink)
        updateTurnOffTime();
    return true;
}

LedLcdManager::LedState LedLcdManager::onOffBlinkParse(const char *value) {
        return value == NULL ? invalid
                : strcmp(value, "on") == 0 ? on
                : strcmp(value, "off") == 0 ? off
                : strcmp(value, "blink") == 0 ? blink
                : invalid;
}

bool LedLcdManager::setLogicLed(led_t logicLed, LedState state) {
    if (logicLed == invalidLed
            || logicLed <  1
            || logicLed > maxLeds
            || state == invalid )
        return false;

    led_t physicalLed = logicalLeds[logicLed - 1];
    if (physicalLed == invalidLed)
        return false;

    return setPhysicalLed(physicalLed, state);
}

bool LedLcdManager::setupLogicLed(led_t logicLed, led_t physicalLed) {
    if (physicalLed == invalidLed)
        return false;

    logicalLeds[logicLed-1] = physicalLed;
    return true;
}

bool LedLcdManager::setupLogicLeds(const led_t logicalLeds_[maxLeds]) {
    for (int i = 0; i < maxLeds; i++)
        logicalLeds[i] = logicalLeds_ == NULL ? i+1 : logicalLeds_[i];
    return true;
}

void LedLcdManager::setupPhysicalLeds(const pin_t physicalLeds_[maxLeds]) {
    for (int i = 0; i < maxLeds; i++) {
        physicalLeds[i] = physicalLeds_ == NULL ? invalidPin : physicalLeds_[i];
        if (physicalLeds[i] != invalidPin)
            pinMode(physicalLeds[i], OUTPUT);
    }
}

void LedLcdManager::setupShouldTimeOut(const bool shouldTimeOut_[maxLeds]) {
    for (int i = 0; i < maxLeds; i++)
        shouldTimeOut[i] = shouldTimeOut_ == NULL ? true : shouldTimeOut_[i];
}

void LedLcdManager::setupShouldTimeout(led_t logicLed, bool state) {
    if (logicLed != invalidLed)
        shouldTimeOut[logicLed-1] = state;
}

void LedLcdManager::setupLcdI2c(int8_t i2cAddress __attribute__((unused)),
        uint8_t columns __attribute__((unused)),
        uint8_t rows __attribute__((unused))) {
#ifdef LCD
    lcd = i2cAddress >= 0 ? new LiquidCrystal_I2C(static_cast<uint8_t>(i2cAddress), columns, rows) : nullptr;
    if (lcd) {
        lcdRows = rows;
        lcdColumns = columns;
        lcd->init();
    }
#endif
}

void LedLcdManager::updateTurnOffTime() {
    turnOffTime = millis() + blinkTime;
}

void LedLcdManager::selfTest(const char *text) {
    lcdPrint(text);
    for (led_t i = 1; i <= maxLeds; i++)
        setLogicLed(i, on);
    delay(
#ifdef LCD
            lcd ? selftestTimeWithLCD :
#endif
                selftestTimeWithoutLCD);
    allOff(true);
}

void LedLcdManager::selfTest(const __FlashStringHelper *text) {
    lcdPrint(text);
    for (led_t i = 1; i <= maxLeds; i++)
        setLogicLed(i, on);
    delay(
#ifdef LCD
    lcd ? selftestTimeWithLCD :
#endif
        selftestTimeWithoutLCD);
    allOff(true);
}

void LedLcdManager::checkTurnoff() {
    if (millis() > turnOffTime)
        allOff(false);
}

void LedLcdManager::allOff(bool force) {
#ifdef LCD
    if (lcd) {
        lcd->noDisplay();
        lcd->noBacklight();
    }
#endif
    for (led_t i = 1; i <= maxLeds; i++)
        if (force || shouldTimeOut[i - 1])
            setLogicLed(i, off);

    disableTurnOffTime();
}

void LedLcdManager::disableTurnOffTime() {
    turnOffTime = (unsigned long) -1;
}

void LedLcdManager::lcdPrint(const __FlashStringHelper *pstr __attribute__ ((unused)), bool clear __attribute__ ((unused)), int x __attribute__ ((unused)), int y __attribute__ ((unused))) {
#ifdef LCD
    if (!lcd)
        return;
    prepare(clear, x, y);
    PGM_P p = reinterpret_cast<PGM_P>(pstr);
    char c;
    do {
        c = pgm_read_byte(p++);
        switch (c) {
            case '\r':
            case '\0':
                break;
            case '\n':
                column = 0;
                row++;
                lcd->setCursor(column, row);
                break;
            default:
                lcd->write(c);
        }
    } while (c != '\0');

    lcd->display();
    lcd->backlight();
    updateTurnOffTime();
#endif
}

void LedLcdManager::lcdPrint(const char* string __attribute__ ((unused)), bool clear __attribute__ ((unused)),
        int x __attribute ((unused)), int y __attribute__ ((unused))) {
#ifdef LCD
    if (!lcd)
        return;
    prepare(clear, x, y);
    size_t len = strlen(string);
    for (size_t i = 0; i < len; i++) {
        char c = string[i];
        switch (c) {
            case '\r':
                break;
            case '\n':
                column = 0;
                row++;
                lcd->setCursor(column, row);
                break;
            default:
                lcd->write(c);
        }
    }
    lcd->display();
    lcd->backlight();
    updateTurnOffTime();
#endif
}

void LedLcdManager::prepare(bool clear __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {
#ifdef LCD
    if (x >= 0 && y >= 0) {
        row = y;
        column = x;
        if (row > (unsigned int) lcdRows - 1 || column > (unsigned int) lcdColumns - 1) // outside of display
            return;
        lcd->setCursor(column, row);
    }

    if (clear) {
        row = 0U;
        column = 0U;
        lcd->clear();
    }
#endif
}