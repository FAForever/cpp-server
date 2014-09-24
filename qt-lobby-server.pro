#-------------------------------------------------
#
# Project created by QtCreator 2014-09-24T23:21:24
#
#-------------------------------------------------

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

QT       -= gui
QT       += network

TARGET = qt-lobby-server


SOURCES += main.cpp \
    LobbyServer.cpp \
    LobbyClient.cpp

HEADERS += \
    LobbyServer.h \
    LobbyClient.h \
    preamble.h
