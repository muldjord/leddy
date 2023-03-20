TEMPLATE = app
TARGET = Leddy
DEPENDPATH += .
INCLUDEPATH += .
#CONFIG += release
CONFIG += debug
QT += core network xml
unix:LIBS += -lgif

!isEmpty(WITHSIM) {
  DEFINES += WITHSIM
  QT += widgets
  message("Unicorn Hat HD simulator enabled. Remember that the simulator requires a running windowing system to work.")
}

QMAKE_CXXFLAGS += -std=c++14

include(./VERSION)
DEFINES+=VERSION=\\\"$$VERSION\\\"

HEADERS += src/uniconn.h \
           src/leddy.h \
           src/netcomm.h \
           src/settings.h \
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
           src/globaldefs.h \
           src/rssscroll.h \
           src/gallery.h \
           src/runcommand.h \
           src/scenedesc.h \
           src/backgrounds.h

SOURCES += src/main.cpp \
           src/uniconn.cpp \
           src/leddy.cpp \
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
           src/rssscroll.cpp \
           src/gallery.cpp \
           src/runcommand.cpp \
           src/backgrounds.cpp

!isEmpty(WITHSIM) {
HEADERS += src/unisim.h
SOURCES += src/unisim.cpp
}
