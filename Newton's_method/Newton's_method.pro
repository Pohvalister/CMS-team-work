QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = interaction-example
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
         ../qcustomplot.cpp

HEADERS  += mainwindow.h \
         ../qcustomplot.h

FORMS    += mainwindow.ui

