#pragma once

#include "Remote.h"

class RemoteSet {
public:
    RemoteSet(const Remote* remotes_[], size_t size_);
    RemoteSet(const RemoteSet& orig);
    virtual ~RemoteSet();

    const Remote** getRemotes() const { return remotes; };
    size_t getSize() const { return size; };
    const Remote* getRemote(const char* name) const;
    const Remote* getRemote(const String& str) const { return getRemote(str.c_str()); };

private:

    const Remote** remotes;
    size_t size;
};
