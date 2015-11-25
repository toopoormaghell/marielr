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
    m_mapper(new QSignalMapper)
{
    ui->setupUi(this);
    MAJOnglet();
    connect(m_mapper,SIGNAL(mapped(int)),this,SLOT(suppressionProduit(int)));

}
OngletBDCClients::~OngletBDCClients()
{
    delete ui;
}
void OngletBDCClients::suppressionProduit(int row)
{
    if (!ui->TableauProduits->item(row,0)->text().isEmpty())
    {
        ui->TableauProduits->removeRow(row);
        ui->TableauProduits->setRowCount(m_row);
        AjoutLigneProduitVide(m_row-1);
        Total();
    }
}
void OngletBDCClients::MAJOnglet()
{
    viderBDC();
    m_row=1;
    ui->TableauProduits->setRowCount(m_row);
    CompletationClients();
    ui->TableauProduits->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    AfficherListeBDC();
    AjoutLigneProduitVide(0);
}

void OngletBDCClients::chargementProduit(int row, int column)
{
    if (row==m_row-2 && row>-1 && column==1)
    {
        QString ref= ui->TableauProduits->item(row,0)->text();
        if (ref!="")
        {
            BDDProduit* temp = BDDProduit::RecupererProduit(ref);
            if (temp->m_Nom!="")
            {
                //On affiche le nom du produit
                QTableWidgetItem* item = new QTableWidgetItem;
                item->setText(temp->m_Nom);
                ui->TableauProduits->setItem(row,1,item);
                //On affiche le prix TTC Client Unitaire
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignCenter);
                item->setText(temp->m_PUClient.replace(".","€"));
                ui->TableauProduits->setItem(row,2,item);
            }
            Total();
        }
    }
}
void OngletBDCClients::AjouterBDC()
{
    BDDGestion temp;
    QString Paye;
    if (ui->checkBox->isChecked())
        Paye="true";
    else
        Paye = "false";
    temp.AjouterBDC(ui->NomClient->text(),RecupererProduits(),ui->NumeroCommande->text(),ui->Date->text(),ui->AutrePromo->text(),Paye,"false","false");

    emit actu();
}

QList<ProduitCom> OngletBDCClients::RecupererProduits()
{
    QList<ProduitCom> Liste;

    for (int ligne=0;ligne<ui->TableauProduits->rowCount();ligne++)
    {
        ProduitCom temp;
        temp.Ref = ui->TableauProduits->item(ligne,0)->text();
        temp.Nom_Produit = ui->TableauProduits->item(ligne,1)->text();
        temp.PUC = ui->TableauProduits->item(ligne,2)->text().replace("€",".");
        temp.Nb_Produit = ui->TableauProduits->item(ligne,3)->text().toInt();
        temp.TVA = "20";
        if ( temp.Ref!="")
            Liste << temp;
    }
    return Liste;
}

void OngletBDCClients::on_NouvelleCommande_clicked()
{
    MAJOnglet();
}

void OngletBDCClients::viderBDC()
{
    ui->AutrePromo->clear();
    ui->checkBox->setChecked(true);
    ui->Date->setDate(QDate::currentDate());
    ui->NomClient->clear();
    ui->NumeroCommande->clear();
    ui->Total->setText("0€00");
    ui->ListeClientsEnCours->clear();
    ui->TableauProduits->clearContents();

}

void OngletBDCClients::CompletationClients()
{
    QStringList Clients= m_affich.AfficherListeClients();

    ui->NomClient->setCompleter(new QCompleter(Clients));
}

