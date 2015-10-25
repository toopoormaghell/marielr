#include "ongletlivraisonclients.h"
#include "ui_ongletlivraisonclients.h"

OngletLivraisonClients::OngletLivraisonClients(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OngletLivraisonClients)
{
    ui->setupUi(this);
}

OngletLivraisonClients::~OngletLivraisonClients()
{
    delete ui;
}
