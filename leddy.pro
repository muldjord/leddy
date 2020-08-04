TEMPLATE = app
TARGET = Leddy
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += release
QT += core network xml

!isEmpty(WITHSIM) {
  DEFINES+=WITHSIM
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
           src/transition.h \
           src/scene.h

SOURCES += src/main.cpp \
           src/uniconn.cpp \
           src/leddy.cpp \
           src/netcomm.cpp \
           src/loader.cpp \
           src/pixelfont.cpp \
           src/transition.cpp \
           src/scene.cpp

!isEmpty(WITHSIM) {
HEADERS += src/unisim.h
SOURCES += src/unisim.cpp
}

RESOURCES += leddy.qrc