void OngletBDCClients::Total()
{
    float Total=0;
    for (int i=0;i<m_row;i++)
    {
        float PUC= ui->TableauProduits->item(i,2)->text().replace("€",".").toFloat();
        int Qte= ui->TableauProduits->item(i,3)->text().toInt();

        float TotalProduit=PUC*Qte;

        Total = Total + TotalProduit;

        //On affiche le prix Total par produit
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(!Qt::ItemIsEditable);
        item->setText(QString::number(TotalProduit,'f',2).replace(".","€"));
        ui->TableauProduits->setItem(i,4,item);

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

        BDDPrix* Res = new BDDPrix(Prix);
        Res->ApplicationReduc(promo);
        Resultat = Res->m_resultat.toFloat();
    }

    return Resultat;
}
void OngletBDCClients::AfficherListeBDC()
{
    m_commandesencours = m_affich.AfficherListeCommandesEncours();
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
    ui->Date->setDate(QDate::fromString(commande->m_Infos->m_Date,"dd.MM.yyyy"));
    ui->AutrePromo->setText(commande->m_Infos->m_Promo);
    ui->checkBox->setChecked(commande->m_Infos->m_Paye== "true" ? true : false);
    ui->NumeroCommande->setText(commande->m_Infos->m_NumCommande);

    ListeProduitsBDCSelectionne(cpt);
}

void OngletBDCClients::ListeProduitsBDCSelectionne(int cpt)
{
    QList<QPair<int,QSharedPointer<BDDProduit> > >liste = m_commandesencours[cpt]->m_ListeProduits;
    m_row = liste.count();
    ui->TableauProduits->setRowCount(m_row);
    for (int i=0;i<liste.count();i++)
    {
        //On affiche la quantite
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(QString::number(liste[i].first));
        ui->TableauProduits->setItem(i,3,item);
        //On affiche la référence
        item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].second->m_Ref);
        ui->TableauProduits->setItem(i,0,item);
        //On affiche le nom du produit
        item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].second->m_Nom);
        ui->TableauProduits->setItem(i,1,item);
        //On affiche le prix TTC Client Unitaire
        item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(liste[i].second->m_PUClient.replace(".","€"));
        ui->TableauProduits->setItem(i,2,item);
        //On affiche le bouton Supprimer de chaque ligne
        QPushButton* button = new QPushButton;
        button->setText("Supprimer");
        connect(button,SIGNAL(clicked(bool)),m_mapper,SLOT(map()));
        m_mapper->setMapping(button,i);
        ui->TableauProduits->setCellWidget(i,5,button);
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
        MAJOnglet();
    }
}
void OngletBDCClients::on_Promo10_clicked()
{
    ui->AutrePromo->setText("10%");
    Total();

}
void OngletBDCClients::AjoutLigneProduitVide(int row)
{
    //On affiche le bouton Supprimer de la nouvelle ligne
    QPushButton* button = new QPushButton;
    button->setText("Supprimer");
    connect(button,SIGNAL(clicked(bool)),m_mapper,SLOT(map()));
    m_mapper->setMapping(button,row);
    ui->TableauProduits->setCellWidget(row,5,button);
    //On affiche la quantite
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::number(1));
    ui->TableauProduits->setItem(row,3,item);
    //On affiche la référence
    item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText("");
    ui->TableauProduits->setItem(row,0,item);
    //On affiche le nom du produit
    item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText("");
    ui->TableauProduits->setItem(row,1,item);
    //On affiche le prix TTC Client Unitaire
    item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText("0.00");
    ui->TableauProduits->setItem(row,2,item);
}

void OngletBDCClients::on_TableauProduits_cellClicked(int row, int column)
{
    if (column==0 && row==m_row-1 )
    {
        m_row++;
        ui->TableauProduits->setRowCount(m_row);
        AjoutLigneProduitVide(m_row-1);
    }
    Total();
    chargementProduit(row,column);
    PrixEnEuros();
}

void OngletBDCClients::PrixEnEuros()
{
    for (int row=0;row<m_row;row++)
    {
        if ( ( ui->TableauProduits->item(row,2) != NULL ) &&
             !ui->TableauProduits->item(row,2)->text().isEmpty() )
        {
            QString ref= ui->TableauProduits->item(row,2)->text();
            ui->TableauProduits->item(row,2)->setText(ref.replace(".","€"));
        }
    }
}
