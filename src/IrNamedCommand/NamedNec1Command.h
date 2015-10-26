#ifndef NEC1COMMAND_H
#define	NEC1COMMAND_H

#include "IrNamedCommand.h"
#include "Nec1Renderer.h"

class NamedNec1Command : public IrNamedCommand {
public:
    // Arduino defines a macro F(...), therefore the assymmetry in the constructors

    NamedNec1Command(const char* name, uint8_t D_, uint8_t S_, uint8_t F_) : IrNamedCommand(name), D(D_), S(S_) {
        F = F_;
    };

    NamedNec1Command(const String& name, uint8_t D_, uint8_t S_, uint8_t F_) : IrNamedCommand(name), D(D_), S(S_) {
        F = F_;
    };

    NamedNec1Command(const char* name, uint8_t D_, uint8_t F_) : IrNamedCommand(name), D(D_), S((uint8_t) 255 - D_) {
        F = F_;
    };

    NamedNec1Command(const String& name, uint8_t D_, uint8_t F_) : IrNamedCommand(name), D(D_), S((uint8_t) 255 - D_) {
        F = F_;
    };

    virtual ~NamedNec1Command() {
    };

    const IrSignal& getIrSignal() const {
        return *(new Nec1Renderer(D, S, F));
    };
private:
    uint8_t D;
    uint8_t S;
    uint8_t F;
};

#endif	/* NEC1COMMAND_H */
