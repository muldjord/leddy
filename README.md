# Leddy
Leddy displays various information on a Unicorn Hat HD for the Raspberry Pi. I was looking around for a C++ project using the Unicorn Hat HD but didn't find any. So instead I decided to make my own. It uses the SPI device from the Linux kernel which it communicates with using `ioctl`. The internal display buffers use Qt QImages which can be manipulated in a plethora of ways using the Qt QPainter and even directly by manipulating the bits. Since the pixel format is simply triplets of RGB 8-bit values, it's very easy to even manipulate them by hand.

I am currently laying the groundwork for scene and transition handling. As of August 2nd 2020 it's hardcoded to render the time + temperature as *scene 1* and the current weather icon as *scene 2* with 10 second delay between switches. The weather location and other things can be configured in `config.ini` so be sure to check that out.

In time I plan to make both scenes and transitions more customizable, preferably using a scripting language and sprite sheets where specific color values will signify the scene you are transitioning from and to - sort of like chroma-keying in movies.

It'll be fun! :)

## Prerequisites

### Hardware
* Raspberry Pi
* Unicorn Hat HD or Ubercorn hat (they are the same, just very different sizes)

### Software
You need to enable the Raspberry Pi SPI interface. This can be done in various simple ways. Read about it [here](https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md).

You also need to install Qt5:
```
$ sudo apt-get update
$ sudo apt-get install build-essential qt5-default
```

Note! You need C++14 to compile Leddy. If you use the latest version of Raspbian it already has it after installing the above prerequisites.

## Compile
```
$ qmake
$ make
```

## Running
```
$ ./Leddy
```

## Running as a service
I like to run Leddy as a service using systemd. This enables it to auto-start when I boot the Raspberry Pi. I've provided a very simple systemd service file you can use to run it as a service. It's located under `systemd/leddy.service`. All you need to do is copy the file to `/etc/systemd/system/leddy.service` and edit the `execStart` path and executable to wherever your compiled `Leddy` executable is located. Then run the following to enable it upon next reboot.
```
$ sudo systemctl enable leddy
```
If you don't want to reboot, you can start the service manually with:
```
$ sudo systemctl start leddy
```
And that's it. If you have the Unicorn Hat HD or Ubercorn Hat installed, it should start running on it.
