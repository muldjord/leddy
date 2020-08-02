## Scene transitions
There are two types of rendering formats in Leddy. One of them is scene transitions. A scene transition is an animation or effect Leddy uses to smoothly move from one scene to the next.

The format of the transitions and even the implementation is still being worked out, so expect things to change.

Transitions are simple to create and use. It currenty uses 1 PNG file per transition and that's it! All information needed will be in the filename and the contained sprite sheet. The sprites in the sheet *must* be 16x16 pixels.

Part of the filename contains information about frame time. This defines how long each frame should be shown before moving on to the next in the sprite sheet. This is done by setting it as a numeric after a dash in the filename. Here's an example:
```
name_of_transition-75.png
```
This will create a transition internally in Leddy which is called `name_of_transition` and it will show each frame for 75 ms. In other words, *this is not a framerate* but rather a frame time. This is plenty precise to make some fun animations. It can be set as low as 10 ms. Setting it lower will force it to 10 ms due to a delay the Unicorn Hat HD has for updating the LED's.

### Chroma keying
The sprite sheet is a horizontal 16x16 sliced sheet. The way Leddy knows where it should show the old scene and the new scene is by using specific colors in the frames. A value of clean magenta (RGB 255, 0, 255) is where the *old scene* will be rendered. A value of clean cyan (RGB 0, 255, 255) is where the *new scene* will be rendered.

Check the bundled example to see what that means.
