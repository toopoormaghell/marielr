#include "bddcommande.h"
#include "bddinfoscommande.h"
#include <QtSql>
#include <QSharedPointer>
#include "bddsingleton.h"
#include "bddclient.h"

BDDCommande::BDDCommande( const QList<QPair<int,QSharedPointer<BDDProduit> > >& ListeProduits, const BDDInfosCommande *Infos, QObject *parent) :
    QObject(parent),
    m_id(),
    m_Infos(Infos),
    m_Client(),
    m_ListeProduits(ListeProduits)
{
    for (int cpt=0;cpt<m_ListeProduits.count();cpt++)
    {
        if(recupererId(cpt)==-1)
        {
            ajouterBDD(cpt);
        } else
        {
            updateBDD(cpt);
        }
    }}
void BDDCommande::ajouterBDD(int cpt)
{
    QString queryStr="INSERT INTO Relations_Clients VALUES (null,'"+ QString::number(m_Infos->m_id) + "','"+ QString::number(m_ListeProduits[cpt].second->m_id) + "','"+ QString::number(m_ListeProduits[cpt].first) +"')";

    madatabase.exec(queryStr);
}

int BDDCommande::recupererId(int cpt)
{
    QString queryStr = "Select Id_BDC from Relations_Clients WHERE Id_Produit='" + QString::number(m_ListeProduits[cpt].second->m_id) +"' AND Id_BDC='" +QString::number(m_Infos->m_id)+ "'" ;
    QSqlQuery query = madatabase.exec( queryStr );

    if ( query.first() )
    {
        QSqlRecord rec = query.record();
        return rec.value( "Id_BDC" ).toInt();
    } else
    {
        return -1;
    }
}
void BDDCommande::updateBDD(int cpt)
{
    QString queryStr="UPDATE Relations_Clients SET  Nb_Produit = '"+ QString::number(m_ListeProduits[cpt].first) +"' WHERE Id_BDC='"+ QString::number(m_Infos->m_id) + "' AND '"+QString::number(m_ListeProduits[cpt].second->m_id)+"'";

    madatabase.exec(queryStr);
}
BDDCommande *BDDCommande::RecupererCommande(const int id)
{
    return new BDDCommande(id);
}


BDDCommande::BDDCommande(const int id, QObject *parent):
    QObject(parent),
    m_id(id),
    m_Infos(),
    m_Client(),
    m_ListeProduits()
{

    //On récupère les infos de la commande et le client
    m_Infos = BDDInfosCommande::RecupererBonCommande(id);
    m_Client= BDDClient::RecupererClient(m_Infos->m_id_client);

    //On récupère maintenant la liste des produits de la commande
    QString queryStr="SELECT Id_Produit, Nb_Produit FROM Relations_Clients WHERE Id_BDC='"+ QString::number( id )+"'";
    QSqlQuery query = madatabase.exec( queryStr );
    while ( query.next() )
    {
        QSqlRecord rec = query.record();
        QPair<int,BDDProduit*> maPaire;

        maPaire.first=rec.value("Nb_Produit").toInt();
        maPaire.second=BDDProduit::RecupererProduit(rec.value("Id_Produit").toInt());

        m_ListeProduits << maPaire;
    }
}
