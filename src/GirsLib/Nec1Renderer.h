//{38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,1,^108m,(16,-4,1,^108m)*) [D:0..255,S:0..255=255-D,F:0..255]

#ifndef NEC1RENDERER_H
#define	NEC1RENDERER_H

#include <Arduino.h>
#include "IrSignal.h"

class Nec1Renderer {
public:
    static const unsigned int frequency = 38400;
    static const unsigned int introLength = 68;
    static const unsigned int repeatLength = 4;
    static const unsigned int endingLength = 0;
    //Nec1Renderer();
    //Nec1Renderer(const Nec1Renderer& orig);
    Nec1Renderer(unsigned int D, unsigned int S, unsigned int F);
    //Nec1Remderer(uint16_t D, uint16_t F)
    virtual ~Nec1Renderer();
    IrSignal* getSignal() const;

    static IrSignal* render(unsigned int D, unsigned int S, unsigned int F);
private:
    uint16_t intro[introLength];
    static const uint16_t repeat[repeatLength];
    void lsbByte(int& i, uint32_t& sum, unsigned int D);
    void process(int& i, uint32_t& sum, int data);

};

#endif	/* NEC1RENDERER_H */

