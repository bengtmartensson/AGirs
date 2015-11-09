#ifndef REMOTESET_H
#define	REMOTESET_H

#include "IrNamedRemote.h"

class IrNamedRemoteSet {
public:

    IrNamedRemoteSet() : remotes(NULL), noRemotes(0) {
    };

    IrNamedRemoteSet(const IrNamedRemoteSet& orig) : remotes(orig.remotes), noRemotes(orig.noRemotes) {
    };

    IrNamedRemoteSet(const IrNamedRemote* const *remotes_, uint16_t noRemotes_) : remotes(remotes_), noRemotes(noRemotes_) {
    };

    virtual ~IrNamedRemoteSet() {
    };

    const IrNamedRemote* getIrNamedRemote(const char* name) const;

    const IrNamedRemote* const * getIrNamedRemotes() const {
        return remotes;
    }

    uint16_t getNoIrNamedRemotes() const {
        return noRemotes;
    }
private:
    const IrNamedRemote* const * remotes;
    uint16_t noRemotes;
};

#endif	/* REMOTESET_H */
