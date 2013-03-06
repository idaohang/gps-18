#-------------------------------------------------
#
# Project created by QtCreator 2013-03-06T21:10:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mygraphicsview.cpp \
    ../../common/SQLiteDatabase.cpp \
    ../../common/sqlite3.c \
    ../../common/Road.cpp \
    ../../common/Node.cpp \
    ../../common/Link.cpp \
    ../../common/Database.cpp

HEADERS  += mainwindow.h \
    mygraphicsview.h \
    ../../common/SQLiteDatabase.hpp \
    ../../common/sqlite3ext.h \
    ../../common/sqlite3.h \
    ../../common/Singleton.hpp \
    ../../common/Road.hpp \
    ../../common/Node.hpp \
    ../../common/Link.hpp \
    ../../common/Database.hpp \
    ../../common/Converter.hpp

FORMS    += mainwindow.ui
