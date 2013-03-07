#-------------------------------------------------
#
# Project created by QtCreator 2013-03-04T15:09:10
#
#-------------------------------------------------

QT       += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPS_second_GUI
TEMPLATE = app

INCLUDEPATH += "../common"
INCLUDEPATH += "../app2_GPS"
INCLUDEPATH += "../GPSVoice/"

SOURCES += main.cpp\
        mainwindow.cpp \
    ../common/SQLiteDatabase.cpp \
    ../common/sqlite3.c \
    ../common/Road.cpp \
    ../common/Node.cpp \
    ../common/Link.cpp \
    ../common/Database.cpp \
    car.cpp \
    ../app2_GPS/PathFinding.cpp \
    ../GPSVoice/RoadReading.cpp \
    ../GPSVoice/GPSVoice.cpp

HEADERS  += mainwindow.h \
    ../common/SQLiteDatabase.hpp \
    ../common/sqlite3ext.h \
    ../common/sqlite3.h \
    ../common/Singleton.hpp \
    ../common/Road.hpp \
    ../common/Node.hpp \
    ../common/Link.hpp \
    ../common/Database.hpp \
    ../common/Converter.hpp \
    car.h \
    ../app2_GPS/PathFinding.hpp \
    ../GPSVoice/RoadReading.hpp \
    ../GPSVoice/GPSVoice.hpp

FORMS    += mainwindow.ui

OTHER_FILES +=
