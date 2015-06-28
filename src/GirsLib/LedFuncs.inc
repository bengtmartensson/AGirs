#ifdef LED

#define BLINK -1

static unsigned long turnoffTime = 0L; // millis() semantic (ms since progarm start)
static bool somethingTurnedOn = false;

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
        ALL_LEDS_OFF;
        LCD_OFF(lcd);
        somethingTurnedOn = false;
    }
}

void blinkAck(uint8_t pin) {
    digitalWrite(pin, HIGH);
    updateTurnoffTime();
}

void setLogicLed(uint8_t pin, int state) {
    digitalWrite(LED2PIN(pin), state ? HIGH : LOW);
    if (state < 0)
        updateTurnoffTime();
}
#else // ! LED

void updateTurnoffTime() {}
static void checkTurnoff() {}

#endif // ! LED
