TEMPLATE = app
TARGET = Leddy
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += release
QT += core network xml
QMAKE_CXXFLAGS += -std=c++14

include(./VERSION)
DEFINES+=VERSION=\\\"$$VERSION\\\"

HEADERS += src/uniconn.h \
           src/leddy.h \
           src/netcomm.h \
           src/settings.h \
           src/loader.h

SOURCES += src/main.cpp \
           src/uniconn.cpp \
           src/leddy.cpp \
           src/netcomm.cpp \
           src/loader.cpp

RESOURCES += leddy.qrc
