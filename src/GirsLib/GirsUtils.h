#pragma once

#include "LedLcdManager.h" // for led_t
#include <MultiDecoder.h>  // for MultiDecoder::Type
#ifdef ARDUINO
#include <IPAddress.h>
#endif
#include "defineMissingStuff.h"

class GirsUtils {
private:
    GirsUtils();

public:
    static unsigned long freeRam();
    static void reset();

    // TODO: where should this go?
    static led_t decode2logicalLed(MultiDecoder::Type type) {
        return (led_t) ((int) type - 1);
    }

    static void printVariable(Stream& stream, const char *variableName, unsigned long value) {
        stream.print(variableName);
        stream.print("=");
        stream.println(value);
    }

#ifdef ARDUINO
    static String ip2string(IPAddress ip);
#endif

    static pin_t receiverPin(uint8_t receiverNo UNUSED) {

#ifdef IRRECEIVER_1_PIN
        if (receiverNo == 1)
            return IRRECEIVER_1_PIN;
#endif

#ifdef IRRECEIVER_2_PIN
        if (receiverNo == 2)
            return IRRECEIVER_2_PIN;
#endif

#ifdef IRRECEIVER_3_PIN
        if (receiverNo == 3)
            return IRRECEIVER_3_PIN;
#endif

#ifdef IRRECEIVER_4_PIN
        if (receiverNo == 4)
            return IRRECEIVER_4_PIN;
#endif

#ifdef IRRECEIVER_5_PIN
        if (receiverNo == 5)
            return IRRECEIVER_5_PIN;
#endif

#ifdef IRRECEIVER_6_PIN
        if (receiverNo == 6)
            return IRRECEIVER_6_PIN;
#endif

#ifdef IRRECEIVER_7_PIN
        if (receiverNo == 7)
            return IRRECEIVER_7_PIN;
#endif

#ifdef IRRECEIVER_8_PIN
        if (receiverNo == 8)
            return IRRECEIVER_8_PIN;
#endif
        return invalidPin;
    }

    static bool receiverPullup(uint8_t receiverNo UNUSED) {

#ifdef IRRECEIVER_1_PIN
        if (receiverNo == 1)
            return IRRECEIVER_1_PULLUP_VALUE;
#endif

#ifdef IRRECEIVER_2_PIN
        if (receiverNo == 2)
            return IRRECEIVER_2_PULLUP_VALUE;
#endif

#ifdef IRRECEIVER_3_PIN
        if (receiverNo == 3)
            return IRRECEIVER_3_PULLUP_VALUE;
#endif

#ifdef IRRECEIVER_4_PIN
        if (receiverNo == 4)
            return IRRECEIVER_4_PULLUP_VALUE;
#endif

#ifdef IRRECEIVER_5_PIN
        if (receiverNo == 5)
            return IRRECEIVER_5_PULLUP_VALUE;
#endif

#ifdef IRRECEIVER_6_PIN
        if (receiverNo == 6)
            return IRRECEIVER_6_PULLUP_VALUE;
#endif

#ifdef IRRECEIVER_7_PIN
        if (receiverNo == 7)
            return IRRECEIVER_7_PULLUP_VALUE;
#endif

#ifdef IRRECEIVER_8_PIN
        if (receiverNo == 8)
            return IRRECEIVER_8_PULLUP_VALUE;
#endif
        return false;
    }

    static void setupLeds() {
#ifdef SIGNAL_LED_1_GND
        pinMode(SIGNAL_LED_1_GND, OUTPUT);
        digitalWrite(SIGNAL_LED_1_GND, LOW);
#endif

#ifdef SIGNAL_LED_2_GND
        pinMode(SIGNAL_LED_2_GND, OUTPUT);
        digitalWrite(SIGNAL_LED_2_GND, LOW);
#endif

#ifdef SIGNAL_LED_3_GND
        pinMode(SIGNAL_LED_3_GND, OUTPUT);
        digitalWrite(SIGNAL_LED_3_GND, LOW);
#endif

#ifdef SIGNAL_LED_4_GND
        pinMode(SIGNAL_LED_4_GND, OUTPUT);
        digitalWrite(SIGNAL_LED_4_GND, LOW);
#endif

#ifdef SIGNAL_LED_5_GND
        pinMode(SIGNAL_LED_5_GND, OUTPUT);
        digitalWrite(SIGNAL_LED_5_GND, LOW);
#endif

#ifdef SIGNAL_LED_6_GND
        pinMode(SIGNAL_LED_6_GND, OUTPUT);
        digitalWrite(SIGNAL_LED_6_GND, LOW);
#endif

#ifdef SIGNAL_LED_7_GND
        pinMode(SIGNAL_LED_7_GND, OUTPUT);
        digitalWrite(SIGNAL_LED_7_GND, LOW);
#endif

#ifdef SIGNAL_LED_8_GND
        pinMode(SIGNAL_LED_8_GND, OUTPUT);
        digitalWrite(SIGNAL_LED_8_GND, LOW);
#endif
    }

