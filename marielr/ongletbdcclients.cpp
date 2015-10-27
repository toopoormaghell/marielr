#include "ongletbdcclients.h"
#include "ui_ongletbdcclients.h"
#include <QCompleter>
#include "bddgestion.h"
#include <QStandardItemModel>
#include "util.h"
#include <QDebug>
#include "bddaffichage.h"
#include "bddcommande.h"
#include "bddclient.h"
#include "bddinfoscommande.h"
#include "bddproduit.h"
#include "bddprix.h"

OngletBDCClients::OngletBDCClients(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OngletBDCClients),
    m_row(1),
    modele(new QStandardItemModel)
{
    ui->setupUi(this);
    CompletationClients();
    ModeleTableau();
    AjustementOnglet();
    AfficherListeBDC();
}

OngletBDCClients::~OngletBDCClients()
{
    delete ui;
}

void OngletBDCClients::AjouterBDC()
{
    BDDGestion temp;QString Paye;
    if (ui->checkBox->isChecked())
        Paye="true";
    else
        Paye = "false";
    temp.AjouterBDC(ui->NomClient->text(),RecupererProduits(),ui->NumeroCommande->text(),ui->Date->text(),ui->AutrePromo->text(),Paye,"false","false");
}

QList<ProduitCom> OngletBDCClients::RecupererProduits()
{
    QList<ProduitCom> Liste;

    for (int ligne=0;ligne<ui->TableauProduits->model()->rowCount();ligne++)
    {

        ProduitCom temp;
        temp.Ref =ui->TableauProduits->model()->index(ligne,0).data(2).toString();
        temp.Nom_Produit = ui->TableauProduits->model()->index(ligne,1).data(2).toString();
        temp.PUC = ui->TableauProduits->model()->index(ligne,2).data(2).toString();
        temp.Nb_Produit = ui->TableauProduits->model()->index(ligne,3).data(2).toString().toInt();
        temp.TVA = "20";
        if ( temp.Ref!="")
            Liste << temp;
    }
    return Liste;
}

void OngletBDCClients::on_NouvelleCommande_clicked()
{
    viderBDC();
}

void OngletBDCClients::viderBDC()
{
    ui->AutrePromo->clear();
    ui->checkBox->setChecked(false);
    ui->Date->setDate(QDate::currentDate());
    ui->NomClient->clear();
    ui->NumeroCommande->clear();
    ui->Total->setText("0€00");
    ModeleTableau();
    ui->ListeClientsEnCours->clear();
    AfficherListeBDC();

}

void OngletBDCClients::CompletationClients()
{
    QStringList Clients= m_affich.AfficherListeClients();


    ui->NomClient->setCompleter(new QCompleter(Clients));
}


void OngletBDCClients::AjustementOnglet()
{
    ui->Date->setDate(QDate::currentDate());

    ui->TableauProduits->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TableauProduits->setAlternatingRowColors(true);
}

void OngletBDCClients::ModeleTableau()
{
    m_row=1;
    modele= new QStandardItemModel(m_row,6);

    // Ici on modifie les titre d'entete
    QStringList list;
    list<<"Ref Produit"<<"Nom Produit"<<"Prix Unitaire Client TTC"<<"Quantité"<<"Prix Total"<<"Supprimer";
    modele->setHorizontalHeaderLabels(list);

    ui->TableauProduits->setModel(modele);
}

