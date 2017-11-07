QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = interaction-example
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11
QT += widgets

SOURCES += main.cpp\
        mainwindow.cpp \
         ../qcustomplot.cpp \
    seq_iterations.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
         ../qcustomplot.h \
    seq_iterations.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui

DISTFILES +=

