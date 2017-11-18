#include "NamedNec1Command.h"
#include "IrNamedRemote.h"

#ifdef ARDUINO
#include <IrSenderPwm.h>
#else
#include <IrSenderSimulator.h>
#endif

const IrNamedCommand* yamaha_cmds[] = {
    new NamedNec1Command("volume_up",    122, 26),
    new NamedNec1Command("volume_down",  122, 27),
    new NamedNec1Command("power_on",     122, 29),
    new NamedNec1Command("power_off",    122, 30)
};

IrNamedRemote irRemote("yamaha", yamaha_cmds, sizeof (yamaha_cmds) / sizeof (NamedNec1Command*));

IrSender *irSender;
#ifdef ARDUINO
Stream& stream = Serial;
#else
Stream stream(std::cout);
#endif

void setup() {
#ifdef ARDUINO
    Serial.begin(9600);
    Serial.setTimeout(10000);
    irSender = IrSenderPwm::getInstance(true);
#else
    irSender = new IrSenderSimulator(stream);
#endif
    stream.println("Existing commands:");
    for (unsigned int i = 0; i < irRemote.getNoCommands(); i++) {
        stream.print(" ");
        stream.print(irRemote.getAllCommands()[i]->getName());
    }
    stream.println();
}

void loop() {
    stream.println("enter a command");
    String name;
#ifdef ARDUINO
    name = Serial.readStringUntil('\r');
#else
    std::cin >> name;
#endif
    const IrNamedCommand *irCommand = irRemote.getIrNamedCommand(name);
    if (irCommand == NULL)
        stream.println("Nonexisting command");
    else
        irSender->sendSignal(*irCommand, 2);
}

#ifndef ARDUINO
int main() {
    setup();
    while (true)
        loop();
}

#endif
