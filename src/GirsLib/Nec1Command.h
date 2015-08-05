#ifndef NEC1COMMAND_H
#define	NEC1COMMAND_H

#include "Command.h"
#include "Nec1Renderer.h"

class Nec1Command : public Command {
public:
    Nec1Command(const char* name, uint8_t D_, uint8_t S_, uint8_t F_) : Command(name),D(D_),S(S_) { F = F_; };
    Nec1Command(const char* name, uint8_t D_, uint8_t F_) : Command(name),D(D_),S((uint8_t)255-D_) { F = F_; };
    virtual ~Nec1Command() {};
    virtual const Nec1Renderer* getRenderer() const { return new Nec1Renderer(D, S, F); };
private:
    uint8_t D;
    uint8_t S;
    uint8_t F;
};

#endif	/* NEC1COMMAND_H */

