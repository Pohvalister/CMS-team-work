QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Newtons_method
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -O2

SOURCES += main.cpp\
        mainwindow.cpp \
         ../qcustomplot.cpp \
    newtons_method.cpp \
    q_tree.cpp

HEADERS  += mainwindow.h \
         ../qcustomplot.h \
    newtons_method.h \
    q_tree.h

FORMS    += mainwindow.ui

