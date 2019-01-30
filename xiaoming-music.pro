#-------------------------------------------------
#
# Project created by QtCreator 2019-01-09T09:04:53
#
#-------------------------------------------------

TARGET = xiaoming-music
TEMPLATE = app

CONFIG += c++11
CONFIG += qt warn_on
CONFIG += release
CONFIG += link_pkgconfig

inst1.files += xiaoming-music.desktop
inst1.path = /usr/share/applications
target.source  += $$TARGET
target.path = /usr/bin
INSTALLS += inst1 \
    target

QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)

LIBS += -lmpv -lavformat -lavutil -lavcodec

isEqual(QT_MAJOR_VERSION, 5) {
    QT += core gui widgets xml network
}

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/mpvplayer.cpp \
    src/global.cpp \
    src/preferences.cpp \
    src/musicmanager.cpp \
    src/analysisermodule.cpp \
    src/musicmeta.cpp \
    src/musicanalysiser.cpp \
    src/utils.cpp \
    src/lyricmodule.cpp \
    src/lyricbaseworker.cpp \
    src/lyricneteaseworker.cpp \
    src/lyrickugouworker.cpp

HEADERS  += src/mainwindow.h \
    src/mpvplayer.h \
    src/global.h \
    src/preferences.h \
    src/musicmanager.h \
    src/analysisermodule.h \
    src/musicmeta.h \
    src/musicanalysiser.h \
    src/utils.h \
    src/lyricmodule.h \
    src/lyricbaseworker.h \
    src/lyricneteaseworker.h \
    src/lyrickugouworker.h
