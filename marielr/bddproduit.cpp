#include "bddproduit.h"
#include "bddsingleton.h"
#include <QtSql>

BDDProduit::BDDProduit(const ProduitCom& prod, QObject *parent) :
    QObject(parent),
    m_id(0),
    m_Ref(prod.Ref),
    m_Nom(prod.Nom_Produit),
    m_PUHT(prod.PUHT),
    m_PUClient(prod.PUC),
    m_TVA(prod.TVA)
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
BDDProduit::~BDDProduit()
{

}
void BDDProduit::updateBDD()
{

    QString queryStr;
    if(m_PUHT!=0)
    {

        queryStr="UPDATE Produit SET PrixUniHT ='" + m_PUHT +"' , TVA='"+ m_TVA +"' , Bloque='Oui'  WHERE Id_Produit='"+ QString::number(m_id) +"'";

    }
    madatabase.exec(queryStr);
}
void BDDProduit::supprimerenBDD() const
{

}
BDDProduit *BDDProduit::RecupererProduit(const int id)
{
    return new BDDProduit(id);
}
void BDDProduit::ajouterBDD()
{
    QString queryStr="INSERT INTO Produit VALUES (null,'"+ m_Ref+ "','"+ m_Nom+ "','"+ m_PUHT+ "','" +m_PUClient + "','" + m_TVA+"','Non')";

    QSqlQuery query = madatabase.exec(queryStr);

    m_id=query.lastInsertId().toInt();
}
void BDDProduit::recupererId()
{
    QString queryStr = "Select Id_Produit As 'Produit' from Produit WHERE Nom_Produit='" + m_Nom+"' AND Ref='" +m_Ref+"' AND Bloque='Non'";
    QSqlQuery query = madatabase.exec( queryStr );

    if ( query.first() )
    {
        QSqlRecord rec = query.record();
        m_id = rec.value( "Produit" ).toInt();
    } else
    {
        m_id = -1;
    }
}
BDDProduit::BDDProduit(const int id, QObject *parent):
    QObject(parent),
    m_id(id),
    m_Ref(),
    m_Nom(),
    m_PUHT(),
    m_PUClient(),
    m_TVA()
{
    QString queryStr="SELECT * FROM Produit WHERE Id_Produit='"+ QString::number( id )+"'";
    QSqlQuery query = madatabase.exec( queryStr );
    while ( query.next() )
    {
        QSqlRecord rec = query.record();

        m_Ref = rec.value("Ref").toString().replace("$","'");
        m_Nom =rec.value("Nom_Produit").toString().replace("$","'");
        float temp = rec.value("PrixUniHT").toFloat();
        m_PUHT = m_PUHT.setNum(temp,'f',2);
        temp = rec.value("PrixUniClient").toFloat();
        m_PUClient= m_PUClient.setNum(temp,'f',2);
        m_TVA= rec.value("TVA").toString();

    }
}
