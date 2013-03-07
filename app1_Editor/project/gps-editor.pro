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
    mygraphicsview.cpp \
    ../../common/Road.cpp \
    ../../common/Node.cpp \
    ../../common/Link.cpp \
    ../../common/Database.cpp \
    ../../common/SQLiteDatabase.cpp \
    ../../common/sqlite3.c \
    myqgraphicsellipseitem.cpp

HEADERS  += mainwindow.h \
    ../../common/Road.hpp \
    ../../common/Node.hpp \
    ../../common/Link.hpp \
    ../../common/Database.hpp \
    ../../common/SQLiteDatabase.hpp \
    ../../common/sqlite3ext.h \
    ../../common/sqlite3.h \
    ../../common/Singleton.hpp \
    ../../common/Converter.hpp \
    mygraphicsview.h \
    myqgraphicsellipseitem.h

FORMS    += \
    mainwindow.ui

OTHER_FILES += \
    ../../map/map-200m.png \
    ../map/map-200m.png