void OngletBDCClients::Total()
{
    float Total=0;
    for (int i=0;i<m_row;i++)
    {
        float PUC= ui->TableauProduits->model()->index(i,2).data(2).toString().replace("€",".").toFloat();
        int Qte= ui->TableauProduits->model()->index(i,3).data(2).toString().toInt();

        float TotalProduit=PUC*Qte;

        Total = Total + TotalProduit;

        //On affiche le prix Total par produit
        QStandardItem* item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(!Qt::ItemIsEditable);
        item->setText(QString::number(TotalProduit,'f',2).replace(".","€"));
        modele->setItem(i,4,item);

    }
    //On vérifie si il y a une promo à appliquer
    Total = ApplicationPromo(Total);

    //On affiche le prix Total par produit
    ui->Total->setText(QString::number(Total,'f',2).replace(".","€"));

}
float OngletBDCClients::ApplicationPromo(float Total)
{
    QString Prix = QString::number(Total);
    float Resultat=Total;
    QString promoaappliquer = ui->AutrePromo->text();

    if (promoaappliquer.contains("%",Qt::CaseInsensitive))
    {
        int promo = promoaappliquer.replace("%","").toInt();

        BDDPrix* Res = new BDDPrix(Prix,promo);

        Resultat = Res->m_resultat.toFloat();
    }

    return Resultat;
}

void OngletBDCClients::BDCEnCours()
{
    BDDAffichage temp;
    m_commandesencours = temp.AfficherListeCommandesEncours();

}

void OngletBDCClients::AfficherListeBDC()
{
    BDCEnCours();
    for (int cpt=0;cpt<m_commandesencours.count();cpt++)
    {
        QListWidgetItem* item = new QListWidgetItem;
        item->setData(Qt::UserRole,cpt);
        item->setText(m_commandesencours[cpt]->m_Client->m_nom+"("+m_commandesencours[cpt]->m_Infos->m_Date+")");

        ui->ListeClientsEnCours->addItem(item);
    }
}

void OngletBDCClients::BDCSelectionne(int cpt)
{
    BDDCommande* commande = m_commandesencours[cpt];
    //On affiche le nom du client
    ui->NomClient->setText(commande->m_Client->m_nom);
    //On affiche les infos de la commande
    //    QString date = commande->m_Infos->m_Date;
    QDate date = QDate::fromString(commande->m_Infos->m_Date,"dd.MM.YYYY");

    ui->Date->setDate(date);
    ui->AutrePromo->setText(commande->m_Infos->m_Promo);
    ui->checkBox->setChecked(commande->m_Infos->m_Paye== "true" ? true : false);
    ui->NumeroCommande->setText(commande->m_Infos->m_NumCommande);

    ListeProduitsBDCSelectionne(cpt);
}

void OngletBDCClients::ListeProduitsBDCSelectionne(int cpt)
{
    QList<QPair<int,QSharedPointer<BDDProduit> > >liste = m_commandesencours[cpt]->m_ListeProduits;
    m_row = liste.count();
    modele->setRowCount(m_row);
    for (int i=0;i<liste.count();i++)
    {
        //On affiche la quantite
        QStandardItem* item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(QString::number(liste[i].first));
        modele->setItem(i,3,item);
        //On affiche la référence
        item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].second->m_Ref);
        modele->setItem(i,0,item);
        //On affiche le nom du produit
        item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].second->m_Nom);
        modele->setItem(i,1,item);
        //On affiche le prix TTC Client Unitaire
        item = new QStandardItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].second->m_PUClient.replace(".","€"));
        modele->setItem(i,2,item);
    }

    Total();
}

void OngletBDCClients::on_ListeClientsEnCours_currentRowChanged(int currentRow)
{
    Q_UNUSED(currentRow);
    QListWidgetItem *item=ui->ListeClientsEnCours->currentItem();
    if (item !=NULL)
        BDCSelectionne(item->data(Qt::UserRole).toInt());
}

void OngletBDCClients::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( button->text() =="Enregistrer" )
    {
        AjouterBDC();
        viderBDC();
    }
}

void OngletBDCClients::on_TableauProduits_clicked(const QModelIndex &index)
{
    int row = index.row();
    int column = index.column();
    if (column==0 && row==m_row-1)
    {
        m_row++;
        modele->setRowCount(m_row);
    }
    Total();

}

void OngletBDCClients::on_Promo10_clicked()
{
    ui->AutrePromo->setText("10%");
    Total();
}
