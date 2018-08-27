#include "Remote.h"

Remote::Remote(const char *name_, const Command *commands_, size_t size_) :
name(name_),commands(commands_),size(size_) {
}

Remote::Remote(const Remote& orig) :
name(orig.name),commands(orig.commands),size(orig.size) {
}

Remote::~Remote() {
}

const IrSignal* Remote::getIrSignal(const char* name) const {
    for (unsigned int i = 0; i < size; i++) {
        if (strcmp(name, commands[i].getName()) == 0)
            return commands[i].getIrSignal();
    }
    return NULL;
}
