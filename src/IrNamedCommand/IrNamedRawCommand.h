#ifndef IRRAWCOMMAND_H
#define	IRRAWCOMMAND_H

#include "IrSignal.h"
#include "IrNamedCommand.h"
#include "IrSender.h"

class IrNamedRawCommand : public IrNamedCommand {
private:
    IrSignal irSignal;

public:
    IrNamedRawCommand(const IrNamedRawCommand& orig) : IrNamedCommand(orig), irSignal(orig.irSignal) {
    };

    IrNamedRawCommand(const char *name, const IrSignal& irSignal_) : IrNamedCommand(name), irSignal(irSignal_) {
    };

    virtual ~IrNamedRawCommand() {
    };

    const IrSignal& getIrSignal() const {
        return irSignal;
    };
};

#endif	/* IRRAWCOMMAND_H */
