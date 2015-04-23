/* IR Widget: capture a raw IR signal and dump the timing of the non-demodulated signal

http://www.piclist.com/images/boards/irwidget/index.htm
http://www.hifi-remote.com/forums/dload.php?action=file&file_id=2044
http://www.hifi-remote.com/wiki/index.php?title=IR_Scope_and_IR_Widget_User%27s_Guide
http://www.compendiumarcana.com/irwidget/

Arduino digital pin numbers for the input capture pin (ICP) and the logic analyzer debugging pin (LA Dbg):
Board name / MCU                           | ICP pin                  | LA Dbg pin
-------------------------------------------|--------------.-----------|------------------------
Duemilanove/Uno (ATmega328P / ATmega168)   | ICP1/PB0, Arduino pin 8  | PD6, Arduino pin 6
Leonardo (ATmega32U4)                      | ICP1/PD4, Arduino pin 4  | PD6, Arduino pin 12
Arduino Mega 2560 (ATmega2560)             | ICP4/PL0, Arduino pin 49 | PL6, Arduino pin 43

see also here:
http://arduino.cc/en/Hacking/PinMapping168 (also for ATmega328P)
http://arduino.cc/en/Hacking/PinMapping32u4
http://arduino.cc/en/Hacking/PinMapping2560
 */

// Copyright (c) 2012 Michael Dreher  <michael(at)5dot1.de>
// this code may be distributed under the terms of the General Public License V2 (GPL V2)

// Code slighty reorganized by Bengt Martensson

#include "IrWidget.h"

IrWidget::IrWidget(unsigned int bufSize_, Stream* stream_) : IrCapturer(bufSize_, stream_) {
    stream = stream_;
    bufSize = bufSize_;
    captureData = new uint16_t[bufSize];
    sensorIsInverting = false; // true means the sensor signal is inverted (low = signal on)
    endingTimeout = _BV(RANGE_EXTENSION_BITS) - 1;
    setup();
    // make sure the whole capture buffer can be used. Your sketch will crash here if the buffer is too large.
    for (uint16_t i = 0; i < bufSize; i++)
        captureData[i] = 0;
}

IrWidget::~IrWidget() {
    delete[] captureData;
}

void IrWidget::setEndingTimeout(uint32_t timeout) {
    endingTimeout = (ovlBitsDataType) ((timeout/1000 + 16)/32);
}

uint32_t IrWidget::getEndingTimeout() const {
    //return (uint16_t) (timerValueToNanoSeconds((((uint32_t) endingTimeout)*CAPTURE_PRESCALER_FACTOR)) / 1000);
    return ((uint32_t) endingTimeout) << 15;
}

////////////////////////////////////////////////////////////////////////////////
// Initialization
////////////////////////////////////////////////////////////////////////////////

// returns 'true' when the sensor is non-inverting, 'false' otherwise
bool IrWidget::checkForInvertingSensor() {
    uint32_t tStart = micros();
    uint32_t tDelta = 0;
    bool negativePolarity = false;

    for (;;) {
        if (((CAT2(PIN, CAP_PORT) & _BV(CAP_PIN)) != 0) != negativePolarity) {
            // if the value has changed, reset the counter and change the polarity
            negativePolarity = !negativePolarity;
            tStart = micros();
        } else {
            tDelta = micros() - tStart;
            if (tDelta >= 1000)
                break; // the value has not changed for the threshold time
        }
    }

    return negativePolarity;
}


// initialize Timer and IO pins, needs to be called once before calling startCapture()
void IrWidget::setup() {
    // configure signal capture ICP pin as input
    cbi(CAT2(DDR, CAP_PORT), CAP_PIN);
#ifdef ENABLE_PULL_UP
    sbi(CAT2(PORT, CAP_PORT), CAP_PIN); // enable the internal 10k pull-up resistor
#endif

#if defined(DEBUG_PIN) && defined(DEBUG_PORT)
    sbi(CAT2(DDR, DEBUG_PORT), DEBUG_PIN); // configure logic analyzer debug pin as output
#endif

    // init timer, disable power save mode of timer
#ifdef PRR0 // for ATmega32U4 and ATmega2560
#if PRTIM <= 2
    cbi(PRR0, CAT2(PRTIM, CAP_TIM)); // for ATmega32U4 and ATmega2560
#else
    cbi(PRR1, CAT2(PRTIM, CAP_TIM)); // for ATmega2560
#endif
#else
    cbi(PRR, CAT2(PRTIM, CAP_TIM));
#endif

    CAT3(TCCR, CAP_TIM, A) = 0; // Timer mode 0 = normal
    CAT3(TCCR, CAP_TIM, B) = _BV(CAT2(ICNC, CAP_TIM)) | CAPTURE_PRESCALER_SETTING; // prescaler according to setting, enable noise canceler

    // TODO: automatically determine if the sensor is inverting:
    //   wait for a long period without level changes (e.g. 1ms) and use the value as the low value
    sensorIsInverting = checkForInvertingSensor();
}
