## Scene transitions
The general idea of Leddy is to show scenes with chroma-keyed transitions between them. The transitions can be customized easily by adding them to the `transitions` subfolder of your theme. Chroma-keying is the same as the green-screen effect used in movies.

Transitions are simple to create and use. Leddy supports both PNG and GIF image for the transitions. One transition only requires one file. All information needed will be in the filename and the contained sprites or sprite sheet.

NOTE! All sprites in the PNG and GIF files should be kept to 16x16 pixel dimensions. Leddy will scale them if needed, but the results will vary. It's better to scale them manually before adding them to get the result you want.

### Filenames
Part of the filename contains information about frame time. This defines how long each frame should be shown before moving on to the next in the sprite sheet. This is done by setting it as a numeric after a dash in the filename. Here's an example:
```
name_of_transition-75.png
```
This will create a transition internally in Leddy which is called `name_of_transition` and it will show each frame for 75 ms. In other words, *this is not a framerate* but rather a frame time. This is plenty precise to make some fun animations. It can be set as low as 10 ms. Setting it lower will force it to 10 ms due to a delay the Unicorn Hat HD has for updating the LED's.


The filenames of your transitions should adhere to the following format for Leddy to understand them.
#### For sprite sheet transitions
```
NAME-frametime-duration.EXTENSION
```
or
```
NAME-frametime.EXTENSION
```
Example:
```
my_transition-50-10000.png
```
This will display each frame for 50 ms and switch to the next scene after 10 seconds (10000 ms). If you leave out the `-10000` part, it will simply display all frames once and switch to the next scene when the animation is over.

### Using PNG
If you use PNG's for your transitions, you will have to export them as a horizontal sprite sheet. Most pixelart editing software will have this as an export option. The dimensions of each sprite / frame in the sprite sheet should be kept at 16x16 pixels. Leddys supports all PNG color formats.

### Using GIF
If you use GIF's for your transitions, you have the option of either exporting them as animated gifs or as horizontal sprite sheets. Most pixelart editing software will have both options. The dimensions of each sprite / frame in the sprite sheet should be kept at 16x16 pixels. The advantage of using animated gifs is that the editing software most often allows you to set the frame time of each frame directly in the animation. This is a lot more dynamic than setting a static frame time in the filename.

### Chroma-keying



### Chroma keying
The sprite sheet is a horizontal 16x16 sliced sheet. The way Leddy knows where it should show the old scene and the new scene is by using specific colors in the frames. A value of clean magenta (RGB 255, 0, 255) is where the *old scene* will be rendered. A value of clean cyan (RGB 0, 255, 255) is where the *new scene* will be rendered.

Check the bundled example to see what that means.
