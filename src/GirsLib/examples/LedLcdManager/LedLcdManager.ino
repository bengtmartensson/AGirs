#include <InfraredTypes.h>

#include <LiquidCrystal_I2C.h>

#define LED
#ifdef ARDUINO
#include <Wire.h>
#else
#define ARDUINO_AVR_MEGA2560
#define LED_BUILTIN 13
#define A1 101
#define A2 102
#define A3 103
#endif
#include <girs_pins_mega2560.h>
#include <lcd_0x27_16_2.h>

#include <LedLcdManager.h>

unsigned long then = millis();
boolean didit = false;

void setup() {
    LedLcdManager::setupLedGroundPins();
    LedLcdManager::setup(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT,
            (const pin_t[]) {SIGNAL_LED_1, SIGNAL_LED_2, SIGNAL_LED_3, SIGNAL_LED_4,
                    SIGNAL_LED_5, SIGNAL_LED_6, SIGNAL_LED_7, SIGNAL_LED_8 }
            ,NULL//,(const led_t[]) {8, 7, 6, 5, 4, 3, 2, 1},
            ,(const boolean[]) {false, true, false, true, false, true, true, false}//true, true, true, true }
    );
    LedLcdManager::selfTest("Welcome to\nLedLcdManager!");
}

void loop() {
    LedLcdManager::checkTurnoff();

    if ((millis() - then > 10000UL) && !didit) {
        didit = true;
        LedLcdManager::setLogicLed(1, LedLcdManager::blink);
        LedLcdManager::setLogicLed(2, LedLcdManager::blink);
        LedLcdManager::setLogicLed(3, LedLcdManager::blink);
        LedLcdManager::setLogicLed(4, LedLcdManager::blink);
        LedLcdManager::setLogicLed(5, LedLcdManager::blink);
        LedLcdManager::setLogicLed(6, LedLcdManager::blink);
        LedLcdManager::setLogicLed(7, LedLcdManager::blink);
        LedLcdManager::setLogicLed(8, LedLcdManager::blink);
        LedLcdManager::lcdPrint("GirsLib", true, 5, 1);
    }
}

#ifndef ARDUINO
int main() {
    setup();
    while (true)
        loop();
}
#endif
