#-------------------------------------------------
#
# Project created by QtCreator 2013-06-25T09:54:22
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = ../../bin
TARGET = SoftUpdate
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/softdownloadlist.cpp \
    src/informdialog.cpp


HEADERS  += mainwindow.h \
    src/softdownloadlist.h\
	../Common/SqliteDb.h \
    ../Common/SytemLog.h \
    src/informdialog.h


FORMS    += mainwindow.ui\
    src/informdialog.ui

RESOURCES +=resource.qrc

