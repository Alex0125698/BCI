#-------------------------------------------------
#
# Project created by QtCreator 2018-05-16T01:54:06
#
#-------------------------------------------------

QT       += core gui serialport opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport winextras
RC_ICONS = bci_icon.ico

TARGET = BCI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QCUSTOMPLOT_USE_OPENGL

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    balltest.cpp \
    bciinterface.cpp \
    brainmap.cpp \
    controller.cpp \
    controllerstate.cpp \
    core.cpp \
    csvio.cpp \
    cytoninterface.cpp \
    debugwindow.cpp \
    error.cpp \
    graphwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowstate.cpp \
    offline.cpp \
    pixplotter.cpp \
    profiler.cpp \
    qcustomplot.cpp \
    resources.cpp \
    shader.cpp \
    sigprocessing.cpp \
    texture.cpp \
    stb_image.c

HEADERS += \
    balltest.h \
    bciinterface.h \
    brainmap.h \
    controller.h \
    controllerstate.h \
    core.h \
    csvio.h \
    cytoninterface.h \
    debugwindow.h \
    error.h \
    graphwidget.h \
    mainwindow.h \
    mainwindowstate.h \
    offline.h \
    openglresources.h \
    pixplotter.h \
    profiler.h \
    qcustomplot.h \
    resources.h \
    shader.h \
    sigprocessing.h \
    texture.h \
    timer.h \
    translation.h

FORMS += \
        mainwindow.ui

LIBS += -lopengl32 -lglu32

RESOURCES += \
    resources.qrc
