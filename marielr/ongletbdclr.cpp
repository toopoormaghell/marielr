#include "ongletbdclr.h"
#include "ui_ongletbdclr.h"
#include "bddproduit.h"
#include "bddaffichage.h"
#include "util.h"
#include <QAbstractButton>
#include "bddcommande.h"
#include "bddclient.h"
#include "bddgestion.h"
#include "bddinfoscommande.h"
#include "bddprix.h"
#include <QDebug>
#include <cmath>

OngletBDCLR::OngletBDCLR(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OngletBDCLR),
    m_row(1)
{
    ui->setupUi(this);
    ActualiserOnglet();
}

OngletBDCLR::~OngletBDCLR()
{
    delete ui;
}

void OngletBDCLR::RemplirTableau()
{
    QList<ProduitCom> liste;
    if (m_commandesencours.count()>0)
        liste  = m_affich.AfficherListeProduitsACommander(m_commandesencours);
    //On ajoute les frais de port à afficher
    ProduitCom temp;
    temp.Nb_Produit=1;
    temp.Nom_Produit= "Frais de port";
    temp.PUHT="5.00";
    temp.TVA="20.00";
    liste << temp;

    m_row = liste.count();
    ui->TableauProduits->setRowCount(m_row);
    //On affiche chaque ligne
    for (int i=0;i<liste.count();i++)
    {
        //On affiche la quantite
        QTableWidgetItem* item = new  QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(QString::number(liste[i].Nb_Produit));
        ui->TableauProduits->setItem(i,5,item);
        //On affiche la référence
        item = new  QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].Ref);
        ui->TableauProduits->setItem(i,0,item);
        //On affiche le nom du produit
        item = new  QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].Nom_Produit);
        ui->TableauProduits->setItem(i,1,item);
        //On affiche le prix HT
        item = new  QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].PUHT.replace(".","€"));
        ui->TableauProduits->setItem(i,2,item);
        //On affiche la TVA
        item = new  QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].TVA+"%");
        ui->TableauProduits->setItem(i,3,item);

    }

    Total();
}
void OngletBDCLR::Total()
{
    float Total=0;
    for (int i=0;i<m_row;i++)
    {
        QString PUHT= ui->TableauProduits->item(i,2)->text().replace("€",".");
        int Qte= ui->TableauProduits->item(i,5)->text().toInt();
        QString TVA =  ui->TableauProduits->item(i,3)->text().replace("%","");
        BDDPrix* CalculTotalProduit = new BDDPrix(PUHT);
        CalculTotalProduit->Quantite(Qte);
        float TotalProduit= CalculTotalProduit->m_res;
        //On affiche le prix Total HT par produit
        QTableWidgetItem* item = new  QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(!Qt::ItemIsEditable);
        item->setText(QString::number(TotalProduit,'f',2).replace(".","€"));
        ui->TableauProduits->setItem(i,6,item);
        //On affiche le prix Unitaire TTC par produit
        BDDPrix* TTC= new BDDPrix(PUHT);
        TTC->ApplicationTVA(TVA);
        item = new  QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(!Qt::ItemIsEditable);
        QString affich = TTC->m_resultat;
        item->setText(affich.replace(".","€"));
        ui->TableauProduits->setItem(i,4,item);
        //On affiche le prix Total TTC par produit
        item = new  QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(!Qt::ItemIsEditable);
        TTC->Quantite(Qte);
        Total = Total + ( std::floor( TTC->m_res*100 + 0.5f ) ) / 100.0f;
        item->setText(TTC->m_resultat.replace(".","€"));
        ui->TableauProduits->setItem(i,7,item);

    }
    //On affiche le prix Total de la commande LR
    ui->Total->setText(QString::number(Total,'f',2).replace(".","€"));
}

void OngletBDCLR::on_TableauProduits_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    Total();

}

void OngletBDCLR::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( button->text() =="Enregistrer" )
    {
        AjouterBDC();
    }
}
void OngletBDCLR::AjouterBDC()
{
    BDDGestion temp;
    temp.MettreAJourProduits(RecupererProduits());
    temp.AjouterBDCLR(m_commandesencours,ui->Date->text());
    emit actu();
}
QList<ProduitCom> OngletBDCLR::RecupererProduits()
{
    QList<ProduitCom> Liste;

    for (int ligne=0;ligne<ui->TableauProduits->model()->rowCount();ligne++)
    {
        ProduitCom temp;
        temp.Ref =ui->TableauProduits->item(ligne,0)->text();
        temp.Nom_Produit = ui->TableauProduits->item(ligne,1)->text();
        temp.PUHT = ui->TableauProduits->item(ligne,2)->text().replace("€",".");
        temp.TVA = ui->TableauProduits->item(ligne,3)->text().replace("%","");

        if ( temp.Ref!="")
            Liste << temp;
    }
    return Liste;
}
void OngletBDCLR::AfficherListeBDC()
{
    m_commandesencours =  m_affich.AfficherListeCommandesEncours();
    for (int cpt=0;cpt<m_commandesencours.count();cpt++)
    {
        QListWidgetItem* item = new QListWidgetItem;
        item->setData(Qt::UserRole,cpt);
        item->setText(m_commandesencours[cpt]->m_Client->m_nom+"("+m_commandesencours[cpt]->m_Infos->m_Date+")");
        item->setSelected(true);
        ui->Liste_Clients->addItem(item);
    }
}
QList<int> OngletBDCLR::RecupererCommandes()
{
    QList<int> listecommandes;
    for (int cpt=0;cpt < m_commandesencours.count(); cpt++)
    {
        listecommandes << m_commandesencours[cpt]->m_id;
    }
    return listecommandes;
}
void OngletBDCLR::ActualiserOnglet()
{
    ui->Liste_Clients->clear();
    AfficherListeBDC();
    RemplirTableau();
    ui->TableauProduits->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Date->setDate(QDate::currentDate());
}
