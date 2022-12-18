# Mixduino Firmare

## Compile and upload

```shell
pio run -t upload
```

## Debug Midi messages

### Linux

With ttymidi

```shell
ttymidi -s /dev/ttyACM0 -v
```

### Windows

With [MidiView](https://hautetechnique.com/midi/midiview/) 
