#include <Arduino.h>
#include "Tokenizer.h"

#ifdef ARDUINO
#define TOKEN2INT(tok) tok.toInt()
#else
#define TOKEN2INT(tok) std::stoi(tok)
#endif

//static boolean isWhiteSpace(char c) {
//    return c == ' ' || c == '\t' || c == '\n' || c == '\t';
//}

/*Tokenizer::Tokenizer(const char *str) : index(0) {
    for (payload = str; isWhiteSpace(*payload); payload++)
        ;
    char *p;
    for (p = payload; *p != '\0'; p++)
        ;
    for (;; p--) {
        if (!isWhiteSpace(*p))
            break;
        *p = '\0';
    }
}*/

Tokenizer::Tokenizer(const String& str) : index(0), payload(str) {
    trim();
}

Tokenizer::Tokenizer(const char *str) : index(0),payload(String(str)) {
    trim();
}

void Tokenizer::trim() {
#ifdef ARDUINO
    payload.trim();
#else
    payload.erase(0, payload.find_first_not_of(" \t\n\r"));
    payload.erase(payload.find_last_not_of(" \t\n\r")+1, payload.length() - payload.find_last_not_of(" \t\n\r"));
#endif
};

Tokenizer::~Tokenizer() {
}

String Tokenizer::getRest() {
    String result =
#ifdef ARDUINO
            payload.substring(index);
#else
            payload.substr(index);
#endif
    index = invalidIndex;
    return result;
}

String Tokenizer::getLine() {
    if (index == invalidIndex)
        return String("");

    int i = payload.indexOf('\n', index);
    String s = (i > 0) ?
#ifdef ARDUINO
        payload.substring(index, i) : payload.substring(index);
#else
        payload.substr(index, i - index) : payload.substr(index);
#endif
    index = (i > 0) ? i+1 : invalidIndex;
    return s;
}

String Tokenizer::getToken() {
    if (index < 0)
        return String("");

    int i = payload.indexOf(' ', index);
    String s = (i > 0) ?
#ifdef ARDUINO
        payload.substring(index, i) : payload.substring(index);
#else
        payload.substr(index, i-index) : payload.substr(index);
#endif
    index = (i > 0) ? i : invalidIndex;
    if (index != invalidIndex)
        while (payload.charAt(index) == ' ')
            index++;
    return s;
}

long Tokenizer::getInt() {
    String token = getToken();
    return token == "" ? (long) invalid : TOKEN2INT(token);
}

microseconds_t Tokenizer::getMicroseconds() {
    long t = TOKEN2INT(getToken());
    return (microseconds_t) ((t < MICROSECONDS_T_MAX) ? t : MICROSECONDS_T_MAX);
}

frequency_t Tokenizer::getFrequency() {
    long t = TOKEN2INT(getToken());
    return (frequency_t) ((t < FREQUENCY_T_MAX) ? t : FREQUENCY_T_MAX);
}
