#ifndef RC5COMMAND_H
#define	RC5COMMAND_H

#include "IrNamedCommand.h"
#include "Rc5Renderer.h"

class NamedRc5Command : public IrNamedCommand {
public:

    NamedRc5Command(const char* name, uint8_t D_, uint8_t F_) : IrNamedCommand(name), D(D_) {
        F = F_;
    };

    NamedRc5Command(const String& name, uint8_t D_, uint8_t F_) : IrNamedCommand(name), D(D_) {
        F = F_;
    };

    virtual ~NamedRc5Command() {
    };

    const IrSignal& getIrSignal() const {
        return new Rc5Renderer(D, F);
    };
private:
    // NOTE: There is no T here, but there is a (persistent, static) T in Rc5Render.
    uint8_t D;
    uint8_t F;
};

#endif	/* RC5COMMAND_H */
