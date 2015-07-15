// {36k,msb,889}<1,-1|-1,1>((1:1,~F:1:6,T:1,D:5,F:6,^114m)+,T=1-T)[T@:0..1=0,D:0..31,F:0..127]

#ifndef RC5RENDERER_H
#define	RC5RENDERER_H

#include <Arduino.h>
#include "IrSignal.h"

class Rc5Renderer {
public:
    static const frequency_t frequency = 36000;
    static const uint16_t introLength = 0;
    static const uint16_t endingLength = 0;
    Rc5Renderer(unsigned int D, unsigned int F, unsigned int T);
    virtual ~Rc5Renderer();
    IrSignal* getSignal();

    static IrSignal* render(unsigned int D, unsigned int F, unsigned int T);
private:
    uint16_t index;
    microseconds_t repeat[28];
    int pending;
    static const microseconds_t timebase = 889;
    void emit(unsigned int t);
    void emitMsb(unsigned int x, unsigned int length);
    void emitEnd();
};

#endif	/* RC5RENDERER_H */

