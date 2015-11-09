#ifndef IRCOMMAND_H
#define	IRCOMMAND_H

#include "IrSignal.h"

/** This class is basically an IrSignal with a name. The class is immutable. */
class IrNamedCommand {
public:

    IrNamedCommand(const char* name_) : name(name_) {
    };

    IrNamedCommand(const IrNamedCommand& orig) : name(orig.name) {
    };

    virtual ~IrNamedCommand() {
    };
    virtual const IrSignal& getIrSignal() const = 0;

    const char* getName() const {
        return name;
    }

    operator const IrSignal& () const {
        return getIrSignal();
    }

private:
    const char *name;
};

#endif	/* IRCOMMAND_H */
