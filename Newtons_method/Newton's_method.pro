QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Newtons_method
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
         ../qcustomplot.cpp \
    newtons_method.cpp

HEADERS  += mainwindow.h \
         ../qcustomplot.h \
    newtons_method.h

FORMS    += mainwindow.ui

