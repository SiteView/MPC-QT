#-------------------------------------------------
#
# Project created by QtCreator 2013-06-25T09:54:22
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

#DESTDIR = ../../bin
TARGET = ../../bin/SoftUpdate
TEMPLATE = app
INCLUDEPATH += . \
               ./include\
    ./GeneratedFiles
LIBS += -L"./lib" \
                -llibcurl 

SOURCES += main.cpp\
        mainwindow.cpp \
    src/SoftDownloadList.cpp \
    src/SoftDownloadItem.cpp \
    src/SoftAllKindList.cpp \
    src/SoftAllKindItem.cpp \
    src/InformDialog.cpp \
    src/SoftUnloadList.cpp \
    src/SoftUnloadItem.cpp \
    src/SoftUpgradeList.cpp \
    src/SoftUpgradeItem.cpp \
    ./curldownloadmanager.cpp

HEADERS  += mainwindow.h \
	../Common/SqliteDb.h \
    ../Common/SytemLog.h \
    src/SoftDownloadList.h \
    src/SoftDownloadItem.h \
    src/SoftAllKindList.h \
    src/SoftAllKindItem.h \
    src/InformDialog.h \
    src/SoftUnloadList.h \
    src/SoftUnloadItem.h \
    src/SoftUpgradeList.h \
    src/SoftUpgradeItem.h \
    ./curldownloadmanager.h

FORMS    += mainwindow.ui
 #   src/informdialog.ui

RESOURCES +=resource.qrc

