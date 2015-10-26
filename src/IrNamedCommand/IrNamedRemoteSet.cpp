#include "IrNamedRemoteSet.h"

const IrNamedRemote* IrNamedRemoteSet::getIrNamedRemote(const char* name) const {
    for (unsigned int i = 0; i < noRemotes; i++) {
        for (unsigned int j = 0; ; j++) {
            if (name[j] != remotes[i]->getName()[j])
                break;
            if (name[j] == '\0')
                return remotes[i];
        }
    }
    return NULL;
}
