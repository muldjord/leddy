## Themes
Leddy supports themes. A single theme is contained in a subfolder of the `themes` subfolder. The layout of a theme subfolder should be as follows:
```
animations/
transitions/
weather/
fonts/
backgrounds/
theme.xml
```
### theme.xml
This XML file is *required* for your theme to work. It basically defines everything about your theme and thereby what will be shown on the LED matrix when Leddy is running.

NOTE! Leddy is currently under heavy development and new features are added almost daily. Some features might not yet have been documented. For that reason the `themes/default/theme.xml` file might contain scenes or attributes that are not documented here. Rest assured they will be. But in the meantime feel free to play around with any undocumented features you find in there. Quite often they are self-explanatory.

Example `theme.xml`:
```
<?xml version="1.0" encoding="UTF-8"?>
<theme name="Default">
  <rotation>
    <animation name="random" duration="10000"/>
    <transition name="random"/>
    <weather duration="10000"
	     city="Berlin"
	     key="fe9fe6cf47c03d2640d5063fbfa053a2"
	     cityfont="tiny"
	     citycolor="#ffffff"
	     cityx="0"
	     cityy="2"
	     cityspacing="1"
	     tempfont="small"
	     tempx="0"
	     tempy="9"
	     tempspacing="1"/>
    <transition name="random"/>
    <animation name="random" duration="10000"/>
    <transition name="random"/>
    <timedate duration="10000"
	      bgcolor="random"
	      fontcolor="complimentary"
	      timefont="tiny"
	      timeformat="HH:mm"
	      timex="0"
	      timey="9"
	      timespacing="0,1,1,0"
	      datefont="tiny"
	      dateformat="ddMMM"
	      datex="0"
	      datey="2"
	      datespacing="0,1,0"/>
    <transition name="random"/>
    <animation name="random" duration="10000"/>
    <transition name="random"/>
    <rss url="http://rss.slashdot.org/Slashdot/slashdotMain"
	 bgcolor="random"
	 font="small"
	 fontcolor="complimentary"
	 showsource="true"
	 waveheight="4"
	 wavelength="60"/>
    <transition name="random"/>
  </rotation>
  <actions>
    <action time="07:00" parameter="brightness" value="100"/>
    <action time="23:59" parameter="brightness" value="5"/>
  </actions>
</theme>
```
The format should be pretty self-explanatory.

