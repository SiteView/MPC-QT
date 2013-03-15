#-------------------------------------------------
#
# Project created by QtCreator 2013-03-15T09:11:51
#
#-------------------------------------------------

QT       += core gui network

TARGET = MPC
TEMPLATE = app

INCLUDEPATH += Core UI Utils

UI_DIR  = gen-ui
MOC_DIR = gen-moc
OBJECTS_DIR = gen-obj
RCC_DIR = gen-rcc


SOURCES += main.cpp\
        UI/MainWindow.cpp \
    Utils/Utils.cpp \
    Core/DownLoad.cpp

HEADERS  += UI/MainWindow.h \
    Utils/Utils.h \
    Core/DownLoad.h

FORMS    += UI/MainWindow.ui

RESOURCES += \
    Resource/Resource.qrc
