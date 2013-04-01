#check Qt version
QT_VERSION = $$[QT_VERSION]
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0)
QT_VER_MIN = $$member(QT_VERSION, 1)

lessThan(QT_VER_MAJ, 4) | lessThan(QT_VER_MIN, 5) {
   error( requires Qt 4.5 or newer but Qt $$[QT_VERSION] was detected.)
}

TEMPLATE = app
QT = gui \
    core \
    network
CONFIG += qt \
    warn_on \
    release
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
HEADERS = \
    src/tools.h \
    src/downloadthread.h \
    src/httpdownload.h \
    src/ftpdownload.h \
    src/allapplicationdialog.h \
    src/start.h \
    src/httphandle.h \
    src/informationanalyze.h \
    src/aes.h \
    setup.h \
    src/setup.h
SOURCES = src/main.cpp \
    src/tools.cpp \
    src/downloadthread.cpp \
    src/httpdownload.cpp \
    src/ftpdownload.cpp \
    src/allapplicationdialog.cpp \
    src/start.cpp \
    src/httphandle.cpp \
    src/informationanalyze.cpp \
    src/aes.cpp \
    src/setup.cpp

RC_FILE = icon.rc



