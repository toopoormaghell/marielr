#include "bddinfoscommande.h"
#include "bddsingleton.h"
#include <QtSql>
#include "bddclient.h"
#include "bddproduit.h"

BDDInfosCommande::BDDInfosCommande(const QString &NumCommande, const int &client, const QString &Date, const QString &Promo, const QString& Paye, const QString &Commande, const QString &Livre, QObject *parent) :
    QObject(parent),
    m_id(0),
    m_NumCommande(NumCommande),
    m_id_client(client),
    m_Date(Date),
    m_Promo(Promo),
    m_Paye(Paye),
    m_Commande(Commande),
    m_Livre(Livre)
{

    recupererId();
    if(m_id==-1)
    {

        ajouterBDD();
    } else
    {
        updateBDD();
    }
}
void BDDInfosCommande::updateBDD()
{
    QString queryStr;
    if(m_Commande=="true" && m_Livre=="false")
    {
  queryStr="UPDATE BDC SET Commande ='" + m_Commande +"' WHERE Id_BDC='"+ QString::number(m_id) +"'";
    }
      madatabase.exec(queryStr);
}

BDDInfosCommande *BDDInfosCommande::RecupererBonCommande(const int id)
{
    return new BDDInfosCommande(id);
}

void BDDInfosCommande::ajouterBDD()
{
    m_Date.replace("/",".");

    QString queryStr="INSERT INTO BDC VALUES(null,'" + m_NumCommande+ "','" + QString::number(m_id_client)+ "','" + m_Date+ "','" + m_Promo+ "','" + m_Paye+ "','" + m_Commande+ "','" + m_Livre+ "')";

    QSqlQuery query = madatabase.exec(queryStr);
    m_id=query.lastInsertId().toInt();
}

void BDDInfosCommande::recupererId()
{
    QString queryStr = "Select Id_BDC As 'Id_BDC' from BDC WHERE Num_Commande='" + m_NumCommande +"'" ;
    QSqlQuery query = madatabase.exec( queryStr );

    if ( query.first() )
    {
        QSqlRecord rec = query.record();
        m_id = rec.value( "Id_BDC" ).toInt();
    } else
    {
        m_id = -1;
    }
}

BDDInfosCommande::BDDInfosCommande(const int id, QObject *parent):
    QObject(parent),
    m_id(id),
    m_NumCommande(),
    m_id_client(),
    m_Date(),
    m_Promo(),
    m_Paye("false"),
    m_Commande("false"),
    m_Livre("false")
{
    QString queryStr="SELECT Num_Commande, Id_Client, Date, Promo, Paye, Commande, Livre FROM BDC WHERE Id_BDC='"+ QString::number( id )+"'";
    QSqlQuery query = madatabase.exec( queryStr );
    while ( query.next() )
    {
        QSqlRecord rec = query.record();

        m_NumCommande = rec.value("Num_Commande").toString().replace("$","'");
        m_id_client = rec.value("Id_Client").toInt();
        m_Date = rec.value("Date").toString();
        m_Promo = rec.value("Promo").toString();
        m_Paye =rec.value("Paye").toString();
        m_Commande=rec.value("Commande").toString();
        m_Livre=rec.value("Livre").toString();

    }
}

