#-------------------------------------------------
#
# Project created by QtCreator 2013-03-05T14:21:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gps-editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mygraphicsview.cpp

HEADERS  += mainwindow.h \
    mygraphicsview.h

FORMS    += \
    mainwindow.ui

OTHER_FILES += \
    ../../map/map-200m.png
