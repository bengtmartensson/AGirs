#include "LedLcdManager.h"
#include "Tokenizer.h"

LiquidCrystal_I2C *LedLcdManager::lcd;
milliseconds_t LedLcdManager::blinkTime = defaultBlinkTime;
unsigned long LedLcdManager::turnOffTime;

pin_t LedLcdManager::physicalLeds[maxLeds];
led_t LedLcdManager::logicalLeds[maxLeds];
boolean LedLcdManager::shouldTimeOut[maxLeds];

void LedLcdManager::setup(int i2cAddress, uint8_t columns, uint8_t rows,
        const pin_t physicalLeds_[], const pin_t logicalLeds_[], const boolean shouldTimeOut_[]) {
    setupLcdI2c(i2cAddress, columns, rows);
    setupPhysicalLeds(physicalLeds_);
    setupLogicLeds(logicalLeds_);
    setupShouldTimeOut(shouldTimeOut_);
    disableTurnOffTime();
}

void LedLcdManager::setPhysicalLed(led_t physicalLed, LedState state) {
    pin_t pin = physicalLeds[physicalLed-1];
    if (pin == invalidPin)
        return;

#ifdef ARDUINO
    digitalWrite(pin, state == off ? LOW : HIGH);
#else
    std::cout << "setPhysicalLed: pin = " << (int) pin << ", " << (state == off ? "off" : "on") << std::endl;
#endif
    if (state == blink)
        updateTurnOffTime();
}

void LedLcdManager::setLogicLed(led_t logicLed, LedState state) {
    led_t physicalLed = logicalLeds[logicLed-1];
    if (physicalLed != invalidLed)
        setPhysicalLed(physicalLed, state);
}

void LedLcdManager::setupLogicLed(led_t logicLed, led_t physicalLed) {
    logicalLeds[logicLed-1] = physicalLed;
}

void LedLcdManager::setupLogicLeds(const led_t logicalLeds_[maxLeds]) {
    for (int i = 0; i < maxLeds; i++)
        logicalLeds[i] = logicalLeds_ == NULL ? i+1 : logicalLeds_[i];
}

void LedLcdManager::setupPhysicalLeds(const led_t physicalLeds_[maxLeds]) {
    for (int i = 0; i < maxLeds; i++) {
        physicalLeds[i] = physicalLeds_ == NULL ? invalidPin : physicalLeds_[i];
//#ifdef ARDUINO
        if (physicalLeds[i] != invalidPin)
            pinMode(physicalLeds[i], OUTPUT);
//#endif
    }
}

void LedLcdManager::setupShouldTimeOut(const boolean shouldTimeOut_[maxLeds]) {
    for (int i = 0; i < maxLeds; i++)
        shouldTimeOut[i] = shouldTimeOut_ == NULL ? true : shouldTimeOut_[i];
}

void LedLcdManager::setupLcdI2c(int i2cAddress, uint8_t columns, uint8_t rows) {
    lcd = i2cAddress >= 0 ? new LiquidCrystal_I2C(i2cAddress, columns, rows) : NULL;
    if (lcd)
        lcd->begin();
}

void LedLcdManager::updateTurnOffTime() {
    turnOffTime = millis() + blinkTime;
}

void LedLcdManager::selfTest(const String& text) {
    lcdPrint(text, true);
    for (led_t i = 1; i <= maxLeds; i++)
        setLogicLed(i, on);
    delay(defaultBlinkTime);
    allOff(true);
}

void LedLcdManager::checkTurnoff() {
    if (millis() > turnOffTime)
        allOff(false);
}

void LedLcdManager::allOff(boolean force) {
    if (lcd) {
        lcd->noDisplay();
        lcd->noBacklight();
    }
    for (led_t i = 1; i <= maxLeds; i++)
        if (force || shouldTimeOut[i - 1])
            setLogicLed(i, off);

    disableTurnOffTime();
}

void LedLcdManager::disableTurnOffTime() {
    turnOffTime = (unsigned long) -1;
}




void LedLcdManager::lcdPrint(String const& str, boolean clear, int x, int y) {
    if (!lcd)
        return;

    int row = (y < 0 && clear) ? 0 : y;
    int column = x;
    if (clear)
        lcd->clear();
    lcd->display();
    lcd->backlight();

    Tokenizer tokenizer(str);
    while (true) {
        String line = tokenizer.getLine();
        if (line.length() == 0)
            break;
        if (row >= 0)
            lcd->setCursor(column, row++);
        column = 0;
        lcd->print(line);
    }
    updateTurnOffTime();
}
