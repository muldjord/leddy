TEMPLATE = app
TARGET = Leddy
DEPENDPATH += .
INCLUDEPATH += .
#CONFIG += release
CONFIG += debug
QT += core network xml

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
           src/pixelfont.h \
           src/scene.h \
           src/transition.h \
           src/animation.h \
           src/timedate.h \
           src/weather.h \
           src/globaldefs.h \
           src/textscroll.h

SOURCES += src/main.cpp \
           src/uniconn.cpp \
           src/leddy.cpp \
           src/netcomm.cpp \
           src/loader.cpp \
           src/pixelfont.cpp \
           src/scene.cpp \
           src/transition.cpp \
           src/animation.cpp \
           src/timedate.cpp \
           src/weather.cpp \
           src/textscroll.cpp

!isEmpty(WITHSIM) {
HEADERS += src/unisim.h
SOURCES += src/unisim.cpp
}

RESOURCES += leddy.qrc
