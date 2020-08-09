## Animations
A scene in Leddy can simply be a pixel art animation. The animations can be customized easily by adding them to the `animations` subfolder of your theme.

Animations are simple to create and use. Leddy supports both PNG and GIF images for the animations. One animation only requires one file. All information needed will be in the filename and the contained sprites or sprite sheet.

NOTE! All sprites in the PNG and GIF files should be kept to 16x16 pixel dimensions. Leddy will scale them if needed, but the results will vary. It's better to scale them manually before adding them to get the result you want.

### Filenames
Depending on file format (GIF or PNG) part of the filename contains information about optional frame time and animation duration. This defines how long each frame should be shown before moving on to the next in the sprite sheet and for how long the animation should play before switching to the next scene. Both can be set as a numerics with dashes between them in the filename. Here's an example:
```
name_of_animation-75-10000.png
```
This will create an animation called `name_of_animation`.

The 75 defines the frame time in ms (note that it's a *frame time*, not a framerate). It can be set as low as 10 ms. Setting it lower will force it to 10 ms due to the delay the Unicorn Hat HD requires for updating the LED's. The frame time is only required for sprite sheet animations. For animated GIF's this information is contained within the frame data of the file and can (should) be left out.

The 10000 defines the duration of the animation in ms. If left out the animation will simply play once.

### Using PNG
If you use PNG's for your animations, you will have to export them as a horizontal sprite sheet. Most pixelart editing software will have this as an export option. The dimensions of each sprite / frame in the sprite sheet *must* be kept at 16x16 pixels. Leddy supports all PNG color formats.

### Using GIF
If you use GIF's for your animations, you have the option of either exporting them as animated gifs or as horizontal sprite sheets. Most pixelart editing software will have both options. The dimensions of each sprite / frame in the sprite sheet should be kept at 16x16 pixels.

The advantage of using animated gifs is that the editing software most often allows you to set the frame time of each frame directly in the animation. This is a lot more dynamic than setting a static frame time in the filename. If you are exporting it as a horizontal sprite sheet instead, it works the same as a PNG.
