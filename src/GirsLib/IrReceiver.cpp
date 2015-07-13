#include "IrReceiver.h"

IrReceiver::IrReceiver(pin_t pin_, boolean pullup, duration_t me) {
    pin = pin_;
    markExcess = me;
    pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
}

IrReceiver::~IrReceiver() {
}
