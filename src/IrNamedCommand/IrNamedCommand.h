#ifndef IRCOMMAND_H
#define	IRCOMMAND_H

#include "IrSignal.h"

/**
 * This class is basically an IrSignal with a name. The class is immutable.
 * Since there may be a large number of such commands, the individual instances should
 * be small. getIrSignal() however may be compatatively expensive.
 */
class IrNamedCommand {
public:

    IrNamedCommand(const char* name_) : name(name_) {
    };

    IrNamedCommand(const IrNamedCommand& orig) : name(orig.name) {
    };

    virtual ~IrNamedCommand() {
    };


    /**
     * Create an IrSignal.
     * @return new IrSignal.Must be deleted by the user.
     */
    virtual IrSignal *getIrSignal() const = 0;

    const char* getName() const {
        return name;
    }

private:
    const char *name;
};

#endif	/* IRCOMMAND_H */
