#ifndef REMOTESET_H
#define	REMOTESET_H

#include "Remote.h"

class RemoteSet {
public:
    //RemoteSet();
    //RemoteSet(const RemoteSet& orig);
    RemoteSet(const Remote** remotes_, uint16_t noRemotes_) : remotes(remotes_), noRemotes(noRemotes_) {};
    virtual ~RemoteSet() {};

    const Remote* getRemote(const char* name) const;
    const Remote* getRemote(const String& name) const {return getRemote(name.c_str()); };
    const Remote** getRemotes() const { return remotes; }
    uint16_t getNoRemotes() const { return noRemotes; }
private:
    const Remote** remotes;
    uint16_t noRemotes;
};

#endif	/* REMOTESET_H */
