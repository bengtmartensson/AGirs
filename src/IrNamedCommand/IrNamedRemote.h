#ifndef IRREMOTE_H
#define	IRREMOTE_H

#include "IrNamedCommand.h"

class IrNamedRemote {
public:
    IrNamedRemote() : name(""), noCommands(0) {
    };

    IrNamedRemote(const IrNamedRemote& orig) : name(orig.name), commands(orig.commands), noCommands(orig.noCommands) {
    };

    virtual ~IrNamedRemote() {
    };

    IrNamedRemote(const char* name_, const IrNamedCommand* const * commands_, unsigned int length_)
    : name(name_), commands(commands_), noCommands(length_) {
    };

    const char* getName() const {
        return name;
    }

    /** Returns the IrNamedCommand with matching name, or NULL if not found. */
    const IrNamedCommand *getIrNamedCommand(const char *name) const;

    const IrNamedCommand * const *getAllCommands() const {
        return commands;
    }

    unsigned int getNoCommands() const {
        return noCommands;
    }

private:
    const char* name;
    const IrNamedCommand * const *commands;
    unsigned int noCommands;
};

#endif	// IRREMOTE_H

