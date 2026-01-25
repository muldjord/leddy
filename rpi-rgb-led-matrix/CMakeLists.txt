# Builds the rpi-rgb-led-matrix library by Henner Zeller
# Find it here: https://github.com/hzeller/rpi-rgb-led-matrix
add_library(ledmatrix
  src/bdf-font.cc
  src/content-streamer-c.cc
  src/content-streamer.cc
  src/framebuffer.cc
  src/gpio.cc
  src/graphics.cc
  src/hardware-mapping.c
  src/led-matrix-c.cc
  src/led-matrix.cc
  src/multiplex-mappers.cc
  src/options-initialize.cc
  src/pixel-mapper.cc
  src/thread.cc
)

target_include_directories(ledmatrix PUBLIC include)
