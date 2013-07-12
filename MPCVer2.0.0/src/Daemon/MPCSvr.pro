# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = MPCSvr
DESTDIR = ../../bin
QT += core sql network gui
CONFIG += qtestlib debug console
DEFINES += _CONSOLE _CONSOLE QT_DLL QT_NETWORK_LIB QT_SQL_LIB QT_TESTLIB_LIB QT_OPENGL_ES_2 QT_OPENGL_ES_2_ANGLE
INCLUDEPATH += . \
    ./GeneratedFiles/Debug \
    ./GeneratedFiles/debug \
    ./../Common \
    ./GeneratedFiles/debug
LIBS += -llibEGLd \
    -llibGLESv2d \
    -lgdi32 \
    -luser32
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
HEADERS += ../Common/SqliteDb.h \
    ../Common/SytemLog.h \
    ./regflashclass.h \
    ./stdsoap2.h \
    ./SOAServiceStub.h \
    ./SOAServiceH.h \
	./tool.h
SOURCES += ./main.cpp \
    ./regflashclass.cpp \
    ./stdsoap2.cpp \
    ./SOAServiceC.cpp \
    ./SOAServiceClient.cpp
