## Scene transitions
There are two types of rendering formats in Leddy. One of them is scene transitions. A scene transition is an animation or effect Leddy uses to smoothly move from one scene to the next.

The format of the transitions and even the implementation is still be worked out, so expect some of this to not yet be implemented. For now this is more like a brain-storming page for myself.

Transitions will be very simple to create and use. It will use 1 PNG file per transition. And all information needed will be in the filename and the containd 16x16 sprite sheet.

The filename can define how long each frame should be shown before moving on to the next in the sprite sheet. This is done by setting it as a numeric after a dash in the filename. Here's an example:
```
name_of_transition-75.png
```
This will create a transition internally in Leddy which is called `name_of_transition` and it will show each frame for 75 ms. In other words, *this is not a framerate* but rather a frametime. This is plenty precise to make some fun animations. It can be set as low as 10 ms, but you might encounter some weird behaviour when it's set so low due to a forced sleep needed for the Unicorn Hat HD to update the LED's.

### Chroma keying
The sprite sheet should be a vertical 16x16 sliced sheet. The way Leddy knows where it should show the old scene and the new scene is by using specific colors in the frames. A value of clean purple (RGB 255, 0, 255) is where the *old scene* will be rendered. A value of clean turqouise (RGB 0, 255, 255) is where the *new scene* will be rendered.

Check the bundled example to see what that means.