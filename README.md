# cocoencode

A simple FSK coder that can translate Tandy TRS-80 Color Computer cassette images in .cas format, to an unsigned 8-bit PCM waveform at 11,025Hz sample rate. It is being used in the FujiNet project to provide a way to convert CAS files to a format that the ESP32 DAC can output directly.

## usage

```sh
cat source.cas | cocoencode >dest.raw
```

## license

GPL v. 3, see LICENSE.md for details

## author

Thomas Cherryhomes &lt;thom dot cherryhomes at gmail dot com&gt;

