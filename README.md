# AGirs (formerly ArduinoGirs)
This project implements a [Girs
server](http://www.harctoolbox.org/Girs.html) for the Arduino. That
is, it is an interactive server that can send and receive IR signals,
with some bells and whistles. The interactive server is mainly meant
for interact with other programs. communicating over a serial line
(likely in USB disguise), or TCP-sockets.

It is build on top of the low-level library called [Infrared4Arduino](https://github.com/bengtmartensson/Infrared4Arduino).

The main content of the project is an applications,
_Girs_ (including the "light" version _GirsLite_), which constitutes the server.
There is also another application, _Listener_, which is a uni-directional program that just
emits decodes on the serial interface.
(It can for example to be used in conjunction with my Java program
[dispatcher](https://github.com/bengtmartensson/dispatcher)).
Finally, there is a IR-to-serial demonstration program; an IR to serial converter (_Opponator_).
These application are found as subdirectories of the directory `examples`.
The subdirectory `src` contains a fairly small amount of supporting functionality,
forming a library in the traditional Arduino sense.

This slighty unlogical organization is due to the wish to be compatible with the current
[Arduino library specification](https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification),
required/desired for inclusion in the Arduino library manager.
(There is presently no "Arduino application manager".)

For compilation with the Arduino IDE, the library
[Infrared4Arduino](https://github.com/bengtmartensson/Infrared4Arduino) and, preferably,
the [LiquidCrystal_I2C](https://github.com/marcoschwartz/LiquidCrystal_I2C) (version 1.1.2 or later)
should be installed with the Arduino library manager.

## Configuration files
It is a modular program that is heavily based on CPP symbols, defined
in the configuration file `examples/Girs/config.h`. This determines the capacities of the
compiled program, and adapts the configuration to the underlying
hardware. The options are (somewhat) documented in `examples/Girs/GirsFat.config.h`.
Not all combination are sensible or implemented. Some, but not all,
of the non-sensible
combinations will be detected and will generate a compilation error.

If the preprocessor symbol `LCD` is defined in `src/GirsLib/LedLcdManager.cpp`
(which is the default, except for the Arduino Micro), the library is configured
with support for the LCD display, regardless of the settings in `config.h`.

## Modules and interactive commands

### General
The program takes commands from its input stream, and write responses on its output stream.
The general concept is presented [here](http://www.harctoolbox.org/Girs.html).
A command input consists of one single line, terminated with a carrage return (binary 0x0d).
Output is also presented as a single line, terminated with a carrage return.
Needless to say, these lines can be long. On a successful execution, typically "OK"
is returned.

The communication can take place with another program or with a human using a
version of a serial terminal. The command names can, in this implementation,
be shortened as long as they are unique. Generally speaking, error handling
and plausibility checking are presently basically non-existent...

The program runs in a single thread, with no multitasking whatsoever.
Also, even in the Ethernet version, only one concurrent session is supported.

### [Base commands](http://www.harctoolbox.org/Girs.html#Base)
These commands are always present in a Girs server. They correspond to the `Base` module.
#### version
*Input*: `version`

*Output*: The program name followed by the version string. Example: `AGirs 1.0.0`.

#### modules
*Input*: `modules`

*Output*: Returns list of implemented modules, separated by whitespace.
Example: `Base Transmit Capture Renderer Receive Decoder Led Lcd Parameters`.

Comment: A communicating peer evaluates this, in order to find out the capabilities of
the Girs implementation. Both IrScrutinizer and the Lirc Girs driver do this, and adjust
their behavior accordingly.

### [NamedRemotes](http://www.harctoolbox.org/Girs.html#NamedRemotes)
Presently not implemented, but is [planned](https://github.com/bengtmartensson/AGirs/issues/37).

### [Transmit](http://www.harctoolbox.org/Girs.html#Transmit)
Supported if the CPP symbol `TRANSMIT` is defined in the configuration.
#### transmit
*Input*: `send <no_sends> <frequency> <length_intro> <length_repeat>  <durations...>`

*Output*: `OK`

Semantics:
* `<no_sends>` denotes the number of times to send the signal.
* `<frequency>` denotes the modulation frequency in Hz (not kHz as in some other programs!). The value 0
is treated as "no modulaton", and sent to a non-modulating sender, if configured.
* `<length_intro>` denotes how many of the numbers supplied as `<durations...>`
are to be considered as belonging to the intro sequence,
* `<length_repeat>` denotes how many of the numbers supplied as `<durations...>`
are be considered as belonging to the repeat sequence (following the intro sequence durations),
* `<length_ending>` (normally 0) denotes how many of the numbers supplied as `<durations...>`
are to be considered as belonging to the ending sequence, following after the repeat durations,
* `<durations>` the durations in micro seconds, making up the intro-, repeat-, and ending sequence,
according to the preceding parameters.

The intro sequence is always sent exactly one. If it is empty, the repeat sequence is sent `<no_sends>` times,
otherwise `<no_sends>` - 1 times. Finally, the (normally empty) ending sequence is sent once.

### [Capture](http://www.harctoolbox.org/Girs.html#Capture)
Available if configured with the `CAPTURE` option.

*Input*: `analyze`

*Output* (Normal): `f=<frequency> <durations with signs...>`

*Output* (Timeout): `.`

Semantics:
* `<frequency>` is the measured modulation frequency.
* `<durations with signs...>` the measured durations in micro seconds; flashes with "+",
gaps as "-".

### [Receive](http://www.harctoolbox.org/Girs.html#Receive)
Available if configured with the `RECEIVE` option.

*Input*: `analyze`

*Output* (Normal): `<durations with signs...>

*Output* (Timeout): `.`

Semantics:
* `<durations with signs...>` the measured durations in micro seconds; flashes with "+",
gaps as "-".

### Render
Available if configured with the RENDER option.

*Input*: `transmit <no_sends> <protocol> <protocol_parameters>`

*Output* (normal): `OK`

*Output* (Syntax error): `<error message>`

Semantics:
* `<no_sends>` number of times to send signal
* `<protocol_name>` name of protocol, presently `NEC1` and `RC5`-
* `<protocol_parameters>` protocol parameters, dependent on protocol.

*Example*: `transmit 1 rc5 0 12`.
This turns on or off most Philips TVs.

### Decoder
Available if configured with the `DECODER` option. No additional interactive commands,
instead the `receive`-d commands are attempted to be decoded, and optionally
display on an LCD display.

### LED
Available if configured with the `LED` option.

*Input*: `led <logical_led_nr> on|off|blink`

*Output*: `OK`.

Turns on/off/blinks the LED with the given logical number.

### LCD
Available if configured with the `LCD` option.

*Input*: `lcd <message>`

*Output*: `OK`

Shows the message `<message>` on a connected LCD display for a certain time.

### Parameters
Available if configured with the `PARAMETERS` option. The allows to inspect and
to change some parameters.

*Input*: `parameter <parameter_name> [<parameter_value>]`

*Output:* `<parameter_name>=<parameter_value>`

Semantics: If `<parameter_value>` is given, the value is assigned to the
named parameter, if possible. If not given, the present value is reported.

#### Named of available adjustable parameters:
The adjustable parameters depend on the configuration options.
A few of these are listed next.

* `beginTimeout`: time in milliseconds until timeout by not detected signal
* `captureEndingTimeout`: time in milliseconds for capture ending marking end of signal
* `receiveEndingTimeout`: time in milliseconds for receive ending marking end of signal
* `captureSize`: size, in number of durations, of captured (received) signals. Must be even.

### Pronto
Available if configured with the `PRONTO` option.

*Input*: `hex <no_sends> <pronto_hex_signal>`

*Output*: `OK`.

Semantics:
* `<no_sends>` number of times to send signal
* `<pronto_hex_signal>` Pronto hex form IR signal. Always start with `0000`.

Sends the given signal the requested number of times.

### Info
Available if configured with the option `INFO`.

*Input*: info

*Output*: Information on current platform;
for example `Board: Arduino Mega2560, CPU frequency: 16000000L`

### Memory
Available if configured with the option `FREEMEM`.

*Input*: memory

*Output*: number of free bytes.

### Ethernet support
If using an Ethernet shield with W5500 or W5200 chip, be sure that the
Ethernet library version 2.0.0 or later is used,
since it supports all W5*00 chips, auto-detecting during runtime.

This applies both to the Girs application as well as to the Listener.

## Hardware configuration
I have written a [fairly detailed description](http://www.harctoolbox.org/arduino_nano.html)
of using an Arduino Nano (clone) by soldering suitable components to the PCB. This is
a small handy gadget at the size of a (large) stamp and the price of a (small) pizza,
perfect for using with IrScrutinizer and Lirc.

The hardware configuration is determined by including a suitable
header file. It describes the attach sensor(s) and the pins
they are connected to. To allow soldering sensors directly to the
holes in some boards, the program supports defining e.g. `SENSOR_GND`
and `SENSOR_VSS`, which will make the program define these pins as
digital outputs, being fed by constant 0 and 5 volts respectively.

Note that the sending pin and the capture pin
(as opposed to the receive pin) are not configurable, but has to
follow the following table:

<pre>
                          Sender Pin      Capture Pin
Uno/Nano (ATmega328P)          3             8
Leonardo/Micro (ATmega32U4)    9             4
Mega2560 (ATmega2560)          9            49
</pre>

## Sending non-modulated signals.
RF signals (433 MHz and other carrier frequencies) do not use the IR
typical modulation. Also there are a few IR protocols (like [Revox, Barco,
Archer](http://www.hifi-remote.com/forums/viewtopic.php?t=14186&start=40))
not using modulation. These signals can be sent by defining the symbol
`NON_MOD`, and connecting
some hardware capable of sending non-modulated signals (IR- _or_ RF-)
to the GPIO pin defined as `NON_MOD_PIN`. Then transmitted signals
having frequency 0 will be directed to that device. (Later versions
may use different syntax and semantic.)

## Testing
The flashed unit can be tested with a standard terminal program, like the
serial monitor of the Arduino IDE. For this, set the baud rate to 115200, and
the line ending to carriage return. It is now possible to communicate
with the unit using the [commands of
Girs](http://www.harctoolbox.org/Girs.html). Just type the command to
the program, and the unit will respond.
Exactly which commands are
available depends on the configuration. In all cases, the
`version` and the `modules` commands are
available. If `receive` is implemented, just type
"`r`" (without the
quotes), followed by return, and fire a suitable IR signal at the
receiver. The raw capture will be output to the terminal program. Using
the clipboard, it can be pasted to IrScrutinizer, and analyzed. Of course, also
the other commands can be tested in this way.

## API

[Up-to-date API, generated by Doxygen](http://bengtmartensson.github.io/AGirs).

## Dependencies

* [`Infrared4Arduino`](https://github.com/bengtmartensson/Infrared4Arduino) by myself.
  Current version is 1.0.1. Can be installed by the library manager within the Arduino IDE.
  (Sketch -> Include library -> Manage libraries, name Infrared (Category: Others)).
* `Ethernet`, preferably version 2.0.0 or later.
* `SPI` (if enabling the `ETHERNET` or `LCD_I2C` configure option). Contained in the Arduino IDE.
* `Wire` (if enabling the `LCD_I2C` configure option). Contained in the Arduino IDE.
* [`LiquidCrystal_I2C`](https://github.com/marcoschwartz/LiquidCrystal_I2C) version 1.1.2 or later.
 Available in the Arduino library manager.
If the preprocessor symbol `LCD` is defined in `src/GirsLib/LedLcdManager.cpp`,
this is needed also if not actually using an LCD display.
* (Optional) [`Beacon`](https://github.com/bengtmartensson/ABeacon) for an AMX compatible Ethernet beacon.
 It is also available in the Arduino library manager with the name `Beacon`.

## Questions and answers

* How do I setup Lirc to use this?

Use the `girs` driver contained in the recent official upstream Lirc distribution.
This is described [here](http://lirc.org/html/girs.html).
also contained in the (recent) distro as `girs.html`.
Configuration options: use TRANSMIT if sending is desired, and RECEIVE if receiving is desired.
CAPTURE is not meaningful for Lirc. Turn on PARAMETERS -- Lircd adjust the ending timeout.
If using DECODE and an LCD display (which is cool, but useless), be sure to
define DONT_REPORT_DECODES. Use of Ethernet connections are supported.
Due to the inner workings of Lirc, it is advantageous not define both TRANSMIT and RECEIVE
unless you really need it.

* What are Makefiles doing in an Arduino project?

To build the project for the Arduino, use the Arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software),
as in most Arduino projects.
The Makefile is used for building a "Software-in-the-loop" test version for the PC;
of interest for developers only.
It is also used for maintainer work, like generating API documentation with Doxygen,
as well as generating keywords.txt. Feel free to learn from it!

* What is the difference between
  ["receive"](http://www.harctoolbox.org/Glossary.html#ReceivingIrSignals)
  and   ["capture"](http://www.harctoolbox.org/Glossary.html#Capturing)?

Please follow the links given. Differently put, "receive" uses a
demodulating receiver, "capture" a non-demodulating decoder. Note that
this is not universally accepted terminology (yet!).

* What Ethernet modules are supported?

Only cards based on the W5100, W5200, and W5500 chip (and compatible), like the
[official
shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield).  There are both cheap clones of the original available, as well as
smallish W5100-based cards.

Also the next generation of W5500 based shields, like the
official [Arduino Ethernet Shield 2](https://www.arduino.cc/en/Main/ArduinoEthernetShield)
work. For this, use the `Ethernet` library version 2.0.0 (or later).

* What about "GirsLite"?

As indicated by the
name, it is a minimalist Girs server
for the Arduino, that implements only the
[capture](http://www.harctoolbox.org/Girs.html#Capture) and the
[transmit](http://www.harctoolbox.org/Girs.html#Transmit) modules,
without all options. It is meant to be used with
[IrScrutinizer](http://www.harctoolbox.org/IrScrutinizer.html)
versions 1.1.0 or later, as well as with Lirc, using  the Lirc
`girs` driver by yours truly. Documentation is found with the [Lirc
driver](http://lirc.org/html/girs), in the Lirc sources the file `girs.html`.

It is not an independent program, it is just AGirs
with certain options enabled, namely
the CPP symbols `TRANSMIT, CAPTURE, LED`, and (optionally) `NON_MOD`
defined. Alternatively, if `RECEIVE` is defined, but not `CAPTURE`, the
program mimics the capture command with a demodulating sensor, for
usage with IrScrutinizer without a non-demodulating sensor.

* Were did the `example/GirsLite` directory go?

It has been merged with `examples/Girs`. To build GirsLite, make sure that `examples/config.h`
is configured as you desire.

* What about Girs4Lirc?

_Now discontinued_, replaced by GirsLite.
Just as GirsLite, this was just a certain configuration of AGirs,
"optimized" for Lirc, supporting `TRANSMIT,
NON_MOD` (optionally), `RECEIVE, LED, LCD, DECODE` (only to the LCD), `TRANSMITTERS`
(only a dummy implementation).

* Can I keep and maintain the library configuration in another location, for example together with
my own sketch?

No, the present Arduino IDE does not support this.
I an not aware of a clean solution. Sorry.
Fiddling with the library's `config.h` for project specific configurations defeats the very idea of a library.

* How is "Girs" pronounced?

It is pronounced like in "girl". The "language" Girs is written capitalized, the name of an implementation is usually written in lower case.

## License
The entire work is licensed under the GPL2 "or later" license, just as Infrared4Arduino. Michael's code
(that is contained in Infrared4Arduino) carries the
GPL2-license, although he is [willing to agree to "or later
versions"](http://www.hifi-remote.com/forums/viewtopic.php?p=112586#112586).
