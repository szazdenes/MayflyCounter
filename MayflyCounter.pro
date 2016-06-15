#-------------------------------------------------
#
# Project created by QtCreator 2015-06-15T16:50:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MayflyCounter
TEMPLATE = app
DESTDIR = ./bin
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
RCC_DIR = ./rcc
UI_DIR = ./ui

SOURCES += main.cpp\
        mayflycounter.cpp \
    counterform.cpp \
    imagegraphicsviewform.cpp \
    communicationsdialog.cpp \
    addgroupsdialog.cpp \
    exportdialog.cpp

HEADERS  += mayflycounter.h \
    counterform.h \
    imagegraphicsviewform.h \
    communicationsdialog.h \
    addgroupsdialog.h \
    exportdialog.h

FORMS    += mayflycounter.ui \
    counterform.ui \
    imagegraphicsviewform.ui \
    communicationsdialog.ui \
    addgroupsdialog.ui \
    exportdialog.ui
