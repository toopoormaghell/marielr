#include "bddclient.h"
#include "bddsingleton.h"
#include <QtSql>

BDDClient::BDDClient(const QString &Nom, const QString& Prenom, const QString& Adresse, const QString& Mail, const QString& Ville, const int& CodePostal, const int& Tel, QObject *parent) : QObject(parent),
    m_nom(Nom),
    m_prenom(Prenom),
    m_adresse(Adresse),
    m_mail(Mail),
    m_ville(Ville),
    m_codepostal(CodePostal),
    m_tel(Tel)
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

BDDClient::~BDDClient()
{

}

void BDDClient::updateBDD()
{

   QString queryStr="UPDATE Client SET Prenom ='" + m_prenom +"', Adresse = '"+ m_adresse+"', CodePostal = '"+ QString::number(m_codepostal) +"', Ville = '"+ m_ville+"', Tel = '" + QString::number(m_tel) +"', Mail ='"+ m_mail + "'  WHERE Id_Client='"+ QString::number(m_id) +"'";

      madatabase.exec(queryStr);
}

void BDDClient::supprimerenBDD() const
{
    /*
 *    //On vérifie si l'album existe ou non dans la table des relations
    QString queryStri =  "Select Id_Relation As 'Relation' from Relations_Clients WHERE Id_Album='"+QString::number(m_id)+"'" ;
    QSqlQuery  query2 = madatabase.exec(queryStri);

    //si la deuxième requête ne renvoie pas de résultat, on efface du coup l'album
    if (!query2.first()) {

        madatabase.exec("DELETE FROM Album WHERE Id_Album='"+QString::number(m_id)+"'");

    }
 * */
}

BDDClient *BDDClient::RecupererClient(const int Id)
{
    return new BDDClient(Id);
}

void BDDClient::ajouterBDD()
{
    QString queryStr="INSERT INTO Client VALUES (null,'"+ m_nom+ "','"+ m_prenom+ "','"+ m_adresse+ "','" +QString::number(m_codepostal) + "','" + m_ville +"','" + QString::number(m_tel)+"','"+m_mail+"')";

    QSqlQuery query = madatabase.exec(queryStr);

    m_id=query.lastInsertId().toInt();

}

void BDDClient::recupererId()
{
    QString queryStr = "Select Id_Client As 'Client' from Client WHERE Nom='" + m_nom+"'" ;
    QSqlQuery query = madatabase.exec( queryStr );

    if ( query.first() )
    {
        QSqlRecord rec = query.record();
        m_id = rec.value( "Client" ).toInt();
    } else
    {
        m_id = -1;
    }
}

BDDClient::BDDClient(const int id, QObject *parent):
    QObject(parent),
    m_id(id),
    m_nom(),
    m_prenom(),
    m_adresse(),
    m_mail(),
    m_ville(),
    m_codepostal(0),
    m_tel(0)
{
    QString queryStr="SELECT * FROM Client WHERE Id_Client='"+ QString::number( id )+"'";
    QSqlQuery query = madatabase.exec( queryStr );
    while ( query.next() )
    {
        QSqlRecord rec = query.record();

        m_nom = rec.value("Nom").toString().replace("$","'");
        m_prenom = rec.value("Prenom").toString().replace("$","'");
        m_adresse = rec.value("Adresse").toString().replace("$","'");
        m_mail = rec.value("Mail").toString().replace("$","'");
        m_ville = rec.value("Ville").toString().replace("$","'");
        m_codepostal = rec.value("CodePostal").toInt();
        m_tel = rec.value("Tel").toInt();
    }

}
