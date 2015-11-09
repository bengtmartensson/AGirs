// Defines a IrNamedRemoteSet remoteSet with commands to be used.

// This file is just to be an example.

#include "config.h" // to pick up NAMED_COMMANDS

#ifdef NAMED_COMMANDS

#include "IrNamedCommand.h"
#include "IrNamedRemote.h"
#include "IrNamedRemoteSet.h"
#include "NamedNec1Command.h"
#include "NamedRc5Command.h"

static const NamedNec1Command yamaha_volume_up("volume_up",    122, 26);
static const NamedNec1Command yamaha_volume_down("volume_down",  122, 27);
static const NamedNec1Command yamaha_power_on("power_on",     122, 29);
static const NamedNec1Command yamaha_power_off("power_off",    122, 30);

static const IrNamedCommand* const yamaha_cmds[] = {
    &yamaha_volume_up,
    &yamaha_volume_down,
    &yamaha_power_on,
    &yamaha_power_off,
};

static const IrNamedRemote yamaha_remote("yamaha", yamaha_cmds, sizeof (yamaha_cmds) / sizeof (IrNamedCommand*));

static const NamedRc5Command tv_0("0", 0, 0);
static const NamedRc5Command tv_1("1", 0, 1);
static const NamedRc5Command tv_2("2", 0, 2);
static const NamedRc5Command tv_3("3", 0, 3);
static const NamedRc5Command tv_4("4", 0, 4);
static const NamedRc5Command tv_5("5", 0, 5);
static const NamedRc5Command tv_6("6", 0, 6);
static const NamedRc5Command tv_7("7", 0, 7);
static const NamedRc5Command tv_8("8", 0, 8);
static const NamedRc5Command tv_9("9", 0, 9);
static const NamedRc5Command tv_power_toggle("power_toggle", 0, 12);

static const IrNamedCommand* const tv_cmds[] = {
    &tv_0,
    &tv_1,
    &tv_2,
    &tv_3,
    &tv_4,
    &tv_5,
    &tv_6,
    &tv_7,
    &tv_8,
    &tv_9,
    &tv_power_toggle,
};

static const IrNamedRemote tv_remote("tv", tv_cmds, sizeof (tv_cmds) / sizeof (IrNamedCommand*));

static const IrNamedRemote* const remotes[] = {
    &yamaha_remote,
    &tv_remote
};

// The only exported variable
IrNamedRemoteSet remoteSet(remotes, sizeof (remotes) / sizeof (IrNamedRemote*));

#endif // NAMED_REMOTES
