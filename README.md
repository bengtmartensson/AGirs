# AGirs (formerly ArduinoGirs)
This project implements a [Girs
server](http://www.harctoolbox.org/Girs.html) for the Arduino. That
is, it is an interactive server that can send and receive IR signals,
with some bells and whistles. The interactive server is mainly meant
for interact with other programs. communicating over a serial line
(likely in USB disguise), TCP-, or UDP sockets, acting as server
(accepting incoming requests) or client (initiating connections).

It implements some of the functionallity of [Chris Young's
IRLib](http://tech.cyborg5.com/irlib/, 
https://github.com/cyborg5/IRLib), which is a major
rewrite of a previous library called IRremote, published by
Ken Shirriff in [his blog](http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html), but does not use it directly. 
It uses Michael Dreher's
IrWidget [(article in
German)](http://www.mikrocontroller.net/articles/High-Speed_capture_mit_ATmega_Timer),
see also [this forum
contribution](http://www.hifi-remote.com/forums/viewtopic.php?p=111876#111876).
Michael's code is included, in somewhat modified form, in the files
IrWidget.[cpp,h] and IrWidgetAggregating.[cpp,h].

The project contains a library, contained in the directory GirsLib,
and two applications: Girs and Listener. The directory GirsLib should
be copied to 
the library area, typically <b>$HOME/Arduino/libraries/GirsLib</b>, while the
application directories can be processed by the Arduino IDE
directly. The directory Girr contains the AGirs application
interactive program. Listener is a uni-directional program that just
emits decodes on the serial interface. It is intended to be used in
conjunction with my Java program
[dispatcher](https://github.com/bengtmartensson/dispatcher). (It may
possibly be discontinued in the future.)

## Configuration
It is a modular program that is heavily based on CPP symbols, defined
in the configuration file __config.h__. This determinines the capacities of the
compiled program, and adapts the configuration to the underlying
hardware. The options are (somewhat) documented in config.h.
Not all combination are sensible or implemented. Some of the non-sensible
combinations will be detected and will generate a compilation error.

## Hardware configuration
The hardware configuration is determined by including a suitable
header file. It describes the attach sensor(s) and the pins
they are connected to. To allow soldering sensors directly to the
holes in some boards, the program supports defining e.g. SENSOR_GND
and SENSOR_VSS, which will make the program define these pins as
digital outputs, being fed by constant 0 and 5 volts respectivelly.

Note that the sending pin and the capure pin
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
__NON_MOD__, and connecting
some hardware capable of sending non-modulated signals (IR- _or_ RF-)
to the GPIO pin defined as NON_MOD_PIN. Then transmitted signals
having frequency 0 will be directed to that device. (Later versions
may use different syntax and semantic.)

## Dependencies

* Ethernet (if enabling the ETHERNET configure option). Contanied in the Arduino IDE. 
* SPI (if enabling the ETHERNET or LCD_I2C configure option). Contanied in the Arduino IDE. 
* Wire (if enabling the LCD_I2C configure option). Contanied in the Arduino IDE.
* LiquidCrystal (if enabling the LCD_4BIT option, i.e. connecting an
LCD display directly). Contanied in the Arduino IDE. 
* [Arduino-LiquidCrystal-I2C](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library.git)
Needed if defining LCD_I2C, i.e. connecting an LCD display with an I2C interface card.

## Questions and answers

* What is the difference between
  ["receive"](http://www.harctoolbox.org/Glossary.html#ReceivingIrSignals)
  and   ["capture"](http://www.harctoolbox.org/Glossary.html#Capturing)?

Please follow the links given. Differently put, "receive" uses a
demodulating receiver, "capure" a non-demodulating decoder. Note that
this is not universally accepted terminology (yet!).

* Are ENC28J60 Ethernet modules supported?

No, not presently, just cards based on the W5100 chip, like the
[official
shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield). (Note
that there are both cleap clones of the orginal available, as well as
smallish W5100-based cards, for prices just slightly higher than a
ENC28J60 module. The W5100 is the better chip anyhow.) I did some test
with the [Arduino uip
library](https://github.com/ntruchsess/arduino_uip), but it was
just too memory hungry on an AtMega328. Contributed code is welcome.

* What about "GirsLite".

"GirsLite" is a precessor to the present program. It is a minimalistic
(thereby the name) Girs server
for the Arduino, that implements only the
[capture](http://www.harctoolbox.org/Girs.html#Capture) and the
[transmit](http://www.harctoolbox.org/Girs.html#Transmit) modules,
without all options. This was meant to be used with
[IrScrutinizer](http://www.harctoolbox.org/IrScrutinizer.html)
versions 1.1.*. It is essentially equivalent to the present program
with only the CPP symbols TRANSMIT and CAPTURE defined.

## License
The entire work is licensed under the GPL3 license. Chris' as well as Ken's
code is licensed under the LGPL 2.1-license. Michael's code carries the
GPL2-license, although he is [willing to agree to "or later 
versions"](http://www.hifi-remote.com/forums/viewtopic.php?p=112586#112586).
