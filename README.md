# rockpi4-mikroe-506
ASoC driver and device tree overlay for the Mikroe-506 (WM8731 board) on
RockPi4. It has been confirmed to work on the 4.4.154-90 kernel.

## Notes
If you try and build these drivers against the kernel tree shipped by Radxa,
you will get "Exec format" errors. This is caused by the erroneous
cross-compilation of the kernel tree on the x86 architecture. The issue is
describere [here](https://forum.radxa.com/t/dkms-building-error/307), and a
workaround is suggested.

Another thing to look out for: the default device tree compiler (dtc) shipped
from Radxa is too old to support overlays. We have confirmed that version 1.6.0
works; the latest version is available
[here](https://git.kernel.org/pub/scm/utils/dtc/dtc.git).

## Build Instructions
```
make
sudo make install
```

## Usage
To enable the overlay, add the following line to `/boot/hw_intf.conf`:
```
intfc:dtoverlay=rockpi-mikroe-506
```
