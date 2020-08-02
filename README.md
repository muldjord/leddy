# Leddy
Leddy displays various information on a Unicorn Hat HD for the Raspberry Pi. I was looking around for a C++ project using the Unicorn Hat HD but didn't find any, so instead I decided to make my own.

It uses the SPI device from the Linux kernel which it communicates with using `ioctl`. The internal display buffers use Qt QImages which can be manipulated in a plethora of ways using the Qt QPainter and even directly by manipulating the bits.

I am currently laying the groundwork for customizable scene and transition handling. As of August 2nd 2020 it's hardcoded to render the time + temperature as *scene 1* and the current weather icon as *scene 2*. Between the scenes it uses transitions that are fully customizable by simply adding some PNG files. Read more about those [here](docs/TRANSITIONS.md).

The weather location and other things can be configured in `config.ini` so be sure to check that out. It uses the OpenWeatherMap API to fetch the weather. And it also currently grabs an RSS feed, but doesn't use it for anything yet. I plan to add rss feeds as scrolling text as a *scene 3*.

In time I also plan to make scenes more customizable, preferably using a scripting language for which information to display on which scene, and where and how they are placed. Stay tuned for that! For now it's quite fun to tinker with the transitions. If you make some cool ones, feel free to PR them, then I'll have a look and maybe add them. :)

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
I like to run Leddy as a service using systemd. This enables it to auto-start when I boot the Raspberry Pi. I've provided a very simple systemd service file you can use to run it as a service. It's located under `systemd/leddy.service`. All you need to do is copy that file to `/etc/systemd/system/leddy.service` and edit the `ExecStart` path and executable to wherever your compiled `Leddy` executable is located. Then run the following to reload the systemd services and enable it upon next reboot:
```
$ sudo systemctl daemon-reload
$ sudo systemctl enable leddy
```
If you don't want to reboot, you can start the service manually with:
```
$ sudo systemctl start leddy
```
And that's it! If you have the Unicorn Hat HD or Ubercorn Hat installed, it should start running on it.
