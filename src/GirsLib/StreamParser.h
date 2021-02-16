/**
 * Simple class for parsing stuff from an (input) Stream.
 * The reads are blocking.
 * To be used instead instead of parseInt() and such,
 * since they do not offer enough flexibility.
 * This is not written to be used outside of the package.
 */

#pragma once

#include "IrSignal.h"

class StreamParser {
public:
    /**
     * Constructor.
     * @param stream Stream, assumed already opened.
     */
    StreamParser(Stream &stream);
#ifndef DOXYGEN
    StreamParser() = delete;
    StreamParser(const StreamParser&) = delete;
    StreamParser(StreamParser&&) = delete;
    StreamParser& operator=(const StreamParser&) = delete;
    StreamParser& operator=(StreamParser&&) = delete;
#endif

    /**
     * Destructor. Does not close or destroy the stream.
     */
    virtual ~StreamParser() {};

    /**
     * Reads the stream and parses it into an IrSignal. Blocking.
     * Expected format: frequency length_irsequence length_repeat length_ending data...
     * Signs are allowed and ignored.
     * @return pointer to IrSignal
     */
    IrSignal* parseIrSignal();

    /**
     * Reads a Pronto Hex format IR signal.
     * @param stream
     * @return
     */
    IrSignal* parsePronto();

    /**
     * Reads a number and returns it as 16 bit unsigned. Blocking. Signs are ignored.
     * @return int16_t
     */
    int32_t parseAbsInt();

    /**
     * Reads a number and returns it as 16 bit unsigned. Blocking. Signs are ignored.
     * If no valid character remains before line end, return the fallback argument.
     * @param fallback Default value
     * @return int16_t
     */
    int32_t parseAbsIntDefault(int32_t fallback);

    /**
     * Returns a word (separated by whitespace).
     * @param buf
     * @param buflen
     * @return Parsed word
     */
    const char* parseWord(char* buf, size_t buflen);

    const char* getLine(char* buf, size_t buflen);

    Stream& getStream() { return stream; }

    void flushLine();

    /**
     * Convenience function: combines the constructor and parseIrSignal. Blocking.
     * @param stream
     * @return
     */
    static IrSignal* parse(Stream &stream);

    microseconds_t* parseData(microseconds_t* buffer, size_t length);

    static constexpr int invalid = -1;

private:

    //static constexpr int32_t invalidInt = -1L;

    Stream &stream;
    microseconds_t* parseData(size_t length);
    int32_t parseAbsInt(char initial);
    char customReadChar();
    void disposeUntilWhitespace();
    static unsigned int parseHex(char c);
    /**
     * Reads a four digit hexadecimal number and returns it as 16 bit unsigned. Blocking.
     * @return uint16_t
     */
    uint16_t parseProntoNumber();
};
