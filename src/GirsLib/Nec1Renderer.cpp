#include "Nec1Renderer.h"

Nec1Renderer::~Nec1Renderer() {
}

const microseconds_t Nec1Renderer::repeat[repeatLength] = { 9024, 2256, 564, 65535 };

Nec1Renderer::Nec1Renderer(unsigned int D, unsigned int S, unsigned int F) {
    int i = 0;
    uint32_t sum = 9024 + 4512 + 564;
    intro[i++] = 9024;
    intro[i++] = 4512;
    lsbByte(i, sum, D);
    lsbByte(i, sum, S);
    lsbByte(i, sum, F);
    lsbByte(i, sum, 255-F);
    intro[i++] = 564;
    intro[i++] = (uint16_t) (108000L - sum);
}

IrSignal* Nec1Renderer::getSignal() const {
    return new IrSignal(frequency, introLength, repeatLength, endingLength, intro, repeat, NULL);
}

void Nec1Renderer::lsbByte(int& i, uint32_t& sum, unsigned int X) {
    for (int index = 0; index < 8; index++) {
        process(i, sum, X & 1);
        X >>= 1;
    }
}

void inline Nec1Renderer::process(int& i, uint32_t& sum, int data) {
    intro[i++] = 564;
    uint16_t gap = data ? 1692 : 564;
    intro[i++] = gap;
    sum += data ? 564+1692 : 564+564;
}

IrSignal* Nec1Renderer::render(unsigned int D, unsigned int S, unsigned int F) {
    return (new Nec1Renderer(D, S, F))->getSignal();
}