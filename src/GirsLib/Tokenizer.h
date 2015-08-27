#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include <GirsTypes.h>
#include <limits.h>

class Tokenizer {
private:
    int index;
    String& payload;
public:
    Tokenizer(String& str) : index(0), payload(str) { payload.trim(); };
    String getToken();
    String getRest();
    String getLine();
    long getInt();
    microseconds_t getMicroseconds();
    frequency_t getFrequency();

    static const int invalid = INT_MAX;
};

#endif // _TOKENIZER_H
