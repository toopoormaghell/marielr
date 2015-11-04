#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ongletbdcclients.h"
#include <QWidget>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_ongletbdcclients(),
    m_ongletbdclr(),
    m_ongletlivraison(),
    m_ongletrecap()

{
    ui->setupUi(this);

    m_ongletbdcclients= ui->BDC;
    m_ongletbdclr = ui->BDCLR;
    m_ongletlivraison = ui->Livraison;
    m_ongletrecap = ui->Recap;
    //Changement dans l'ongletBDDClients, donc on rafraichit l'onglet LR
    connect(m_ongletbdcclients,SIGNAL(actu()),this,SLOT(changementOngletBDCClients()));
    //Changement dans l'ongletBDCLR, donc on rafraichit l'onglet Récap
    connect(m_ongletbdclr,SIGNAL(actu()),this,SLOT(changementOngletBDCLR()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::changementOngletBDCClients()
{
    ui->BDCLR->ActualiserOnglet();

}
void MainWindow::changementOngletBDCLR()
{
    ui->Recap->ActualiserOnglet();

}