    static void setupReceivers() {
#ifdef IRRECEIVER_1_GND
        pinMode(IRRECEIVER_1_GND, OUTPUT);
        digitalWrite(IRRECEIVER_1_GND, LOW);
#endif

#ifdef IRRECEIVER_2_GND
        pinMode(IRRECEIVER_2_GND, OUTPUT);
        digitalWrite(IRRECEIVER_2_GND, LOW);
#endif

#ifdef IRRECEIVER_3_GND
        pinMode(IRRECEIVER_3_GND, OUTPUT);
        digitalWrite(IRRECEIVER_3_GND, LOW);
#endif

#ifdef IRRECEIVER_4_GND
        pinMode(IRRECEIVER_4_GND, OUTPUT);
        digitalWrite(IRRECEIVER_4_GND, LOW);
#endif

#ifdef IRRECEIVER_5_GND
        pinMode(IRRECEIVER_5_GND, OUTPUT);
        digitalWrite(IRRECEIVER_5_GND, LOW);
#endif

#ifdef IRRECEIVER_6_GND
        pinMode(IRRECEIVER_6_GND, OUTPUT);
        digitalWrite(IRRECEIVER_6_GND, LOW);
#endif

#ifdef IRRECEIVER_7_GND
        pinMode(IRRECEIVER_7_GND, OUTPUT);
        digitalWrite(IRRECEIVER_7_GND, LOW);
#endif

#ifdef IRRECEIVER_8_GND
        pinMode(IRRECEIVER_8_GND, OUTPUT);
        digitalWrite(IRRECEIVER_8_GND, LOW);
#endif

#ifdef IRRECEIVER_1_VSS
        pinMode(IRRECEIVER_1_VSS, OUTPUT);
        digitalWrite(IRRECEIVER_1_VSS, HIGH);
#endif

#ifdef IRRECEIVER_2_VSS
        pinMode(IRRECEIVER_2_VSS, OUTPUT);
        digitalWrite(IRRECEIVER_2_VSS, HIGH);
#endif

#ifdef IRRECEIVER_3_VSS
        pinMode(IRRECEIVER_3_VSS, OUTPUT);
        digitalWrite(IRRECEIVER_3_VSS, HIGH);
#endif

#ifdef IRRECEIVER_4_VSS
        pinMode(IRRECEIVER_4_VSS, OUTPUT);
        digitalWrite(IRRECEIVER_4_VSS, HIGH);
#endif

#ifdef IRRECEIVER_5_VSS
        pinMode(IRRECEIVER_5_VSS, OUTPUT);
        digitalWrite(IRRECEIVER_5_VSS, HIGH);
#endif

#ifdef IRRECEIVER_6_VSS
        pinMode(IRRECEIVER_6_VSS, OUTPUT);
        digitalWrite(IRRECEIVER_6_VSS, HIGH);
#endif

#ifdef IRRECEIVER_7_VSS
        pinMode(IRRECEIVER_7_VSS, OUTPUT);
        digitalWrite(IRRECEIVER_7_VSS, HIGH);
#endif

#ifdef IRRECEIVER_8_VSS
        pinMode(IRRECEIVER_8_VSS, OUTPUT);
        digitalWrite(IRRECEIVER_8_VSS, HIGH);
#endif
    }

