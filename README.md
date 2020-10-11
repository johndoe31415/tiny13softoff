# tiny13softoff
This is a generic implementation for a soft-off circuit I've created. It runs
an ATtiny13 at the lowest possible current to minimize power dissipation and
essentially only is a smart timer circuit. To achieve this, it runs at the
lowest possible internal clock speed, 500 Hz kHz (128 kHz with a clock division
factor of 1/256), uses power saving modes and disables all unneeded digital
input buffers. In this configuration, it runs at about 75µA at 5V and at about
50µA at 2.7V.

The intent is to not have to recompile the binary every time, but to have one
stable version of which a Python script can parameterize the binary as needed
in the field.

## Parameterization
Use the `edit_binary` script to edit the binary as you want. First, you can
dump the configuration block as JSON:

```
$ cp binary/main.bin .
$ ./edit_binary
{
    "shutoff_after_secs": 10,
    "ext_pin": {
        "function": "LevelResetTimer",
        "pullup": true,
        "active_low": true
    },
    "aux_pin": {
        "function": "Disabled",
        "pullup": true,
        "active_low": false,
        "deadtime_ticks": 0
    }
}
```

Then write this to a JSON file:

```
$ ./edit_binary >conf.json
```

Edit the JSON file to your liking (e.g., increase the shutoff delay to 3600
seconds or one hour), then:

```
$ ./edit_binary conf.json
{
    "shutoff_after_secs": 3600,
    "ext_pin": {
        "function": "LevelResetTimer",
        "pullup": true,
        "active_low": true
    },
    "aux_pin": {
        "function": "Disabled",
        "pullup": true,
        "active_low": false,
        "deadtime_ticks": 0
    }
}
```

The binary has been modified accordingly and can be flashed on the IC.

## Schematic
The schematic has been drawn with Altium CircuitMaker and [is also available at
Altium Vault](https://workspace.circuitmaker.com/Projects/Details/johndoe31415/softoff).
Fuses of the ATtiny13 need to be programmed in the following way:

```-U lfuse:w:0x6b:m -U hfuse:w:0xff:m```

## License
GNU GPL-3.
