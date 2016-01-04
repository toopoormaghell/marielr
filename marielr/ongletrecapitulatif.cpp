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
#include "util.h"
#include <QDate>
OngletRecapitulatif::OngletRecapitulatif(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OngletRecapitulatif),
    m_commandesencours(),
    m_util()
{
    ui->setupUi(this);
    ActualiserOnglet();
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
        haut_item->setData(0,Qt::UserRole,"CDE"+QString::number(cpt));

        haut_item->setText(0,"Commande du "+m_util.ChangementDate(m_commandesencours[cpt]->m_Date) );
        ui->ListeBDCLR->addTopLevelItem(haut_item);

        int compteurcommande =0;

        for (int iter = 0;iter<m_commandesencours[cpt]->m_Liste_Id_BDC.count();iter++)
        {
            QTreeWidgetItem* item=  new QTreeWidgetItem(haut_item);
            BDDCommande* temp = BDDCommande::RecupererCommande(m_commandesencours[cpt]->m_Liste_Id_BDC[iter]);

            item->setText(0,temp->m_Client->m_nom+"("+m_util.ChangementDate( temp->m_Infos->m_Date) +")");
            item->setData(0,Qt::UserRole,QString::number(cpt)+"BDC"+QString::number(compteurcommande));
            haut_item->addChild(item);
            compteurcommande ++;
        }
    }
}
void OngletRecapitulatif::AffichageMarge()
{
    ui->Marge->setText(QString::number(m_Marge,'f',2).replace(".","€"));
    ui->PrixClients->setText(QString::number(m_TotalTTCClients,'f',2).replace(".","€"));
}

void OngletRecapitulatif::on_ListeBDCLR_itemSelectionChanged()
{
    AfficherBDCSelectionne();
    //   Total();
}
QString OngletRecapitulatif::ChoixLR()
{
    QTreeWidgetItem* item = ui->ListeBDCLR->currentItem();

    return item != NULL ? item->data(0,Qt::UserRole).toString(): QString();
}
void OngletRecapitulatif::AfficherBDCSelectionne()
{
    ui->TableauProduits->clearContents();
    QString temp = ChoixLR();
    int cpt = 1;
    m_Marge= 0;
    m_TotalTTCClients=0;
    int row=0;

    int iter_debut = 0;
    int iter_fin = 0;


    if ( temp.contains("CDE"))
    {
        QString cpttemp =  temp.split("CDE")[1];
        cpt = cpttemp.toInt();
        iter_fin = m_commandesencours[cpt]->m_Liste_Id_BDC.count();

    } else
    {

        if (temp.contains("BDC"))
        {

            QStringList cpttemp = temp.split("BDC");

            cpt = cpttemp[0].toInt();
            iter_debut= cpttemp[1].toInt();
            iter_fin = iter_debut+1;
        }
    }

    for (int iter = iter_debut;iter<iter_fin;iter++)
    {

        BDDCommande* temp = BDDCommande::RecupererCommande(m_commandesencours[cpt]->m_Liste_Id_BDC[iter]);
        ui->TableauProduits->setRowCount(row+ temp->m_ListeProduits.count()+1);
        for (int i=0;i<temp->m_ListeProduits.count();i++)
        {
            QSharedPointer < BDDProduit > tempProduit = temp->m_ListeProduits[i].second;
            AfficherProduit(tempProduit,row+i,temp->m_ListeProduits[i].first,temp->m_Client->m_id);
        }

        //Affichage des totaux
        row = row + temp->m_ListeProduits.count();
        //Total Clients
        QTableWidgetItem* item = new QTableWidgetItem;
        QFont serifFont("Times", 12, QFont::Bold);item->setFont( serifFont);
        item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        item->setText(QString::number(m_TotalTTCClients,'f',2).replace(".","€"));
        ui->TableauProduits->setItem(row,7,item);

        //Total Marge
        item = new QTableWidgetItem;
        item->setText(QString::number(m_Marge,'f',2).replace(".","€"));
        item->setFont( serifFont);
        item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->TableauProduits->setItem(row,8,item);
    }
}
void OngletRecapitulatif::AfficherProduit(QSharedPointer < BDDProduit > tempProduit, int row, int Qte, int id_client)
{
    //affichage de la référence
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText(tempProduit->m_Ref);
    ui->TableauProduits->setItem(row,0,item);
    //affichage du nom du produit
    item = new QTableWidgetItem;
    item->setText(tempProduit->m_Nom);
    ui->TableauProduits->setItem(row,1,item);
    //Affichage du Prix HT LR
    item = new QTableWidgetItem;
    QString tempStr=tempProduit->m_PUHT ;
    item->setText(tempStr.replace(".","€"));
    item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->TableauProduits->setItem(row,2,item);
    //Affichage du Prix TTC LR
    item = new QTableWidgetItem;
    BDDPrix* prix = new BDDPrix(tempProduit->m_PUHT);
    prix->ApplicationTVA(tempProduit->m_TVA);
    tempStr = prix->m_resultat;
    item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    item->setText(tempStr.replace(".","€"));
    ui->TableauProduits->setItem(row,3,item);

    //Affichage du Prix TTC Clients
    item = new QTableWidgetItem;
    tempStr = tempProduit->m_PUClient;
    item->setText(tempStr.replace(".","€"));
    if (id_client ==1)
    {
        item->setText("0€00");
    }
    item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->TableauProduits->setItem(row,4,item);


    //Affichage de la Quantité
    item = new QTableWidgetItem;
    item->setText(QString::number(Qte));
    ui->TableauProduits->setItem(row,5,item);
    item->setTextAlignment(Qt::AlignCenter);
    //Affichage du Total TTC LR
    float TotalLR = prix->m_res * Qte;
    item = new QTableWidgetItem;
    tempStr =QString::number(TotalLR,'f',2);
    item->setText(tempStr.replace(".","€"));
    item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->TableauProduits->setItem(row,6,item);

    //Affichage du Total TTC Clients
    float TotalClient = tempProduit->m_PUClient.toFloat() * Qte;
    item = new QTableWidgetItem;
    tempStr =QString::number(TotalClient,'f',2);
    item->setText(tempStr.replace(".","€"));
    if (id_client ==1)
    {
        item->setText("0€00");
    }
    item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->TableauProduits->setItem(row,7,item);

    //Affichage de la Marge
    float Marge = TotalClient-TotalLR;
    item = new QTableWidgetItem;
    tempStr =QString::number(Marge,'f',2);
    item->setText(tempStr.replace(".","€"));
    if (id_client ==1)
    {
        item->setText("0€00");
    }
    item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->TableauProduits->setItem(row,8,item);

    if (id_client !=1)
    {
        //Calcul du Total Marge et du Total Clients
        m_Marge = m_Marge+ Marge;
        m_TotalTTCClients = m_TotalTTCClients + TotalClient;
    }
}

