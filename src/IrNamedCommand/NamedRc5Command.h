#ifndef RC5COMMAND_H
#define	RC5COMMAND_H

#include "Command.h"
#include "Rc5Renderer.h"

class Rc5Command : public Command {
public:

    Rc5Command(const char* name, uint8_t D_, uint8_t F_) : Command(name), D(D_) {
        F = F_;
    };

    virtual ~Rc5Command() {
    };

    const Rc5Renderer* getRenderer() const {
        return new Rc5Renderer(D, F);
    };
private:
    // NOTE: There is no T here, but there is a (persistent, static) T in Rc5Render.
    uint8_t D;
    uint8_t F;
};

#endif	/* RC5COMMAND_H */
