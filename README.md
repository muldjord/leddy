# Leddy
Displays various information on a Unicorn Hat HD for the Raspberry Pi. I was looking around for a C++ project using the Unicorn Hat HD but didn't find any. So instead I decided to make my own. It uses the SPI device from the Linux kernel which it communicates with using `ioctl`. The internal display buffer is a Qt QImage which can be manipulated in a plethora of ways using the Qt QPainter and even directly by manipulating the bits. Since the pixel format is simply triples of RGB 8-bit values, it's very easy to even manipulate them by hand. When the buffer has been updated, simply run the `SPIConn->refresh()` and it will push the buffer to the Unicorn Hat HD.

[Here's a small video](https://youtu.be/2UuTr3zGO34) demonstrating the current status of the project. I plan to expand this greatly. It will have 'scenes' that will show different information. Clock, weather, rss feed scroll. And I plan to have bitmap transition animation between those scenes. It'll be fun! :)

## Prerequisites

### Hardware
* Raspberry Pi
* Unicorn Hat HD

### Software
You need to enable the Raspberry Pi SPI interface. This can be done in various simple ways. Read about it [here](https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md).

You also need to install Qt5:
```
$ sudo apt-get update
$ sudo apt-get install build-essential qt5-default
```

## Compile
```
$ qmake
$ make
```

## Running
```
$ ./Leddy
```
