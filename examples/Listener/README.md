# Listerner

This is a quite a dumb program. It just runs in an infinite loop, receiving IR signals,
and outputs either their decodes or the raw signal,
to either the serial console, or a UDP socket.

if the preprocessor symbol `DECODE` is defined, the program tries do decode the
received signals (presently, only NEC1 and RC5 protocols are supported), otherwise the raw signal is output.

It uses a demodulating IR receiver (e.g. TSOP-*).
If an LCD display is connected, the decodes
are written to it. Also, LEDs can be connected, and when a signal is successfully decoded,
a LED, dependent on the decode type, is lit. (Using a "multicolored LED" for this is nice.)

This program is intended to work as a gateway;
to send events to a home automation systems using a cheap IR remote, as an alternative
to, for example, a smartphone app. A NEC1 remote is recommended.

For example, the simple Java program [dispatcher](https://github.com/bengtmartensson/dispatcher)
can be used to receive and dispatch the events.

To configure, just edit config.h. See the build instructions in top level README.md.

Strictly speaking, this program is not "Girs". However, it uses the code of AGirs (and thus, indirectly, Infrared4Arduino).