The `<rotation>` node contains the subnodes that define the scene rotation that will be shown on the LED matrix. The possible subnodes and their attributes are described in detail below (if I've gotten around to documenting them).

The `<actions>` node contains the subnodes that define the actions that are taken while running the software. This is currently only used to change the brightness of the LED matrix during the day. Especially useful to set the brightness low during the night to avoid your home looking like a laser light-show from outside. I might rename the actions to something else later.

#### &lt;animation&gt;
An `<animation .../>` subnode contains details about an animation.

* `name="NAME"`: Tells Leddy what animation to play. The animations are contained in the `animations` theme subfolder and are described [here](ANIMATIONS.md). If you set `name="random"` it will choose a random animation each time.
* `duration="10000"`: For how long should the animation play before switching to the next scene. If this attribute is left out the animation will play once.

#### &lt;transition&gt;
A `<transition .../>` subnode contains details about a transition that will blend the previous and next scenes together for a smooth transition.

* `name="NAME"`: Tells Leddy what transition to use. The transitions are contained in the `transitions` theme subfolder and are described [here](TRANSITIONS.md). If you set `name="random"` it will choose a random transition each time.

NOTE! Transitions aren't required between scenes. If left out it will simply switch instantly to the next scene in the rotation.

#### &lt;weather&gt;
A `<weather .../>` subnode uses the OpenWeatherMap API to shows the current weather for a specific city.

The following attributes are supported:
* `duration="10000"`: Sets the duration of the scene in ms.
* `background="myback"`: Sets a background image for the scene. Use the basename of any PNG from the `backgrounds` folder (eg. `myback.png` should be entered with `myback`).
* `bgcolor="#123456"`: If no background is set it will use this background color instead. Also supports special `random` which picks a random dark color.
* `city="Berlin"`: Sets the city to show the weather for. For instance `Copenhagen` or `Berlin`. If you have trouble getting it to show your city, try first searching for it at openweathermap.org and then type it in precisely as it shows up on there.
* `key="KEY"`: The OpenWeatherMap API key. If you want to use the default one provided with Leddy simply leave out this attribute entirely.
* `cityfont="tiny"`: What font to use for the city name. Use the base name of any PNG from the `fonts` folder (eg. `myfont.png` should be entered with `myfont`).
* `citycolor="#12345"`: Sets the font color for the city name. Also supports specials `random` which picks a random color and `complimentary` which picks a complimentary color to the defined `bgcolor`. If this attribute is left out, it will choose a color matching the temperature in a blue-green-orage-red scheme.
* `cityx="0"`: The x-coordinate for the city.
* `cityy="9"`: The y-coordinate for the city. 
* `cityspacing="0,1,1,0"`: The spacing in pixels to use between each of the characters of the city string. Supports both multivalues comma-separated or a single value which will then be used between all characters. Values can also be negative.
* `tempfont="tiny"`: What font to use for the temperature. Use the basename of any PNG from the `fonts` folder (eg. `myfont.png` should be entered with `myfont`).
* `tempcolor="#12345"`: Sets the font color for the temperature. Also supports specials `random` which picks a random color and `complimentary` which picks a complimentary color to the defined `bgcolor`. If this attribute is left out, it will choose a color matching the temperature in a blue-green-orage-red scheme.
* `tempx="0"`: The x-coordinate for the temp.
* `tempy="2"`: The y-coordinate for the temp.
* `tempspacing="0,1,0"`: The spacing in pixels to use between each of the characters of the temp string. Supports both multivalues comma-separated or a single value which will then be used between all characters. Values can also be negative.


#### &lt;timedate&gt;
The `<timedate .../>` subnode shows the current time and date.

The following attributes are supported:
* `duration="10000"`: Sets the duration of the scene in ms.
* `background="myback"`: Sets a background image for the scene. Use the basename of any PNG from the `backgrounds` folder (eg. `myback.png` should be entered with `myback`).
* `bgcolor="#123456"`: If no background is set it will use this background color instead. Also supports special `random` which picks a random dark color.
* `fontcolor="#123456"`: Sets the font color. Also supports specials `random` which picks a random color and `complimentary` which picks a complimentary color to the defined `bgcolor`.
* `timefont="tiny"`: What font to use for the time. Use the basename of any PNG from the `fonts` folder (eg. `myfont.png` should be entered with `myfont`).
* `timeformat="HH:mm"`: The format of the time. Supports Qt's formats as listed [here](https://doc.qt.io/qt-5/qtime.html#toString)
* `timex="0"`: The x-coordinate for the time.
* `timey="9"`: The y-coordinate for the time. 
* `timespacing="0,1,1,0"`: The spacing in pixels to use between each of the characters of the time string. Supports both multivalues comma-separated or a single value which will then be used between all characters. Values can also be negative.
* `datefont="tiny"`: What font to use for the date. Use the basename of any PNG from the `fonts` folder (eg. `myfont.png` should be entered with `myfont`).
* `dateformat="ddMMM"`: 
* `datex="0"`: The x-coordinate for the date.
* `datey="2"`: The y-coordinate for the date.
* `datespacing="0,1,0"`: The spacing in pixels to use between each of the characters of the date string. Supports both multivalues comma-separated or a single value which will then be used between all characters. Values can also be negative.

#### &lt;rss&gt;
The `<rss .../>` subnode allows you to show the titles of RSS feeds as scrolling text. Set the URL of the feed with `url="URL"`.

The following attributes are supported:
* `background="myback"`: Sets a background image for the scene. Use the basename of any PNG from the `backgrounds` folder (eg. `myback.png` should be entered with `myback`).
* `bgcolor="#123456"`: If no background is set it will use this background color instead. Also supports special `random` which picks a random dark color.
* `url="http://rss.slashdot.org/Slashdot/slashdotMain"`: Defines the URL to load the RSS feed from.
* `showsource="true|false"`: Boolean setting whether to show the source of the RSS feed title. If yes it will prepend `SLASHDOT: ` for this example.
* `font`: What font to use. Use the basename of any PNG from the `fonts` folder (eg. `myfont.png` should be entered with `myfont`).
* `fontcolor="#123456"`: Sets the font color. Also supports specials `random` which picks a random color and `complimentary` which picks a complimentary color to the defined `bgcolor`.
* `waveheight`: The titles can be animated in a scrolling sinewave. This defines the height of the sinewave in pixels.
* `wavelength`: The titles can be animated in a scrolling sinewave. This defines the length of the sinewave in pixels.

### Animations
Read more about animations [here](ANIMATIONS.md)

### Transitions
Read more about transitions [here](TRANSITIONS.md)

### Weather
The subfolder `weather` must contain all of the weather icons as needed by the OpenWeatherMap API. It *requires* the following icons:
* `01d.png`: No clouds (night)
* `01n.png`: No clouds (day)
* `02d.png`: Few clouds (night)
* `02n.png`: Few clouds (day)
* `03d.png`: Scattered clouds (night)
* `03n.png`: Scattered clouds (day)
* `04d.png`: Broken clouds (night)
* `04n.png`: Broken clouds (day)
* `09d.png`: Shower rain (night)
* `09n.png`: Shower rain (day)
* `10d.png`: Rain (night)
* `10n.png`: Rain (day)
* `11d.png`: Thunderstorm (night)
* `11n.png`: Thunderstorm (day)
* `13d.png`: Snow (night)
* `13n.png`: Snow (day)
* `50d.png`: Fog (night)
* `50n.png`: Fog (day)

All icons must be 16x16 pixel dimensions. Any color format is supported.

### Fonts
The subfolder `fonts` contains all of the custom fonts for the theme. Each letter is defined in a horizontal sprite sheet separated by a vertical line of clear magenta pixels (RGB 255, 0, 255). The name of the font is the base name of the filename. So a font loaded from `small.png` will simply be called `small`.

The ordering of the letters is defined in a text file that *must* have the same base name as the font PNG. So for `small.png` it must be called `small.txt`. This file simply contains all of the letters that are defined in the PNG, in the order they appear.

### Backgrounds
Some scene types allow you to set a custom background. The name of those backgrounds come from the base name of the PNG files located in this subfolder. So if you create a background file called `mybackground.png` it will be called `mybackground` when used in the `theme.xml`. For more information on what scenes support setting a background read the `theme.xml` documentation.
