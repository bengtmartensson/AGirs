#pragma once

#include "Tokenizer.h"
#include <InfraredTypes.h>
#include <IrSender.h>
#ifdef LCD
#include <LiquidCrystal_I2C.h> // https://github.com/marcoschwartz/LiquidCrystal_I2C
#endif

typedef uint8_t led_t;

class LedLcdManager {
public:
    static const int maxLeds = 8;
    static const int defaultLcdRows = 2;
    static const int defaultLcdColumns = 16;

    static const int defaultBlinkTime = 2000;
    static const int selftestTimeWithoutLCD = 200;
    static const int selftestTimeWithLCD = 2000;

    enum LedState {
        on,
        off,
        blink,
        invalid
    };


    static const led_t invalidLed = 255;

private:
    /** This array maps physical LEDs to hardware pins. */
    static pin_t physicalLeds[maxLeds];

    /** This array maps logical LEDs to physical LEDs. */
    static led_t logicalLeds[maxLeds];

    /** Indicates which logical leds should time out. */
    static bool shouldTimeOut[maxLeds];

    static milliseconds_t blinkTime;

    /** Time to turn of, in the semantic of millis(). */
    static unsigned long turnOffTime;

#ifdef LCD
    static LiquidCrystal_I2C *lcd;
#endif

    static unsigned int lcdRows;
    static unsigned int lcdColumns;

    //static LedLcdManager instance;

    static void setupPhysicalLeds(const pin_t physicalLeds[maxLeds]);
    static void setupShouldTimeOut(const bool shouldTimeOut[maxLeds]);
    /*pin_t pled1, pin_t pled2 = invalidPin,
            pin_t pled3 = invalidPin, pin_t pled4 = invalidPin,
            pin_t pled5 = invalidPin, pin_t pled6 = invalidPin,
            pin_t pled7 = invalidPin, pin_t pled8 = invalidPin);*/

    static void setupLcdI2c(int8_t i2cAddress, uint8_t columns, uint8_t rows);
    LedLcdManager();

    //void initArray(char *array, uint8_t value);

    static void disableTurnOffTime();

    static void groundPin(pin_t pin) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

public:
    /** Sets up the instance, to be called before using the instance.  */
    static void setup(int8_t i2cAddress = -1, uint8_t columns = defaultLcdColumns, uint8_t rows = defaultLcdRows,
            const pin_t physicalLeds[maxLeds] = NULL,
            const led_t logicalLeds[maxLeds] = NULL,
            const bool shouldTimeOut[maxLeds] = NULL);

    static void setup(int8_t i2cAddress, uint8_t columns, uint8_t rows,
            pin_t pled1, pin_t pled2 = invalidLed,
            pin_t pled3 = invalidPin, pin_t pled4 = invalidPin,
            pin_t pled5 = invalidPin, pin_t pled6 = invalidPin,
            pin_t pled7 = invalidPin, pin_t pled8 = invalidPin);

    void setupPhysicalLeds(pin_t led1 = invalidPin, pin_t led2 = invalidPin,
            pin_t led3 = invalidPin, pin_t led4 = invalidPin,
            pin_t led5 = invalidPin, pin_t led6 = invalidPin,
            pin_t led7 = invalidPin, pin_t led8 = invalidPin);

    void static lcdPrint(String& string, bool clear = true, int x = 0, int y = -1);
    void static lcdPrint(const char *str, bool clear = true, int x = 0, int y = -1) {
        String string(str);
        lcdPrint(string, clear, x, y);
    };
#ifdef ARDUINO
    void static lcdPrint(const __FlashStringHelper *pstr, bool clear = true, int x = 0, int y = -1) {
        String string(pstr);
        lcdPrint(string, clear, x, y);
    }
#endif

    //void static lcdPrint(const String& str);

    //LedLcdManager& getInstance() {
    //    return instance;
    //};

    //static LiquidCrystal_I2C& getLcd() {
    //    return *lcd;
    //}

    static void lcdSetCursor(uint8_t x = 0, uint8_t y = 0) {
#ifdef LCD
        lcd->setCursor(x, y);
#else
        (void) x;
        (void) y;
#endif
    }

    static void allOff(bool force);

    static milliseconds_t getBlinkTime() {
        return blinkTime;
    };

    static void setBlinkTime(milliseconds_t value) {
        blinkTime = value;
    }

    static void updateTurnOffTime();

    /** Turn off if it is due. This has to be called periodically by the user. */
    static void checkTurnoff();

    static void selfTest(const char *text);

#ifdef ARDUINO
    static void selfTest(const __FlashStringHelper *text);
#endif

    static bool setPhysicalLed(led_t physicalLed, LedState state);

    static bool setLogicLed(led_t logicLed, LedState state);

    static bool setLogicLed(led_t logicLed, const char *state) {
        return setLogicLed(logicLed, onOffBlinkParse(state));
    };

    static LedState onOffBlinkParse(const char *value);

    static void setupShouldTimeout(led_t logicLed, bool state);

    static bool setupLogicLed(led_t loginLed, led_t physicalLed);

    static bool setupLogicLeds(const led_t array[maxLeds]);

    static void setupLedGroundPins() {
#ifdef SIGNAL_LED_1_GND
        groundPin(SIGNAL_LED_1_GND);
#endif
#ifdef SIGNAL_LED_2_GND
        groundPin(SIGNAL_LED_2_GND);
#endif
#ifdef SIGNAL_LED_3_GND
        groundPin(SIGNAL_LED_3_GND);
#endif
#ifdef SIGNAL_LED_4_GND
        groundPin(SIGNAL_LED_4_GND);
#endif
#ifdef SIGNAL_LED_5_GND
        groundPin(SIGNAL_LED_5_GND);
#endif
#ifdef SIGNAL_LED_6_GND
        groundPin(SIGNAL_LED_6_GND);
#endif
#ifdef SIGNAL_LED_7_GND
        groundPin(SIGNAL_LED_7_GND);
#endif
#ifdef SIGNAL_LED_8_GND
        groundPin(SIGNAL_LED_8_GND);
#endif

    }
};
