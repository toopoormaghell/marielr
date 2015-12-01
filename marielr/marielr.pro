#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T17:22:34
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MarieLR
TEMPLATE = app

CONFIG += debug_and_release
CONFIG(debug,debug|release) {
    DESTDIR = ../../bin/debug
    OBJECTS_DIR = ../../obj/debug
    MOC_DIR = ../../moc/debug
    UI_DIR = ../../ui/debug
    LIBS += -L../../bin/debug
    RCC_DIR += ../../rcc/debug
} else {
    DESTDIR = ../../bin/release
    OBJECTS_DIR = ../../obj/release
    MOC_DIR = ../../moc/release
    UI_DIR = ../../ui/release
    LIBS += -L../../bin/release
    RCC_DIR += ../../rcc/release
}

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    ongletbdcclients.cpp \
    ongletbdclr.cpp \
    ongletlivraisonclients.cpp \
    ongletrecapitulatif.cpp \
    bddsingleton.cpp \
    bddclient.cpp \
    bddgestion.cpp \
    bddproduit.cpp \
    bddcommande.cpp \
    bddinfoscommande.cpp \
    util.cpp \
    bddaffichage.cpp \
    bddcomdelr.cpp \
    bddprix.cpp \
    ongletclients.cpp

HEADERS += \
    mainwindow.h \
    ongletbdcclients.h \
    ongletbdclr.h \
    ongletlivraisonclients.h \
    ongletrecapitulatif.h \
    bddsingleton.h \
    bddclient.h \
    bddgestion.h \
    bddproduit.h \
    bddcommande.h \
    bddinfoscommande.h \
    util.h \
    bddaffichage.h \
    bddcomdelr.h \
    bddprix.h \
    ongletclients.h

FORMS += \
    mainwindow.ui \
    ongletbdcclients.ui \
    ongletbdclr.ui \
    ongletlivraisonclients.ui \
    ongletrecapitulatif.ui \
    ongletclients.ui
