#-------------------------------------------------
#
# Project created by QtCreator 2018-05-16T01:54:06
#
#-------------------------------------------------

QT       += core gui serialport

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
    bciinterface.cpp \
    controller.cpp \
    core.cpp \
    csvio.cpp \
    emotivinterface.cpp \
    error.cpp \
    graphwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    serialio.cpp \
    spatial_filter.cpp \
    state.cpp \
    statevariable.cpp

HEADERS += \
    bciinterface.h \
    controller.h \
    core.h \
    csvio.h \
    emotivinterface.h \
    error.h \
    graphwidget.h \
    main.h \
    mainwindow.h \
    qcustomplot.h \
    serialio.h \
    spatial_filter.h \
    state.h \
    statevariable.h \
    timer.h \
    translation.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources.qrc