    static void setupSensors() {
#ifdef IRSENSOR_1_GND
        pinMode(IRSENSOR_1_GND, OUTPUT);
        digitalWrite(IRSENSOR_1_GND, LOW);
#endif

#ifdef IRSENSOR_2_GND
        pinMode(IRSENSOR_2_GND, OUTPUT);
        digitalWrite(IRSENSOR_2_GND, LOW);
#endif

#ifdef IRSENSOR_3_GND
        pinMode(IRSENSOR_3_GND, OUTPUT);
        digitalWrite(IRSENSOR_3_GND, LOW);
#endif

#ifdef IRSENSOR_4_GND
        pinMode(IRSENSOR_4_GND, OUTPUT);
        digitalWrite(IRSENSOR_4_GND, LOW);
#endif

#ifdef IRSENSOR_5_GND
        pinMode(IRSENSOR_5_GND, OUTPUT);
        digitalWrite(IRSENSOR_5_GND, LOW);
#endif

#ifdef IRSENSOR_6_GND
        pinMode(IRSENSOR_6_GND, OUTPUT);
        digitalWrite(IRSENSOR_6_GND, LOW);
#endif

#ifdef IRSENSOR_7_GND
        pinMode(IRSENSOR_7_GND, OUTPUT);
        digitalWrite(IRSENSOR_7_GND, LOW);
#endif

#ifdef IRSENSOR_8_GND
        pinMode(IRSENSOR_8_GND, OUTPUT);
        digitalWrite(IRSENSOR_8_GND, LOW);
#endif

#ifdef IRSENSOR_1_VSS
        pinMode(IRSENSOR_1_VSS, OUTPUT);
        digitalWrite(IRSENSOR_1_VSS, HIGH);
#endif

#ifdef IRSENSOR_2_VSS
        pinMode(IRSENSOR_2_VSS, OUTPUT);
        digitalWrite(IRSENSOR_2_VSS, HIGH);
#endif

#ifdef IRSENSOR_3_VSS
        pinMode(IRSENSOR_3_VSS, OUTPUT);
        digitalWrite(IRSENSOR_3_VSS, HIGH);
#endif

#ifdef IRSENSOR_4_VSS
        pinMode(IRSENSOR_4_VSS, OUTPUT);
        digitalWrite(IRSENSOR_4_VSS, HIGH);
#endif

#ifdef IRSENSOR_5_VSS
        pinMode(IRSENSOR_5_VSS, OUTPUT);
        digitalWrite(IRSENSOR_5_VSS, HIGH);
#endif

#ifdef IRSENSOR_6_VSS
        pinMode(IRSENSOR_6_VSS, OUTPUT);
        digitalWrite(IRSENSOR_6_VSS, HIGH);
#endif

#ifdef IRSENSOR_7_VSS
        pinMode(IRSENSOR_7_VSS, OUTPUT);
        digitalWrite(IRSENSOR_7_VSS, HIGH);
#endif

#ifdef IRSENSOR_8_VSS
        pinMode(IRSENSOR_8_VSS, OUTPUT);
        digitalWrite(IRSENSOR_8_VSS, HIGH);
#endif
    }

    static bool sensorPullup(uint8_t sensorNo UNUSED) {

#ifdef IRSENSOR_1_PIN
        if (sensorNo == 1)
            return IRSENSOR_1_PULLUP_VALUE;
#endif

#ifdef IRSENSOR_2_PIN
        if (sensorNo == 2)
            return IRSENSOR_2_PULLUP_VALUE;
#endif

#ifdef IRSENSOR_3_PIN
        if (sensorNo == 3)
            return IRSENSOR_3_PULLUP_VALUE;
#endif

#ifdef IRSENSOR_4_PIN
        if (sensorNo == 4)
            return IRSENSOR_4_PULLUP_VALUE;
#endif

#ifdef IRSENSOR_5_PIN
        if (sensorNo == 5)
            return IRSENSOR_5_PULLUP_VALUE;
#endif

#ifdef IRSENSOR_6_PIN
        if (sensorNo == 6)
            return IRSENSOR_6_PULLUP_VALUE;
#endif

#ifdef IRSENSOR_7_PIN
        if (sensorNo == 7)
            return IRSENSOR_7_PULLUP_VALUE;
#endif

#ifdef IRSENSOR_8_PIN
        if (sensorNo == 8)
            return IRSENSOR_8_PULLUP_VALUE;
#endif
        return false;
    }

#ifndef NON_MOD_PIN
#define NON_MOD_PIN invalidPin
#endif
};
