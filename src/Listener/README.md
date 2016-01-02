# Listerner

This sketch is designed to work with the Java program [dispatcher](https://github.com/bengtmartensson/dispatcher),
but can be used also in other contexts. It runs in an infinite loop, collecting measuremenets
with a demodulating IR receiver (e.g. TSOP-*). If an LCD display is connected, the decodes (if successful)
are written to it. Also, if LEDs are connected, a LED, dependent on the decode type, is lit.
