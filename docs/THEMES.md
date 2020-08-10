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
    <timedate duration="5000" background="#000000" timefont="tiny" timecolor="#ffffff" timeformat="HH:mm" timex="0" timey="9" timespacing="0,1,1,0" datefont="tiny" datecolor="#ffffff" dateformat="ddMMM" datex="0" datey="2" datespacing="0,1,0"/>
    <transition name="random"/>
    <weather duration="5000"/>
    <transition name="random"/>
    <rss url="http://rss.slashdot.org/Slashdot/slashdotMain" background="lemmings" font="small" showsource="true" waveheight="4" wavelength="60"/>
    <transition name="random"/>
    <animation name="random"/>
    <transition name="random"/>
  </rotation>
</theme>
```
The format is pretty self-explanatory. The `<rotation>` node contains the subnodes that define the scene rotation that will be shown on the LED matrix. The possible subnodes and their attributes are described in detail below (if I've gotten around to documenting them).

#### &lt;animation&gt;
The `<animation .../>` subnode contains details about an animation.
*`name="NAME"`: Tells Leddy what animation to play. The animations are contained in the `animations` theme subfolder and are described [here](ANIMATIONS.md). If you set `name="random"` it will choose a random animation each time.
*`duration="10000"`: For how long should the animation play before switching to the next scene. If this attribute is left out the animation will play once.

#### &lt;transition&gt;
The `<transition .../>` subnode contains details about a transition that will blend the previous and next scenes together for a smooth transition.

*`name="NAME"`: Tells Leddy what transition to use. The transitions are contained in the `transitions` theme subfolder and are described [here](TRANSITIONS.md). If you set `name="random"` it will choose a random transition each time.

NOTE! Transitions aren't required between scenes. If left out it will simply switch instantly to the next scene in the rotation.

#### &lt;weather&gt;
The `<weather .../>` subnode uses the OpenWeatherMap API to shows the current weather for a specific city which can be set with `city="CITY"`. CITY can be, for instance, `Copenhagen`. If you have trouble getting it to show your city, try first searching for it at openweathermap.org and then type it in precisely as it shows up on there.

#### &lt;timedate&gt;
The `<timedate .../>` subnode shows the current time and date. Set a background image with `background="BACKGROUND"`. The BACKGROUND name comes from one of the defined backgrounds located in the theme `backgrounds` subfolder.

#### &lt;rss&gt;
The `<rss .../>` subnode allows you to show the titles of RSS feeds as scrolling text. Set the URL of the feed with `url="URL"`.

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
