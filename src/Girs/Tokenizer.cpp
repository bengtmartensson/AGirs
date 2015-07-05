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
    return getToken().toInt();
}