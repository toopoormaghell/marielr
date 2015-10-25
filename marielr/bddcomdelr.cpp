#include "bddcomdelr.h"
#include <QtSql>
#include "bddsingleton.h"

BDDComdeLR::BDDComdeLR(const QList<int> &Liste_Id_BDC, const QString &Date, QObject *parent):
    QObject(parent),
    m_id(0),
    m_Date(Date),
    m_Liste_Id_BDC(Liste_Id_BDC)
{
    recupererId();
    if(m_id==-1)
    {
        ajouterBDD();
        entrerProduits();
    }
}

BDDComdeLR *BDDComdeLR::RecupererCommande(const int id)
{
    return new BDDComdeLR(id);
}

void BDDComdeLR::ajouterBDD()
{
    QString queryStr="INSERT INTO LR VALUES (null,'"+ m_Date+ "')";

    QSqlQuery query = madatabase.exec(queryStr);

    m_id=query.lastInsertId().toInt();
}

void BDDComdeLR::recupererId()
{
    QString queryStr = "Select Id_LR from LR WHERE Date='" + m_Date+"'" ;
    QSqlQuery query = madatabase.exec( queryStr );

    if ( query.first() )
    {
        QSqlRecord rec = query.record();
        m_id = rec.value( "Id_LR" ).toInt();
    } else
    {
        m_id = -1;
    }
}

BDDComdeLR::BDDComdeLR(const int id, QObject *parent):
    QObject(parent),
    m_id(id),
    m_Date(),
    m_Liste_Id_BDC()
{
    QString queryStr="SELECT Date FROM LR WHERE Id_LR='"+ QString::number( id )+"'";
    QSqlQuery query = madatabase.exec( queryStr );
    while ( query.next() )
    {
        QSqlRecord rec = query.record();

        m_Date = rec.value("Date").toString();
    }

    queryStr="SELECT Id_BDC FROM Relations_LR WHERE Id_LR='"+ QString::number( id )+"'";
    query = madatabase.exec( queryStr );
    while ( query.next() )
    {
        QSqlRecord rec = query.record();

        m_Liste_Id_BDC << rec.value("Id_BDC").toInt();
    }
}

void BDDComdeLR::entrerProduits()
{

    for (int cpt=0;cpt<m_Liste_Id_BDC.count();cpt++)
    {
        qDebug() << " passe ici";
        int id=0;
        id = recupererProduitId(m_Liste_Id_BDC[cpt]);
        if(id==-1)
        {
            ajouterProduitBDD(m_Liste_Id_BDC[cpt]);
        }
    }
}
int BDDComdeLR::recupererProduitId(int BDC)
{
    QString queryStr = "Select Id_Relation from Relations_LR WHERE Id_BDC='" + QString::number(BDC)+"' AND Id_LR='" + QString::number(m_id)+ "'";
    QSqlQuery query = madatabase.exec( queryStr );

    if ( query.first() )
    {
        QSqlRecord rec = query.record();
        return rec.value( "Id_Relation" ).toInt();
    } else
    {
        return -1;
    }
}
void BDDComdeLR::ajouterProduitBDD(int BDC)
{
    QString queryStr="INSERT INTO Relations_LR VALUES (null,'"+ QString::number(BDC) + "','"+ QString::number(m_id) +"')";

    QSqlQuery query = madatabase.exec(queryStr);
}
