# Leddy
Leddy displays easily customizable pixelly goodness on a variety of LED matrix displays. Currently it supports the following LED matrices:
- [Unicorn Hat HD](https://shop.pimoroni.com/products/unicorn-hat-hd)
- [Ubercorn Hat](https://shop.pimoroni.com/products/ubercorn)
- [Adafruit RGB Bonnet for the Raspberry Pi](https://www.adafruit.com/product/3211)
- An LED matrix simulator (running in a window on your desktop using Qt6).

## Video demonstration
A small video demonstrating some of the features of Leddy using an Ubercorn LED matrix can be seen [here](https://youtu.be/06wdx83tDZE). I've built a frame using the instructions seen [here](https://johnmccabe.net/technology/projects/ubercorn-gameframe-pt1) with a slight variation in the four spacer blocks to better hold the Ubercorn in place. All STL files can be found in the `stl` subfolder.

The scene rotation and look of each scene is easily customizable through just one XML file per [theme](docs/THEMES.md). Default themes are provided for a 16x16 matrix and for a 64x32 matrix. Those should get you started in no time.

Leddy supports the following features:
- Animations (PNG sprite sheet or animated GIF)
- Chroma-keyed transitions (PNG sprite sheet or animated GIF)
- Weather (using OpenWeatherMap API)
- Time / Date
- Rss feed headlines / titles as scrolling text
- Conway's "Game of Life"
- Run terminal commands and display output on LED matrix
- Scrolling image gallery
- A cozy snowfall scene

The general configuration of Leddy is handled through the `config.ini` file. It is created in the same folder as the executable when you run Leddy for the first time. In this file you can configure which theme to load with:
```
[theme]
xml=themes/yourtheme/theme.xml
```
You can also adjust the rotation, framerate and default brightness of your LED matrix:
```
[matrix]
brightness=50
framerate=30
rotation=180
```
But the fun stuff happens in the `themes` subfolder. In here you can create your own [theme](docs/THEMES.md) that Leddy can load and display on your LED matrix. A theme consists of a single XML file with theme definitions and a folder that contains all of the theme resources.

Read more on how to customize a theme XML file [here](docs/THEMES.md).

## Leddy prerequisites

### Hardware
* Raspberry Pi (most likely)
* A supported LED matrix (unless you just want to use the LED matrix simulator)

### Software
You will need the following packages installed on your Raspberry Pi OS:
```
$ sudo apt-get update
$ sudo apt-get install build-essential qt6-base-dev libgif-dev
```
For the Unicorn HAT HD or the Ubercorn HAT you need to enable the Raspberry Pi SPI interface. This can be easily done by editing `/boot/config.txt` and uncommenting the line `#rdtparam=spi=on` (by removing the `#`). Save the file and reboot the pi for the change to take effect.

## Compiling Leddy
Follow these instructions to clone the Leddy repository and compile the code.
```
$ git clone https://github.com/muldjord/leddy.git
$ cd leddy
$ mkdir build
$ cd build
$ cmake .. -DMATRIX_IMPL=MATRIXEXAMPLE
$ make
$ make install
```
Be sure to change `MATRIXEXAMPLE` to match your LED matrix. The `make install` command installs all relevant files in the `leddy/release` folder and makes a symbolic link to the `themes` folder.

## Running
From the `build` folder:
```
$ cd ../release
$ ./leddy
```

## Running as a service
You should consider running Leddy as a service using systemd. This enables it to auto-start when you boot the Raspberry Pi. I've provided a very simple systemd service file you can use to run it as a service. It's located under `systemd/leddy.service`. All you need to do is copy that file to `/etc/systemd/system/leddy.service` and edit the `ExecStart` path and executable to wherever your compiled `leddy` executable is located (most likely `release/leddy`). If you want to run Leddy with a different user than the `pi` user and group, you can also change that. Then run the following to reload the systemd services and enable it upon next reboot:
```
$ sudo systemctl daemon-reload
$ sudo systemctl enable leddy
```
If you don't want to reboot, you can start the service manually with:
```
$ sudo systemctl start leddy
```
And that's it! Your chosen LED matrix should hopefully come to life now.

#### Version 1.1.0 (In progress, unreleased):
* Switched project from qmake to cmake
* Added support for the Adafruit RGB LED bonnet (https://www.adafruit.com/product/3211)
* Added 'snowfall' scene type
* Added 'gallery' scene type
* Now uses EasyGifReader (MIT License) by Viktor Chlumsky which in turn uses gif_lib
* Fixed config framerate to be a framerate rather than a frametime
* Now uses MATRIX::WIDTH and MATRIX::HEIGHT constexpr for all matrix dimension manipulations

#### Version 1.0.0 (Unreleased):
* First code upload to the public
