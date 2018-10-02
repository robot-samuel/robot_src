#-------------------------------------------------
#
# Project created by QtCreator 2017-03-10T15:34:22
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 01-MCtrlterminal
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    uart_Mode.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    uart_Mode.hpp

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    terminal.qrc

DISTFILES += \
    ./images/application-exit.png \
    ./images/clear.png \
    ./images/connect.png \
    ./images/disconnect.png \
    ./images/settings.png
