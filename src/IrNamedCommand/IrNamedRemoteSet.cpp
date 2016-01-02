#include "IrNamedRemoteSet.h"
#include <string.h>

const IrNamedRemote* IrNamedRemoteSet::getIrNamedRemote(const char* name) const {
    for (unsigned int i = 0; i < noRemotes; i++) {
        if (strcmp(name, remotes[i]->getName()) == 0)
            return remotes[i];
    }
    return NULL;
}
