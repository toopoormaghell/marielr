#include "bddgestion.h"
#include "bddsingleton.h"
#include "bddclient.h"
#include <QDebug>
#include <QSharedPointer>
#include "bddproduit.h"
#include "bddinfoscommande.h"
#include "util.h"
#include "bddcommande.h"
#include "bddcomdelr.h"

BDDGestion::BDDGestion(QObject *parent) : QObject(parent)
{

}

void BDDGestion::AjouterBDC(QString Nom_Client,QList<ProduitCom> ListeProduits,QString Num_Commande, QString Date,QString Promo, const QString& Paye, const QString& Commande, const QString& Livre )
{
    BDDClient client(Nom_Client,"","","","",0,0);
    BDDInfosCommande Infos(Num_Commande,client.m_id,Date,Promo,Paye,Commande,Livre);
    QList < QPair <int, QSharedPointer<BDDProduit> > > ListeBDD;

    for(int cpt=0;cpt<ListeProduits.count();cpt++)
    {
        QPair<int, QSharedPointer<BDDProduit> > maPaire;

        QSharedPointer<BDDProduit> temp( new BDDProduit( ListeProduits[cpt] ) );
        maPaire.second = temp;
        maPaire.first = ListeProduits[cpt].Nb_Produit;

        ListeBDD << maPaire;
    }

    BDDCommande comm(ListeBDD,&Infos);

}

void BDDGestion::MettreAJourProduits(QList<ProduitCom> ListeProduits)
{
    for(int cpt=0;cpt<ListeProduits.count();cpt++)
    {
        QSharedPointer<BDDProduit> temp( new BDDProduit( ListeProduits[cpt] ) );

    }
}

void BDDGestion::AjouterBDCLR(QList<BDDCommande*> listecommandes, QString Date)
{
    QList<int> liste;

    for (int cpt=0;cpt<listecommandes.count();cpt ++)
    {
        liste << listecommandes[cpt]->m_id;
      const  BDDInfosCommande* recupliste = listecommandes[cpt]->m_Infos;
        BDDInfosCommande* temp (new BDDInfosCommande(recupliste->m_NumCommande,recupliste->m_id_client,recupliste->m_Date,recupliste->m_Promo,recupliste->m_Paye,"true","false"));
        Q_UNUSED(temp);

    }
    BDDComdeLR lr(liste,Date);
}

