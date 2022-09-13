# Leddy
Leddy displays customizable pixelly goodness and information on a Unicorn Hat HD (or Ubercorn Hat) for the Raspberry Pi.

## Video demonstration
A small video demonstrating the features of Leddy as of February 9th 2021 using an Ubercorn LED matrix can be seen [here](https://youtu.be/06wdx83tDZE). I've built a frame using the instructions seen [here](https://johnmccabe.net/technology/projects/ubercorn-gameframe-pt1) with a slight variation in the four spacer blocks to better hold the Ubercorn in place. All STL files can be found in the `stl` subfolder.

The scene rotation and look of each scene is easily customizable through just one XML file per [theme](docs/THEMES.md). [Themes](docs/THEMES.md) are located in subfolders under `themes`. A default [theme](docs/THEMES.md) is provided with the software to get you started in no time.

Leddy supports the following features:
- Animations (PNG sprite sheet or animated GIF)
- Chroma-keyed transitions (PNG sprite sheet or animated GIF)
- Weather (using OpenWeatherMap API)
- Time / Date
- Rss feed headlines / titles as scrolling text
- Conway's "Game of Life"
- Run terminal commands and display output on LED matrix

The general configuration of Leddy is handled through the `config.ini` file. It is created in the same folder as the executable when you run Leddy for the first time. You might want to fiddle with the following settings:
```
[theme]
path=themes/default

[unicorn_hd]
brightness=50
framerate=50
rotation=180
```
The fun stuff happens in the `themes` subfolder. In here you can create your own [theme](docs/THEMES.md) for use with the LED matrix. I recommend simply copying the `themes/default` [theme](docs/THEMES.md) and rename it to whatever you like. To use your [theme](docs/THEMES.md) simply set it in `config.ini`:
```
[theme]
path=themes/YOURTHEME
```
Read more about how to customize the `theme.xml` file [here](docs/THEMES.md).

## Prerequisites

### Hardware
* Raspberry Pi
* Unicorn Hat HD or Ubercorn hat (they are the same, just very different sizes)

### Software
You need to enable the Raspberry Pi SPI interface. This can be done in various simple ways. Read about it [here](https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md).

You also need to install Qt5 and libgif:
```
$ sudo apt-get update
$ sudo apt-get install build-essential qtbase5-dev libgif-dev
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
Leddy includes a Unicorn Hat HD simulator which shows the LED's as pixels in a window on you desktop. Note that this requires a windowing system to work. This is useful while customizing your theme, if you want to run it on a non-Pi system. When it's running you can resize the simulator window with the mouse scroll-wheel.
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
I like to run Leddy as a service using systemd. This enables it to auto-start when I boot the Raspberry Pi. I've provided a very simple systemd service file you can use to run it as a service. It's located under `systemd/leddy.service`. All you need to do is copy that file to `/etc/systemd/system/leddy.service` and edit the `ExecStart` path and executable to wherever your compiled `Leddy` executable is located. If you want to run Leddy with a different user than the `pi` user and group, you can also change that. Then run the following to reload the systemd services and enable it upon next reboot:
```
$ sudo systemctl daemon-reload
$ sudo systemctl enable leddy
```
If you don't want to reboot, you can start the service manually with:
```
$ sudo systemctl start leddy
```
And that's it! If you have the Unicorn Hat HD or Ubercorn Hat installed, it should start running on it.

#### Version 1.0.1 (In progress, unreleased):
* Added 'gallery' scene type

#### Version 1.0.0 (Unreleased):
* First public release