void OngletRecapitulatif::ActualiserOnglet()
{
    ui->TableauProduits->clearContents();
    ui->ListeBDCLR->clear();
    AfficherListeBDC();
    AfficherBDCSelectionne();
    ui->TableauProduits->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    RecupererCommandesMois();
    AfficherPeriode();

}
void OngletRecapitulatif::RecupererCommandesMois()
{
    //On récupère la date pour la comparer
    QString Date = QDateTime::currentDateTime().toString("yyyy.MM.dd");
    QStringList Datetemp = Date.split(".");
    int Annee = Datetemp[0].toInt();
    int Mois = Datetemp[1].toInt();
    int Jour = Datetemp[2].toInt();



    for (int i=0;i<m_commandesencours.count();i++)
    {
        Date = m_commandesencours[i]->m_Date;
        Datetemp = Date.split(".");
        int AnneeCde  = Datetemp[0].toInt();
        int MoisCde = Datetemp[1].toInt();
        int JourCde = Datetemp[2].toInt();


        if (Jour > 9 && MoisCde == Mois)
        {
            m_commandesmois << m_commandesencours[i];
        }
        if (Jour < 9 )
        {
            if (Mois == MoisCde)
            {
                m_commandesmois << m_commandesencours[i];

            }
            if ( ( Mois-1 == MoisCde || (Mois==1 && MoisCde == 12 && Annee-1 == AnneeCde) ) && JourCde > 9 )
            {
                m_commandesmois << m_commandesencours[i];
            }
        }
    }
    CalculMarge();
}
void OngletRecapitulatif::CalculMarge()
{
    m_Marge= 0;
    m_TotalTTCClients=0;

    //On prend toute les commandes
    for (int cpt= 0;cpt<m_commandesmois.count();cpt++)
    {
        for (int iter = 0;iter<m_commandesmois[cpt]->m_Liste_Id_BDC.count();iter++)
        {
            BDDCommande* temp = BDDCommande::RecupererCommande(m_commandesmois[cpt]->m_Liste_Id_BDC[iter]);
            for (int i = 0 ; i<temp->m_ListeProduits.count(); i++)
            {
                QSharedPointer < BDDProduit > tempProduit = temp->m_ListeProduits[i].second;
                int Qte = temp->m_ListeProduits[i].first;

                //Calcul Prix TTC LR
                BDDPrix* prix = new BDDPrix(tempProduit->m_PUHT);
                prix->ApplicationTVA(tempProduit->m_TVA);
                float TotalLR = prix->m_res * Qte;
                //Calcul de la marge
                float TotalClient = tempProduit->m_PUClient.toFloat() * Qte;
                float Marge = TotalClient-TotalLR;

                if (temp->m_Client->m_id !=1)
                {
                    //Calcul du Total Marge et du Total Clients
                    m_TotalTTCClients = m_TotalTTCClients + TotalClient;
                    m_Marge = m_Marge+ Marge;
                }
            }
        }
    }
    AffichageMarge();

}
void OngletRecapitulatif::AfficherPeriode()
{
    //On récupère la date pour afficher la période.
    QString Date = QDateTime::currentDateTime().toString("yyyy.MM.dd");
    QStringList Datetemp = Date.split(".");
    int Annee = Datetemp[0].toInt();
    int Mois = Datetemp[1].toInt();
    int Jour = Datetemp[2].toInt();

    QString premier ;
    int MoisDeuxieme;
    int AnneeDeuxieme;
    QString dernier ;
    if( Jour > 16 )
    {
        MoisDeuxieme = Mois +1 ;
        AnneeDeuxieme = Annee ;
        if (MoisDeuxieme > 12 )
        {
            MoisDeuxieme = 1;
            AnneeDeuxieme= Annee + 1;
        }
        premier = "10."+QString::number(Mois)+"."+QString::number(Annee);
        dernier = "09."+QString::number(MoisDeuxieme)+"."+QString::number(AnneeDeuxieme);
    }
    if (Jour < 15)
    {
        MoisDeuxieme = Mois - 1;
        AnneeDeuxieme = Annee ;
        if ( MoisDeuxieme < 1 )
        {
            MoisDeuxieme = 12;
            AnneeDeuxieme --;
        }
        premier = "10."+QString::number(MoisDeuxieme)+"."+QString::number(AnneeDeuxieme);
        dernier = "09."+QString::number(Mois)+"."+QString::number(Annee);
    }

    ui->Periode->setText("Entre le "+premier+" et le "+dernier);
}
