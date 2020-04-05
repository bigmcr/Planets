#-------------------------------------------------
#
# Project created by QtCreator 2011-01-28T11:14:33
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

TARGET = Planets
TEMPLATE = app

INCLUDEPATH += "D:\Dropbox\QT Programming\Include Files"

SOURCES += main.cpp\
    Planets.cpp \
    pictureframe.cpp \
    Options.cpp \
    Matrix3x3.cpp \
    Plane.cpp \
    Point3D.cpp \
    scalarUnits.cpp \
    toString.cpp

HEADERS  += Planets.h \
    Object.h \
    pictureFrame.h \
    precision.h \
    toString.h \
    tracePoints.h \
    Options.h \
    Point3D.h \
    Matrix3x3.h \
    scalarUnits.h

FORMS    += Planets.ui \
    Options.ui
