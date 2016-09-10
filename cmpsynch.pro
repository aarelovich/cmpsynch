#-------------------------------------------------
#
# Project created by QtCreator 2016-09-10T06:27:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cmpsync
TEMPLATE = app


SOURCES += main.cpp\
        cmpsynch.cpp \
    datapacket.cpp \
    transfermanager.cpp

HEADERS  += cmpsynch.h \
    datapacket.h \
    transfermanager.h

FORMS    += cmpsynch.ui

CONFIG += mobility
MOBILITY = 

