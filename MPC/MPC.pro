#-------------------------------------------------
#
# Project created by QtCreator 2013-04-03T10:24:05
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPC
TEMPLATE = app
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build

SOURCES += main.cpp\
        mainwindow.cpp \
    initialize.cpp \
    allapplicationdialog.cpp \
    ftpdownload.cpp \
    tools.cpp \
    aes.cpp \
    downloadthread.cpp \
    informationanalyze.cpp \
    httphandle.cpp \
    httpdownload.cpp \
    setup.cpp \
    start.cpp

HEADERS  += mainwindow.h \
    allapplicationdialog.h \
    ftpdownload.h \
    tools.h \
    downloadthread.h \
    start.h \
    httphandle.h \
    aes.h \
    setup.h \
    httpdownload.h \
    informationanalyze.h \
    data.h \
    initialize.h

FORMS    += mainwindow.ui
RC_FILE = icon.rc

RESOURCES += \
    src/resource.qrc
