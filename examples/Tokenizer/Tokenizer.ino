#include <Arduino.h>
#include "Tokenizer.h"
#ifndef ARDUINO
#include <iostream>
#include <unistd.h>
#endif

#ifdef ARDUINO
Stream& stream = Serial;
#else
Stream stream(std::cout);
#endif

void setup() {
#ifdef ARDUINO
    Serial.begin(9600);
#endif
    Tokenizer tokenizer(" Lorem ipsum   dolor sit amet, consectetur, adipisci velit ");
    while (true) {
        String token = tokenizer.getToken();
        if (token.length() == 0)
            break;
        stream.println(token);
    }

    Tokenizer tokLines("line 1\nline2");
    while (true) {
        String token = tokLines.getLine();
        if (token.length() == 0)
            break;
        stream.println(token);
    }

    Tokenizer tokStuff("command 12 34 60000 70000 1234 123456 Lorem ipsum   dolor sit amet");
    stream.println(tokStuff.getToken());
    stream.println(tokStuff.getInt() - 1);
    stream.println(tokStuff.getInt() - 1);
    stream.println(tokStuff.getMicroseconds() - 1);
    stream.println(tokStuff.getMicroseconds() - 1);
    stream.println(tokStuff.getFrequency() - 1);
    stream.println(tokStuff.getFrequency() - 1);
    stream.println(tokStuff.getRest());
}

void loop() {
}


#ifndef ARDUINO
int main() {
    //sleep(20);
    setup();
}
#endif
