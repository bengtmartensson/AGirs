#ifdef LED

#define BLINK -1

static void allLedsOff();

static unsigned long turnoffTime = 0L; // millis() semantic (ms since progarm start)
static boolean somethingTurnedOn = false;

static PARAMETER_CONST unsigned long blinkTime =
#ifdef LCD
        2500L; // milliseconds
#else
        1000L;
#endif

void updateTurnoffTime() {
    somethingTurnedOn = true;
    turnoffTime = millis() + blinkTime;
}

static void checkTurnoff() {
    if (millis() > turnoffTime && somethingTurnedOn) {
        //ALL_LEDS_OFF;
        allLedsOff();
        LCD_OFF(lcd);
        somethingTurnedOn = false;
    }
}

void blinkAck(pin_t pin) {
    digitalWrite(pin, HIGH);
    updateTurnoffTime();
}

void setLogicLed(pin_t pin, int state) {
    uint8_t physicalPin = LED2PIN(pin);
    if (physicalPin > 0 && physicalPin < 255) {
        digitalWrite(physicalPin, state ? HIGH : LOW);
        if (state < 0)
            updateTurnoffTime();
    }
}
#else // ! LED

void updateTurnoffTime() {}
static void checkTurnoff() {}

#endif // ! LED
