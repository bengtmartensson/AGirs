#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include <GirsTypes.h>

class Tokenizer {
private:
    int index;
    String& payload;
public:
    Tokenizer(String& str) : index(0), payload(str) {};
    String getToken();
    String getRest();
    long getInt();
    microseconds_t getMicroseconds();
    frequency_t getFrequency();
};

#endif // _TOKENIZER_H