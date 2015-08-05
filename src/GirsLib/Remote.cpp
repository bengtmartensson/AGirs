#include "Remote.h"

//Remote::Remote() {
//}

//Remote::Remote(const Remote& orig) {
//}
//uint16_t Remote::noRemotes = 0;
//Remote* Remote::remotes[maxRemotes] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // FIXME

//Remote::Remote(const char* name_,  const Command** commands_, uint16_t length_) : name(name_), commands(commands_), noCommands(length_) {
//    remotes[noRemotes++] = this;
//}

//Remote::~Remote() {
//}


const Command* Remote::getCommand(const char* name) const {
    for (unsigned int i = 0; i < noCommands; i++) {
        for (int j = 0; ; j++) {
            if (name[j] != commands[i]->getName()[j])
                break;
            if (name[j] == '\0')
                return commands[i];
        }
    }
    return NULL;
}
