#-------------------------------------------------
#
# Project created by QtCreator 2020-11-17T19:36:26
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = softUpdate
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    commonfun.cpp \
    cJSON.c \
    updatetask.cpp

HEADERS  += widget.h \
    commonfun.h \
    cJSON.h \
    updatetask.h

FORMS    += widget.ui
