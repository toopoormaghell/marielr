#include "ongletclients.h"
#include "ui_ongletclients.h"
#include "QListWidgetItem"
#include "bddclient.h"
#include "bddcommande.h"
#include "bddinfoscommande.h"
#include "bddprix.h"

OngletClients::OngletClients(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OngletClients),
    m_cptclient(0),
    m_ListeClients (),
    m_Commandes()
{
    ui->setupUi(this);
    ActualiserOnglet();
}

OngletClients::~OngletClients()
{
    delete ui;
}

void OngletClients::ActualiserOnglet()
{
    AfficherListeClients();
      ui->TableauProduits->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void OngletClients::AfficherListeClients()
{
    m_ListeClients = m_affich.RecupererListeClients();

    for (int cpt =0 ; cpt < m_ListeClients.count(); cpt++ )
    {
        QListWidgetItem* item = new QListWidgetItem;
        item->setData(Qt::UserRole,cpt);
        item->setText(m_ListeClients[cpt]->m_nom);
        ui->ListeClients->addItem(item);
    }
}

void OngletClients::on_ListeClients_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    ui->TableauProduits->clearContents();
    ui->ListeBDC->clear();
    m_cptclient = current->data(Qt::UserRole).toInt();
    AfficheRenseignementsClient();
    AfficherListeBDC();

}
void OngletClients::AfficheRenseignementsClient()
{
    ui->NomClient->setText( m_ListeClients[m_cptclient]->m_nom);
    ui->PrenomClient->setText(m_ListeClients[m_cptclient]->m_prenom);
    ui->AdresseClient->setText(m_ListeClients[m_cptclient]->m_adresse);
    ui->CP->setText(QString::number(m_ListeClients[m_cptclient]->m_codepostal));
    ui->Ville->setText(m_ListeClients[m_cptclient]->m_ville);
    ui->TelClient->setText(QString::number(m_ListeClients[m_cptclient]->m_tel).rightJustified(10,'0'));
    ui->MailClient->setText(m_ListeClients[m_cptclient]->m_mail);
}
void OngletClients::AfficherListeBDC()
{

    ui->NbCommandes->display(0);
    m_Commandes = m_affich.AfficherCommandesParClient( m_ListeClients[m_cptclient]->m_id);

    ui->NbCommandes->display(m_Commandes.count());

    for (int cpt = 0 ; cpt < m_Commandes.count() ; cpt++ )
    {
        QListWidgetItem* item = new QListWidgetItem;
        item->setData(Qt::UserRole,cpt);
        item->setText("Commande du "+m_Commandes[cpt]->m_Infos->m_Date+"" );
        ui->ListeBDC->addItem(item);
    }
}

void OngletClients::on_ListeBDC_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (current != NULL)
        AfficherBDCSelectionne(current->data(Qt::UserRole).toInt());
}
void OngletClients::AfficherBDCSelectionne(int id)
{
    BDDCommande* commandeencours = m_Commandes[id];
    float MargeGlobale=0;
    float totalttcclient=0;

    ui->TableauProduits->setRowCount(commandeencours->m_ListeProduits.count()+1);

    for (int i=0;i<commandeencours->m_ListeProduits.count();i++)
    {
        QSharedPointer < BDDProduit > tempProduit = commandeencours->m_ListeProduits[i].second;
        //affichage de la référence
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(tempProduit->m_Ref);
        ui->TableauProduits->setItem(i,0,item);
        //affichage du nom du produit
        item = new QTableWidgetItem;
        item->setText(tempProduit->m_Nom);
        ui->TableauProduits->setItem(i,1,item);
        //Affichage du Prix HT LR
        item = new QTableWidgetItem;
        QString tempStr=tempProduit->m_PUHT ;
        item->setText(tempStr.replace(".","€"));
        item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->TableauProduits->setItem(i,2,item);
        //Affichage du Prix TTC LR
        item = new QTableWidgetItem;
        BDDPrix* prix = new BDDPrix(tempProduit->m_PUHT);
        prix->ApplicationTVA(tempProduit->m_TVA);
        tempStr = prix->m_resultat;
        item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        item->setText(tempStr.replace(".","€"));
        ui->TableauProduits->setItem(i,3,item);
        //Affichage du Prix TTC Clients
        item = new QTableWidgetItem;
        tempStr = tempProduit->m_PUClient;
        item->setText(tempStr.replace(".","€"));
        item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->TableauProduits->setItem(i,4,item);
        //Affichage de la Quantité
        item = new QTableWidgetItem;
        item->setText(QString::number(commandeencours->m_ListeProduits[i].first));
        ui->TableauProduits->setItem(i,5,item);
        item->setTextAlignment(Qt::AlignCenter);
        //Affichage du Total TTC LR
        float TotalLR = prix->m_res * commandeencours->m_ListeProduits[i].first;
        item = new QTableWidgetItem;
        tempStr =QString::number(TotalLR,'f',2);
        item->setText(tempStr.replace(".","€"));
        item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->TableauProduits->setItem(i,6,item);
        //Affichage du Total TTC Clients
        float TotalClient = tempProduit->m_PUClient.toFloat() * commandeencours->m_ListeProduits[i].first;
        item = new QTableWidgetItem;
        tempStr =QString::number(TotalClient,'f',2);
        item->setText(tempStr.replace(".","€"));
        item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->TableauProduits->setItem(i,7,item);

       totalttcclient = totalttcclient + TotalClient;

        if(commandeencours->m_Client->m_id!=1)
        {
        //Affichage de la Marge
        float Marge = TotalClient-TotalLR;
        item = new QTableWidgetItem;
        tempStr =QString::number(Marge,'f',2);
        item->setText(tempStr.replace(".","€"));
        item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->TableauProduits->setItem(i,8,item);

        MargeGlobale = MargeGlobale + Marge;
        }
    }
    //Total Clients
    QTableWidgetItem* item = new QTableWidgetItem;
    QFont serifFont("Times", 12, QFont::Bold);item->setFont( serifFont);
    item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    item->setText(QString::number(totalttcclient,'f',2).replace(".","€"));
    ui->TableauProduits->setItem(commandeencours->m_ListeProduits.count(),7,item);
    if(commandeencours->m_Client->m_id!=1)
    {
        //Total Marge
        item = new QTableWidgetItem;
        item->setText(QString::number(MargeGlobale,'f',2).replace(".","€"));
        item->setFont( serifFont);
        item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->TableauProduits->setItem(commandeencours->m_ListeProduits.count(),8,item);
    }
}
