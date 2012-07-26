#-------------------------------------------------
#
# Project created by QtCreator 2012-07-20T13:30:42
#
#-------------------------------------------------

QT       += core gui

TARGET = TemperatureControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    TemperatureIO.cpp

HEADERS  += mainwindow.h \
    TemperatureIO.h

FORMS    += mainwindow.ui

CONFIG += link_pkgconfig
PKGCONFIG += libwcl
