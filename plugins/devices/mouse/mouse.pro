#-------------------------------------------------
#
# Project created by QtCreator 2019-08-22T11:12:59
#
#-------------------------------------------------

QT       += widgets x11extras

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = lib
CONFIG += plugin

TARGET = $$qtLibraryTarget(mouse)
DESTDIR = ../../../pluginlibs

include(../../../env.pri)
include($$PROJECT_COMPONENTSOURCE/switchbutton.pri)

INCLUDEPATH   +=  \
                 $$PROJECT_COMPONENTSOURCE \
                 $$PROJECT_ROOTDIR \

LIBS          +=  -L/usr/lib/  -lXi -lgsettings-qt



#DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        mousecontrol.cpp

HEADERS += \
        mousecontrol.h

FORMS += \
        mousecontrol.ui
