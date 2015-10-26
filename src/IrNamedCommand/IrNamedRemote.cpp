#include "IrNamedRemote.h"

const IrNamedCommand *IrNamedRemote::getIrNamedCommand(const char* name) const {
    for (unsigned int i = 0; i < noCommands; i++) {
        for (unsigned int j = 0; ; j++) {
            if (name[j] != commands[i]->getName()[j])
                break;
            if (name[j] == '\0')
                return commands[i];
        }
    }
    return NULL;
}
