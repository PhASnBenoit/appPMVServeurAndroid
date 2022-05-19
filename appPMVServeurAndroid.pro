#-------------------------------------------------
#
# Project created by QtCreator 2016-03-07T01:22:56
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = appServeurTcp
TEMPLATE = app


SOURCES += main.cpp \
    cihm.cpp \
    cprotocoleJson.cpp \
    cserveurtcp.cpp

HEADERS  += \
    cihm.h \
    cprotocoleJson.h \
    cserveurtcp.h

FORMS    += \
    cihm.ui
