# tiny13softoff
This is a generic implementation for a soft-off circuit I've created. It runs
an ATtiny13 at the lowest possible current to minimize power dissipation and
essentially only is a smart timer circuit. To achieve this, it runs at the
lowest possible internal clock speed, 500 Hz kHz (128 kHz with a clock division
factor of 1/256), uses power saving modes and disables all unneeded digital
input buffers. In this configuration, it runs at about 75µA at 5V and at about
50µA at 2.7V.

## Schematic
The schematic has been drawn with Altium CircuitMaker and [is also available at
Altium Vault](https://workspace.circuitmaker.com/Projects/Details/johndoe31415/softoff).
Fuses of the ATtiny13 need to be programmed in the following way:

```-U lfuse:w:0x6b:m -U hfuse:w:0xff:m```

## License
GNU GPL-3.
