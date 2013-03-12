#-------------------------------------------------
#
# Project created by QtCreator 2013-03-12T13:44:37
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPC
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    informationanalyze.cpp \
    httphandle.cpp

HEADERS  += dialog.h \
    httphandle.h \
    informationanalyze.h

FORMS    += dialog.ui
