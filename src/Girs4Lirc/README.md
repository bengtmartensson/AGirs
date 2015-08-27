# Girs4Lirc

This is a version of AGirs, optimized for usage with Lirc, using the
driver "girs", to be published.

## Hardware requirements and configuration
All components are optional. The sketch can be configured for sending
and/or receiving, optionally supporting signalling LEDs and an
I2C-connected LCD display. Sending of non-modulated signals,
e.g. using an RF sender, is supported. Thanks to the "module" command of Girs, the
"girs" driver informs itself about the capacities of the current device, and
informs Lirc in its turn.

Configuration is done by adapting the file config.h, where desirable
components are selected, and parameters are selected. By convention,
it includes a hardware configuration file, where the particular
pinouts is defined. Either use an existing one, or create your own.

A more detailed configuration guide for Girs is in the planning stage.

### Sending
For sending, define TRANSMIT. Connect a suitable IR LED to the sending
pin (pin 9 on Mega2560 and Leonardo/Mini, pin 3 on most others), using
suitable resistor and/or driver transistor.

### Sending non-demodulating signals
If NON_MOD is defined, and a suitable sender is connected  to pin
NON_MOD_PIN, the firmware will be able to send non-modulated
signals. This can be another IR LED, but more interestingly, it can be an
RF sender, used for controlling e.g. remotely controlled power outlets
etc. (Signals can be genated by IrScrutinizer, using e.g. the arctech
protocol.) If (and only if) the frequency of a signal is 0,
the signal is sent to the non modulating receiver at NON_MOD_PIN,
otherwise the normal, modulating, sender will be used.

Current lircd parses remotes having no explicit frequency as having
frequency 0
([ticket](https://sourceforge.net/p/lirc/tickets/132/)). To avoid
problems, make sure that all used remotes have explicit frequencies in
their lircd.conf. (If unsure, just add "frequency 38000" to the
preable of each remote.)

### Receiving
For receiving, define RECEIVE.  Use a demodulating receiver, TSOP*,
the data pin connected to IRSENSOR_1_PIN. (Warning: Different
receivers look similar, but have different pin-outs. Check the data sheet.)

### LEDs
For using attached LEDs, define the symbol LED. The usage of LEDs with
build-resistors is recommended. If desired, define one as COMMANDLED, another as
RECEIVELED, and a third one as TRANSMITLED. If so, the COMMANDLED led will be
lit when the device is listening for commands, the RECEIVELED led will
be lit when the receiver is "listening", and the TRANSMITLED led will
light up when transmission takes place.

### LCD
If the use of an I2C connected LCD display is desired, define
I2C_LCD. This will allow the Lirc driver to send message to it for
display. Also, it is used during startup to signalize the current
firmware version.

As an extra feature, a received signal will be decoded usint the
decoder of Girs. This currently identifies NEC and RC5 signals. The
outcome of the decoding is displayed, but (at least currently) not
sent to Lirc.

## Future development
This is work in development, and may change, including the interface,
at any time (in the master branch).

As Girs is intended to be a general solution, having a separate
version for Lirc is contradictory. Girs4Lirc may therefore in the
future be replaced by some other sketch.

