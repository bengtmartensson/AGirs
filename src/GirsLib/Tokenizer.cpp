#include <Arduino.h>
#include "Tokenizer.h"

String Tokenizer::getRest() {
    String result = payload.substring(index);
    index = -1;
    return result;
}

String Tokenizer::getToken() {
    if (index < 0)
        return String("");

    int i = payload.indexOf(' ', index);
    String s = (i > 0) ? payload.substring(index, i) : payload.substring(index);
    index = (i > 0) ? i : -1;
    if (index != -1)
        while (payload.charAt(index) == ' ')
            index++;
    return s;
}

long Tokenizer::getInt() {
    String token = getToken();
    return token.length() > 0 ? token.toInt() : (long) invalid;
}

microseconds_t Tokenizer::getMicroseconds() {
    long t = getToken().toInt();
    return (microseconds_t) ((t < MICROSECONDS_T_MAX) ? t : MICROSECONDS_T_MAX);
}

frequency_t Tokenizer::getFrequency() {
    long t = getToken().toInt();
    return (frequency_t) ((t < FREQUENCY_T_MAX) ? t : FREQUENCY_T_MAX);
}
