#include "RemoteSet.h"

RemoteSet::RemoteSet(const Remote** remotes_, size_t size_) : remotes(remotes_),size(size_) {
}

RemoteSet::RemoteSet(const RemoteSet& orig) : remotes(orig.remotes),size(orig.size) {
}

RemoteSet::~RemoteSet() {
}

const Remote *RemoteSet::getRemote(const char* name) const {
    for (unsigned int i = 0; i < size; i++) {
        if (strcmp(name, remotes[i]->getName()) == 0)
            return remotes[i];
    }
    return NULL;
}
