# Leddy
Leddy displays easily customizable graphics and information on a Unicorn Hat HD for the Raspberry Pi. The project is currently under heavy development but already has some really cool features.

The scene rotation and look of each scene is easily customizable through just one XML file per [theme](docs/THEMES.md). [Themes](docs/THEMES.md) are, oddly enough, located in subfolders under `themes`. A default [theme](docs/THEMES.md) is provided with the software to get you started in no time.

The project is currently under heavy development and already has a lot of cool features. New scene types will be added as I come up with them. As of August 8th 2020 it supports fully customizable:
- Animations (PNG sprite sheet or GIF)
- Chroma-keyed transitions (PNG sprite sheet or GIF)
- Weather (using OpenWeatherMap API)
- Time / Date
- Rss feed titles as scrolling text

The main configuration of Leddy is done through the `config.ini` which is created when you run Leddy for the first time. Most things in there should be pretty self-explanatory.

But the fun stuff happens in the `themes` subfolder. In here you can create your own [theme](docs/THEMES.md) for use with the LED matrix. I recommend simply copying the `themes/default` [theme](docs/THEMES.md) and rename it to whatever you like. To use your [theme](docs/THEMES.md) simply set it in `config.ini`:
```
[theme]
path=themes/YOURTHEME
```
## Video demonstration
A small video demonstrating the current features of Leddy can be seen [here](https://youtu.be/k52cSKUpY_k). I'm currently using the LED diffuser that came with the Unicorn Hat HD, which does an ok job, but doesn't make the LED's *pixel square*. This will soon change when I built a frame and diffuser for my Ubercorn Hat instead. It'll look *a lot* better. Diffusers are pretty easy to built. It's just a matter of separating each LED away from each other with some sort of plastic matrix frame with a diffuser material in front of it. I'm gonna built the one described [here](https://pinshape.com/items/48046-3d-printed-pimoroni-ubercorn-game-frame).

## Prerequisites

### Hardware
* Raspberry Pi
* Unicorn Hat HD or Ubercorn hat (they are the same, just very different sizes)

### Software
You need to enable the Raspberry Pi SPI interface. This can be done in various simple ways. Read about it [here](https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md).

You also need to install Qt5 and libgif:
```
$ sudo apt-get update
$ sudo apt-get install build-essential qt5-default libgif-dev
```
Note! You need C++14 to compile Leddy. If you use the latest version of Raspberry Pi OS (previously named Raspbian) it already has it after installing the above prerequisites.

## Compile
### Without Unicorn Hat HD simulator
```
$ qmake
$ make clean
$ make
```

### With Unicorn Hat HD simulator
Leddy includes a Unicorn Hat HD simulator which shows the LED's as pixels in a window on you desktop. Note that this requires a windowing system to work. This is useful while customizing your theme, if you want to run it on a non-Pi system.
```
$ qmake WITHSIM=1
$ make clean
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

## TODO
* Make weather scene specific (set city in xml) instead of global. So you can have several weather scenes, each with its own city