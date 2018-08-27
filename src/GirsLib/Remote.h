#pragma once

#include "Command.h"

class Remote {
public:
    Remote(const char *name_, const Command *commands_[], size_t size_);
    Remote(const Remote& orig);
    virtual ~Remote();

    const char *getName() const { return name; }
    size_t getSize() const { return size; }
    const IrSignal *getIrSignal(const char* name) const;
    const IrSignal *getIrSignal(const String& str) const { return getIrSignal(str.c_str()); };
    const Command* getCommands() const { return commands; }

private:
    const char *name;
    const Command *commands;
    size_t size;
};
