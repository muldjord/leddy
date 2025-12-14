TEMPLATE = app
TARGET = Leddy
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += release
#CONFIG += debug
QT += core network xml
LIBS += -lgif

!isEmpty(WITHSIM) {
  DEFINES += WITHSIM
  QT += widgets
  HEADERS += src/matrixsim.h
  SOURCES += src/matrixsim.cpp
  message("Matrix LED simulator enabled.")
}

!isEmpty(WITHADA) {
  DEFINES += WITHADA
  INCLUDEPATH += ./include
  LIBS += -L./lib -lrgbmatrix -lpthread -lm -lrt
  HEADERS += src/matrixada.h
  SOURCES += src/matrixada.cpp
  message("Adafruit LED matrix bonnet support enabled.")
}

!isEmpty(WITHUNI) {
  DEFINES += WITHUNI
  HEADERS += src/matrixuni.h
  SOURCES += src/matrixuni.cpp
  message("Unicorn and Ubercorn LED matrix support enabled.")
}

QMAKE_CXXFLAGS += -std=c++20

include(./VERSION)
DEFINES+=VERSION=\\\"$$VERSION\\\"

HEADERS += src/leddy.h \
           src/settings.h \
           src/matrixabstract.h \
           src/netcomm.h \
           src/loader.h \
           src/EasyGifReader.h \
           src/commandhandler.h \
           src/commandqueue.h \
           src/pixelfont.h \
           src/scene.h \
           src/transition.h \
           src/animation.h \
           src/timedate.h \
           src/weather.h \
           src/gameoflife.h \
           src/snowfall.h \
           src/globaldefs.h \
           src/rssscroll.h \
           src/gallery.h \
           src/runcommand.h \
           src/scenedesc.h \
           src/backgrounds.h

SOURCES += src/main.cpp \
           src/leddy.cpp \
           src/matrixabstract.cpp \
           src/netcomm.cpp \
           src/loader.cpp \
           src/EasyGifReader.cpp \
           src/commandhandler.cpp \
           src/commandqueue.cpp \
           src/pixelfont.cpp \
           src/scene.cpp \
           src/transition.cpp \
           src/animation.cpp \
           src/timedate.cpp \
           src/weather.cpp \
           src/gameoflife.cpp \
           src/snowfall.cpp \
           src/rssscroll.cpp \
           src/gallery.cpp \
           src/runcommand.cpp \
           src/backgrounds.cpp
