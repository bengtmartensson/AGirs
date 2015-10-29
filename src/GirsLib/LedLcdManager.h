#ifndef LEDLCDMANAGER_H
#define	LEDLCDMANAGER_H

#include <InfraredTypes.h>
#include <IrSender.h>
#include <LiquidCrystal_I2C.h> //https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

typedef uint8_t led_t;

class LedLcdManager {
public:
    static const int maxLeds = 8;
    static const int defaultLcdRows = 2;
    static const int defaultLcdColumns = 16;

    static const int defaultBlinkTime = 2000;

    enum LedState {
        on,
        off,
        blink
    };


    static const led_t invalidLed = 255;

private:
    /** This array maps physical LEDs to hardware pins. */
    static pin_t physicalLeds[maxLeds];

    /** This array maps logical LEDs to physical LEDs. */
    static led_t logicalLeds[maxLeds];

    /** Indicates which logical leds should time out. */
    static boolean shouldTimeOut[maxLeds];

    static milliseconds_t blinkTime;

    /** Time to turn of, in the semantic of millis(). */
    static unsigned long turnOffTime;

    static LiquidCrystal_I2C *lcd;

    static LedLcdManager instance;

    static void setupPhysicalLeds(const pin_t physicalLeds[maxLeds]);
    static void setupShouldTimeOut(const boolean shouldTimeOut[maxLeds]);
    /*pin_t pled1, pin_t pled2 = IrSender.invalid_pin,
            pin_t pled3 = IrSender.invalid_pin, pin_t pled4 = IrSender.invalid_pin,
            pin_t pled5 = IrSender.invalid_pin, pin_t pled6 = IrSender.invalid_pin,
            pin_t pled7 = IrSender.invalid_pin, pin_t pled8 = IrSender.invalid_pin);*/

    static void setupLcdI2c(int i2cAddress, uint8_t columns, uint8_t rows);
    LedLcdManager();;

    //void initArray(char *array, uint8_t value);

    static void disableTurnOffTime();

public:

    //LedLcdManager(const LedLcdManager& orig);
    //virtual ~LedLcdManager();

    /** Sets up the instance, to be called before using the instance.  */
    static void setup(int i2cAddress = -1, uint8_t columns = defaultLcdColumns, uint8_t rows = defaultLcdRows,
            const pin_t physicalLeds[maxLeds] = NULL,
            const led_t logicalLeds[maxLeds] = NULL,
            const boolean shouldTimeOut[maxLeds] = NULL);
/*            pin_t pled1, pin_t pled2 = IrSender.invalid_pin,
            pin_t pled3 = IrSender.invalid_pin, pin_t pled4 = IrSender.invalid_pin,
            pin_t pled5 = IrSender.invalid_pin, pin_t pled6 = IrSender.invalid_pin,
            pin_t pled7 = IrSender.invalid_pin, pin_t pled8 = IrSender.invalid_pin);
*/
    //void setupLogicalLeds(pin_t logicalLeds[maxLeds]);
    /*pin_t led1, pin_t led2 = IrSender.invalid_pin,
            pin_t led3 = IrSender.invalid_pin, pin_t led4 = IrSender.invalid_pin,
            pin_t led5 = IrSender.invalid_pin, pin_t led6 = IrSender.invalid_pin,
            pin_t led7 = IrSender.invalid_pin, pin_t led8 = IrSender.invalid_pin);*/

    void static lcdPrint(const String& str, boolean clear = false, int x = 0, int y = -1);
    //void static lcdPrint(const String& str);

    //LedLcdManager& getInstance() {
    //    return instance;
    //};

    static LiquidCrystal_I2C& getLcd() {
        return *lcd;
    }

    static void allOff(boolean force);

    static milliseconds_t getBlinkTime() {
        return blinkTime;
    };

    static void setBlinkTime(milliseconds_t value) {
        blinkTime = value;
    }

    static void updateTurnOffTime();

    /** Turn off if it is due. This has to be called periodically by the user. */
    static void checkTurnoff();

    static void selfTest(const String& text);

    static void setPhysicalLed(led_t physicalLed, LedState state);

    static void setLogicLed(led_t logicLed, LedState state);

    static void setupLogicLed(led_t loginLed, led_t physicalLed);

    static void setupLogicLeds(const led_t array[maxLeds]);

    static void groundPin(pin_t pin) {
#ifdef ARDUINO
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
#else
        std::cout << "Grounded pin " << (int) pin << std::endl;
#endif
    }

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

#endif	/* LEDLCDMANAGER_H */
