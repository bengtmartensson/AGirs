#ifndef REMOTE_H
#define	REMOTE_H

#include "Command.h"

class Remote {
public:
    //Remote();
    //Remote(const Remote& orig);
    virtual ~Remote() {};

    Remote(const char* name_, const Command** commands_, uint16_t length_)
       : name(name_), commands(commands_), noCommands(length_) {
    }
    const char* getName() const { return name; }
    const Command* getCommand(const char *name) const;
    const Command* getCommand(const String& name) const { return getCommand(name.c_str()); };
    const Command** getCommands() const { return commands; };
    uint16_t getNoCommands() const { return noCommands; }

private:
    const char* name;
    const Command **commands;
    uint16_t noCommands;
};

#endif	/* REMOTE_H */

