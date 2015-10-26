#ifndef COMMAND_H
#define	COMMAND_H

#include "Renderer.h"

class Command {
public:
    Command(const char* name_) : name(name_) {};
    virtual ~Command() {};
    virtual const Renderer* getRenderer() const = 0;
    const char* getName() const { return name; }
private:
    const char *name;
};

#endif	/* COMMAND_H */
