#-------------------------------------------------
#
# Project created by QtCreator 2019-03-17T17:06:49
#
#-------------------------------------------------

QT  += core gui
QT  += widgets

TARGET = DeadByUnicodeGUI
TEMPLATE = app


SOURCES += main.cpp\
    inputwindow.cpp \
    mainwindow.cpp \
    hexinput.cpp \
    keyeventreceiver.cpp \
    localization.cpp

HEADERS  += \
    inputwindow.h \
    mainwindow.h \
    hexinput.h \
    keyeventreceiver.h \
    localization.h

FORMS    += \
    inputwindow.ui \
    mainwindow.ui

CONFIG += c++11

RC_FILE = dbu_gui.rc

RESOURCES += \
    dbu_gui.qrc
