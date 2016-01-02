#include "IrNamedRemote.h"
#include <string.h>

const IrNamedCommand *IrNamedRemote::getIrNamedCommand(const char* name) const {
    for (unsigned int i = 0; i < noCommands; i++) {
        if (strcmp(name, commands[i]->getName()) == 0)
            return commands[i];
    }
    return NULL;
}
