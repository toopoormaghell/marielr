#include "ongletrecapitulatif.h"
#include "ui_ongletrecapitulatif.h"
#include "bddaffichage.h"
#include "bddcommande.h"
#include "bddclient.h"
#include "bddgestion.h"
#include "bddinfoscommande.h"
#include "bddcomdelr.h"
#include <QSharedPointer>
#include "bddprix.h"
#include <QDebug>

OngletRecapitulatif::OngletRecapitulatif(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OngletRecapitulatif),
    m_commandesencours()
{
    ui->setupUi(this);
    AfficherListeBDC();
}

OngletRecapitulatif::~OngletRecapitulatif()
{
    delete ui;
}
void OngletRecapitulatif::AfficherListeBDC()
{
    m_commandesencours =  m_affich.AfficherListeCommandesLREncours();
    for (int cpt=0;cpt<m_commandesencours.count();cpt++)
    {
        QTreeWidgetItem* haut_item = new QTreeWidgetItem(ui->ListeBDCLR);
        haut_item->setData(0,Qt::UserRole,cpt);
        haut_item->setText(0,"Commande du "+m_commandesencours[cpt]->m_Date+"" );
        ui->ListeBDCLR->addTopLevelItem(haut_item);

        for (int iter = 0;iter<m_commandesencours[cpt]->m_Liste_Id_BDC.count();iter++)
        {
            QTreeWidgetItem* item=  new QTreeWidgetItem(haut_item);
            BDDCommande* temp = BDDCommande::RecupererCommande(m_commandesencours[cpt]->m_Liste_Id_BDC[iter]);
            item->setText(0,temp->m_Client->m_nom+"("+temp->m_Infos->m_Date+")");
            //            item->setFlags(!Qt::ItemIsSelectable);
            haut_item->addChild(item);
        }
    }
}
void OngletRecapitulatif::Total()
{
    int cpt = ChoixLR();
    float marge=0;
    for (int iter = 0;iter<m_commandesencours[cpt]->m_Liste_Id_BDC.count();iter++)
    {
        BDDCommande* temp = BDDCommande::RecupererCommande(m_commandesencours[cpt]->m_Liste_Id_BDC[iter]);
        for (int i=0;i<temp->m_ListeProduits.count();i++)
        {
            QSharedPointer < BDDProduit > tempProduit = temp->m_ListeProduits[i].second;
            float TTCClient = tempProduit->m_PUClient.toFloat();

            BDDPrix* TTC = new BDDPrix(tempProduit->m_PUHT,tempProduit->m_TVA);
            float TTCLR = TTC->m_resultat.toFloat();
            qDebug() << tempProduit->m_PUHT << TTC->m_resultat;
            int nb = temp->m_ListeProduits[i].first;

            marge = marge + nb*(TTCClient-TTCLR);
        }
    }
    ui->Marge->setText(QString::number(marge));
}

void OngletRecapitulatif::on_ListeBDCLR_itemSelectionChanged()
{
    Total();
}
int OngletRecapitulatif::ChoixLR()
{
    return ui->ListeBDCLR->currentItem()->data(0,Qt::UserRole).toInt();
}
void OngletRecapitulatif::AfficherBDCSelectionne()
{

}
void OngletRecapitulatif::ActualiserOnglet()
{
    AfficherListeBDC();
}
