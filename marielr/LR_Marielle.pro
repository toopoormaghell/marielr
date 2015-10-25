#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T17:22:34
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MarieLR
TEMPLATE = app


SOURCES += main.cpp\
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
    bddprix.cpp

HEADERS  += mainwindow.h \
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
    bddprix.h

FORMS    += mainwindow.ui \
    ongletbdcclients.ui \
    ongletbdclr.ui \
    ongletlivraisonclients.ui \
    ongletrecapitulatif.ui
