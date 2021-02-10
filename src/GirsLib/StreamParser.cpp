#include "StreamParser.h"
#include "Pronto.h"
#include <string.h>

#ifndef ARDUINO
#define Stream std::istream
#endif

StreamParser::StreamParser(Stream &stream_) : stream(stream_) {
}

IrSignal* StreamParser::parseIrSignal() {
    frequency_t frequency = static_cast<frequency_t> (parseAbsInt());
    size_t introLength = parseAbsInt();
    size_t repeatLength = parseAbsInt();
    size_t endingLength = parseAbsInt();

    microseconds_t *introData = parseData(introLength);
    microseconds_t *repeatData = parseData(repeatLength);
    microseconds_t *endingData = parseData(endingLength);

    return new IrSignal(introData, introLength, repeatData, repeatLength, endingData, endingLength, frequency);
}

IrSignal* StreamParser::parse(Stream &stream) {
    StreamParser parser(stream);
    return parser.parseIrSignal();
}

const char* StreamParser::parseWord(char* buf, size_t buflen) {
#ifdef ARDUINO
    while (stream.available() == 0)
        yield();

    unsigned i = 0U;
    while (true) {
        yield();
        int c = stream.read(); // non-blocking, returns immediately
        if (c == invalid)
            continue;
        if (isspace(c)) {
            if (i == 0 && isblank(c))
                continue; // ignore leading spaces/tabs, but not CR/LF
            else
                break;
        }

        if (i == buflen - 1) {
            disposeUntilWhitespace();
            break;
        }
        buf[i] = static_cast<char>(c);
        i++;
    }
    buf[i] = '\0';

#else
    std::string str;
    stream >> str;
    strncpy(buf, str.c_str(), buflen);
    //if (str.length() > buflen - 1)
    //    str.resize(buflen - 1);

    return str.c_str();
#endif
    return buf;
}

const char* StreamParser::getLine(char* buf, size_t buflen) {
    #ifdef ARDUINO
    while (stream.available() == 0)
        yield();

    unsigned i = 0U;
    while (true) {
        yield();
        int c = stream.read(); // non-blocking, returns immediately
        if (c == invalid)
            continue;
        if (isspace(c)) {
            if (isblank(c)) {
                if (i == 0)
                    continue; // ignore leading spaces/tabs, but not CR/LF
            } else
                break;
        }

        if (i == buflen - 1) {
            disposeUntilWhitespace();
            break;
        }
        buf[i] = static_cast<char>(c);
        i++;
    }
    buf[i] = '\0';
    return buf;
#else
    (void) buf;
    (void) buflen;
    std::string str;
    stream >> str;
    return str.c_str();
#endif
}

int32_t StreamParser::parseAbsInt() {
    char c;
    do {
        c = readCharBlock();
    } while (isspace(c) || c == '-' || c == '+');

    int32_t value = 0;
    while (true) {
        if (isspace(c))
            return value;
        int32_t n = static_cast<int> (c) - static_cast<int> ('0');
        if (n < 0 || n > 9) {
            disposeUntilWhitespace();
            return invalid;
        }
        value = value * 10 + n;
        c = readCharBlock();
    }
}

uint16_t StreamParser::parseProntoNumber() {
    char c;
    do {
        c = readCharBlock();
    } while (isspace(c));

    int32_t value = 0;
    for (unsigned i = 0; i < Pronto::digitsInProntoNumber; i++) {
        int n = parseHex(c);
        value = value * 16 + n;
        c = readCharBlock();
    }
    return value;
}

IrSignal* StreamParser::parsePronto() {
    uint16_t type = parseProntoNumber();
    uint16_t frequencyCode = parseProntoNumber();
    uint16_t introPairs = parseProntoNumber();
    uint16_t repeatPairs = parseProntoNumber();
    size_t size = Pronto::numbersInPreamble + 2 * (introPairs + repeatPairs);
    uint16_t* data = new microseconds_t[size];
    data[0] = type;
    data[1] = frequencyCode;
    data[2] = introPairs;
    data[3] = repeatPairs;
    for (unsigned i = Pronto::numbersInPreamble; i < size; i++)
        data[i] = parseProntoNumber();
    return Pronto::parse(data, size);
}

unsigned int StreamParser::parseHex(char c) {
    return
            c < '0' ? invalid
            : c <= '9' ? static_cast<unsigned>(c - '0')
            : c < 'A'  ? invalid
            : c <= 'F' ? static_cast<unsigned>(c - 'A') + 10U
            : c < 'a'  ? invalid
            : c <= 'f' ? static_cast<unsigned>(c - 'a') + 10U
            : invalid;
}

char StreamParser::readCharBlock() {
    int c;
#ifdef ARDUINO
    do {
        c = stream.read(); // does not block
    } while (c == invalid);
#else
    c = stream.get();
#endif

    return static_cast<char> (c);
}

void StreamParser::disposeUntilWhitespace() {
#ifdef ARDUINO
    int c;
    do {
        c = stream.read();
    } while (isspace(c));
#endif
}

microseconds_t* StreamParser::parseData(size_t length) {
    microseconds_t* data = new microseconds_t[length];
    return parseData(data, length);
}

microseconds_t* StreamParser::parseData(microseconds_t* data, size_t length) {
    for (unsigned int i = 0; i < length; i++)
        data[i] = parseAbsInt();

    return data;
}
