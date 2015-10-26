#ifndef REMOTESET_H
#define	REMOTESET_H

#include "IrNamedRemote.h"

class IrNamedRemoteSet {
public:

    IrNamedRemoteSet() : remotes(NULL), noRemotes(0) {
    };

    IrNamedRemoteSet(const IrNamedRemoteSet& orig) : remotes(orig.remotes), noRemotes(orig.noRemotes) {
    };

    IrNamedRemoteSet(const IrNamedRemote** remotes_, uint16_t noRemotes_) : remotes(remotes_), noRemotes(noRemotes_) {
    };

    virtual ~IrNamedRemoteSet() {
    };

    const IrNamedRemote* getIrNamedRemote(const char* name) const;

    const IrNamedRemote* getIrNamedRemote(const String& name) const {
        return getIrNamedRemote(name.c_str());
    };

    const IrNamedRemote** getIrNamedRemotes() const {
        return remotes;
    }

    uint16_t getNoIrNamedRemotes() const {
        return noRemotes;
    }
private:
    const IrNamedRemote** remotes;
    uint16_t noRemotes;
};

#endif	/* REMOTESET_H */
