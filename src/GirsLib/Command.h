#pragma once

#include <IrSignal.h>

class Command {
public:
    Command(const char* name_, const IrSignal *irSignal_) : name(name_),irSignal(irSignal_) {};
    Command(const Command& orig) : name(orig.name),irSignal(orig.irSignal) {};
    virtual ~Command();

    const char *getName() const { return name; };
    const IrSignal* getIrSignal() const { return irSignal; };

private:
    const char *name;
    const IrSignal *irSignal;
};
