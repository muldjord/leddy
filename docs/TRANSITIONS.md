## Scene transitions
The general idea of Leddy is to show scenes with chroma-keyed transitions between them. The transitions can be customized easily by adding them to the `transitions` subfolder of your theme. Chroma-keying is the same as the green-screen effect used in movies.

Transitions are simple to create and use. Leddy supports both PNG and GIF image for the transitions. One transition only requires one file. All information needed will be in the filename and the contained sprites or sprite sheet.

NOTE! All sprites in the PNG and GIF files should be kept to 16x16 pixel dimensions. Leddy will scale them if needed, but the results will vary. It's better to scale them manually before adding them to get the result you want.

### Chroma keying
The way Leddy knows in what parts of the transition it should show the old scene and the new scene is by using specific colors in the frames. A value of clean magenta (RGB 255, 0, 255) is where the *old scene* will be rendered. A value of clean cyan (RGB 0, 255, 255) is where the *new scene* will be rendered.

Check the bundled examples to see how it can be used.

### Filenames
Part of the filename contains optional information about frame time (optional for animated GIF's). This defines how long each frame should be shown before moving on to the next in the sprite sheet. It is defined by setting it as a numeric after a dash in the filename. Here's an example:
```
name_of_transition-75.png
```
This will create a transition called `name_of_transition` and it will show each frame for 75 ms. Note that this is a *frame time* not a framerate. It can be set as low as 10 ms. Setting it lower will force it to 10 ms due to a delay the Unicorn Hat HD requires for updating the LED's.

### Using PNG
If you use PNG's for your transitions, you will have to export them as a horizontal sprite sheet. Most pixel art editing software will have this as an export option. The dimensions of each sprite / frame in the sprite sheet *must* be kept at 16x16 pixels. Leddy supports all PNG color formats.

### Using GIF
If you use GIF's for your transitions, you have the option of either exporting them as animated GIF's or as horizontal sprite sheets. Most pixel art editing software will have both options. The dimensions of each sprite / frame in the sprite sheet should be kept at 16x16 pixels. The advantage of using animated GIF's is that the editing software most often allows you to set the frame time of each frame directly in the animation. This is a lot more dynamic than setting a static frame time in the filename as described above.
