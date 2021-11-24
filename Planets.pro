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
    Matrix.cpp \
    Planets.cpp \
    Scenario.cpp \
    pictureframe.cpp \
    Options.cpp \
    Matrix3x3.cpp \
    Plane.cpp \
    Point3D.cpp \
    scalarUnits.cpp \
    scenariodialog.cpp \
    toString.cpp \
    unit.cpp

HEADERS  += Planets.h \
    Butcher_Tableau.h \
    Matrix.h \
    Object.h \
    Scenario.h \
    physicsconsts.h \
    pictureFrame.h \
    precision.h \
    scenariodialog.h \
    toString.h \
    tracePoints.h \
    Options.h \
    Point3D.h \
    Matrix3x3.h \
    scalarUnits.h \
    unit.h

FORMS    += Planets.ui \
    Options.ui \
    scenariodialog.ui
