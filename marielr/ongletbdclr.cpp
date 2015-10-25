#include "ongletbdclr.h"
#include "ui_ongletbdclr.h"
#include "bddproduit.h"
#include "bddaffichage.h"
#include <QStandardItemModel>
#include "util.h"
#include <QAbstractButton>
#include "bddcommande.h"
#include "bddclient.h"
#include "bddgestion.h"
#include "bddinfoscommande.h"
#include "bddprix.h"

OngletBDCLR::OngletBDCLR(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OngletBDCLR),
    m_row(1),
    modele(new QStandardItemModel)
{
    ui->setupUi(this);
    AfficherListeBDC();
    ModeleTableau();
    RemplirTableau();
}

OngletBDCLR::~OngletBDCLR()
{
    delete ui;
}

void OngletBDCLR::ModeleTableau()
{
    m_row=1;
    modele= new QStandardItemModel(m_row,8);

    // Ici on modifie les titre d'entete
    QStringList list;
    list<<"Ref Produit"<<"Nom Produit"<<"Prix Unitaire LR HT"<<"TVA"<<"Quantité"<<"Prix Total HT"<<"Prix Total TTC"<<"Supprimer";
    modele->setHorizontalHeaderLabels(list);

    ui->TableauProduits->setModel(modele);
    ui->TableauProduits->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TableauProduits->setAlternatingRowColors(true);
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
    temp.PUHT="5";
    temp.TVA="20";
    liste << temp;

    m_row = liste.count();
    modele->setRowCount(m_row);
    //On affiche chaque ligne
    for (int i=0;i<liste.count();i++)
    {
        //On affiche la quantite
        QStandardItem* item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(QString::number(liste[i].Nb_Produit));
        modele->setItem(i,4,item);
        //On affiche la référence
        item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].Ref);
        modele->setItem(i,0,item);
        //On affiche le nom du produit
        item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].Nom_Produit);
        modele->setItem(i,1,item);
        //On affiche le prix HT
        item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].PUHT.replace(".","€"));
        modele->setItem(i,2,item);
        //On affiche la TVA
        item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].TVA+"%");
        modele->setItem(i,3,item);
    }

    Total();
}
void OngletBDCLR::Total()
{
    float Total=0;
    for (int i=0;i<m_row;i++)
    {
        QString PUHT= ui->TableauProduits->model()->index(i,2).data(2).toString().replace("€",".");
        int Qte= ui->TableauProduits->model()->index(i,4).data(2).toString().toInt();
        QString TVA =  ui->TableauProduits->model()->index(i,3).data(2).toString().replace("%","");

        float TotalProduit=PUHT.toFloat()*Qte;
        //On affiche le prix Total HT par produit
        QStandardItem* item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(!Qt::ItemIsEditable);
        item->setText(QString::number(TotalProduit,'f',2).replace(".","€"));
        modele->setItem(i,5,item);
        //On affiche le prix Total TTC par produit
        BDDPrix* TTC= new BDDPrix(PUHT,TVA);
        item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(!Qt::ItemIsEditable);
        item->setText(TTC->m_resultat.replace(".","€"));
        modele->setItem(i,6,item);

        Total = Total + TTC->m_resultat.toFloat();
    }
    //On affiche le prix Total par produit
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
    temp.AjouterBDCLR(m_commandesencours,"06/10/2015");
}
QList<ProduitCom> OngletBDCLR::RecupererProduits()
{
    QList<ProduitCom> Liste;

    for (int ligne=0;ligne<ui->TableauProduits->model()->rowCount();ligne++)
    {
        ProduitCom temp;
        temp.Ref =ui->TableauProduits->model()->index(ligne,0).data(2).toString();
        temp.Nom_Produit = ui->TableauProduits->model()->index(ligne,1).data(2).toString();
        temp.PUHT = ui->TableauProduits->model()->index(ligne,2).data(2).toString().replace("€",".").toFloat();
        temp.TVA = ui->TableauProduits->model()->index(ligne,3).data(2).toString().replace("%","").toFloat();

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
