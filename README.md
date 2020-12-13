# rockpi4-mikroe-506
ASoC driver and device tree overlay for the Mikroe-506 (WM8731 board) on
RockPi4. It has been confirmed to work on the 4.4.154-90 kernel.

## Notes
If you try and build these drivers against the kernel tree shipped by Radxa,
you will get "Exec format" errors. This is caused by the erroneous
cross-compilation of the kernel tree on the x86 architecture. The issue is
described [here](https://forum.radxa.com/t/dkms-building-error/307), and a
workaround is suggested.

Another thing to look out for: the default device tree compiler (dtc) shipped
from Radxa is too old to support overlays. We have confirmed that version 1.6.0
works; the latest version is available
[here](https://git.kernel.org/pub/scm/utils/dtc/dtc.git).

## Build/Install Drivers and Overlay:
```
make
sudo make install
```

## Usage

Connect the Mikroe-506 to the RockPi 4 like this:

```
Mikroe-506        RockPi4
----------        -------
SCK         <-->  Pin 12
MISO        <-->  Pin 38
MOSI        <-->  Pin 40
ADCL        <-->  Pin 36
DACL        <-->  Pin 35
SDA         <-->  Pin 3
SCL         <-->  Pin 5
3.3V        <-->  Pin 1
GND         <-->  Pin 6
```

Then, to enable the overlay, add the following lines to `/boot/hw_intf.conf`:
```
intfc:i2c7=on
intfc:dtoverlay=rockpi-mikroe-506
```

Reboot, and check that the new `sndmikroe506` device appears in the list of
sound cards:

```
aplay -l
```

If that looks good, then configure the sound card parameters for input and
output:

```
alsamixer
```

Press F6, select sndmikroe506, then press F5 for all controls, and:

* enable item 'Mic' (space)
* set item 'Mic Boost' to 100 (up arrow key)
* enable item 'Playback Deemphasis' (m key)
* disable item 'ADC High Pass Filter' (m key)
* set item 'Input Mux' to Mic (arrow keys)
* enable item 'Output Mixer HiFi' (m key)

Escape to exit, now test output:

```
aplay -D hw:2,0 mySong.wav
```

and mic input:

```
arecord -D hw:1,0 -r 48000 -f S16_LE -d 5 -c 2 myRecoring.wav
```

