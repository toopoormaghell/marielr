#include "bddaffichage.h"
#include "bddcommande.h"
#include <qtsql>
#include "bddsingleton.h"
#include "util.h"
#include "bddcomdelr.h"
#include "bddclient.h"
BDDAffichage::BDDAffichage(QObject *parent) : QObject(parent)
{

}

QList<BDDCommande*> BDDAffichage::AfficherListeCommandesEncours()
{
    QList<BDDCommande*> commandes;
    QString queryStr="SELECT Id_BDC FROM BDC WHERE Commande='false'";
    QSqlQuery query= madatabase.exec(queryStr);

    while (query.next())
    {
        QSqlRecord rec= query.record();
        commandes << BDDCommande::RecupererCommande(rec.value("Id_BDC").toInt());
    }

    return commandes;
}
QStringList BDDAffichage::AfficherListeClients()
{
    QStringList clients;
    QString queryStr="SELECT Nom FROM Client ";
    QSqlQuery query= madatabase.exec(queryStr);

    while (query.next())
    {
        QSqlRecord rec= query.record();
        clients <<rec.value("Nom").toString().replace("$","'");
    }

    return clients;
}
QStringList BDDAffichage::AfficherListeRef()
{
    QStringList Ref;

    return Ref;
}
QList<ProduitCom > BDDAffichage::AfficherListeProduitsACommander(QList<BDDCommande *> commandes)
{

    QList<ProduitCom> liste;
    QString queryStr="SELECT Ref,Nom_Produit,PrixUniHT,PrixUniClient,TVA,SUM(Nb_Produit) AS 'Nb_Produit' FROM BDC B,Produit P, Relations_Clients R WHERE P.Id_Produit = R.Id_Produit AND B.Id_BDC = R.Id_BDC AND " + CreerQueryString(commandes) + " GROUP BY R.Id_Produit ORDER BY R.Id_Produit";

    QSqlQuery query= madatabase.exec(queryStr);

    while (query.next())
    {
        ProduitCom temp;
        QSqlRecord rec= query.record();

        temp.Nb_Produit = rec.value("Nb_Produit").toInt();
        temp.Nom_Produit = rec.value("Nom_Produit").toString().replace("$","'");
        temp.PUC = rec.value("PrixUniClient").toFloat();
        float prixtemp = rec.value("PrixUniHT").toFloat();
        temp.PUHT = temp.PUHT.setNum(prixtemp,'f',2);
        prixtemp = rec.value("TVA").toFloat();
        temp.TVA = temp.TVA.setNum(prixtemp,'f',2);
        temp.Ref = rec.value("Ref").toString().replace("$","'");

        liste << temp;
    }
    return liste;
}
QString BDDAffichage::CreerQueryString(QList<BDDCommande *> commandes)
{
    QString temp="(";

    temp += " B.Id_BDC='" + QString::number(commandes[0]->m_id)+"'";
    for (int cpt=1;cpt<commandes.count();cpt++)
    {
        temp += " OR B.Id_BDC='" + QString::number(commandes[cpt]->m_id)+"'";
    }
    temp +=")";
    return temp;
}
QList<BDDComdeLR *> BDDAffichage::AfficherListeCommandesLREncours()
{
    QList<BDDComdeLR*> commandes;
    QString queryStr="SELECT Id_LR FROM LR ";
    QSqlQuery query= madatabase.exec(queryStr);

    while (query.next())
    {
        QSqlRecord rec= query.record();
        commandes << BDDComdeLR::RecupererCommande(rec.value("Id_LR").toInt());
    }
    return commandes;
}
QList<BDDClient *> BDDAffichage::RecupererListeClients()
{
    QList< BDDClient * > ListeClients ;
    QString queryStr="SELECT Id_Client FROM Client ORDER BY Nom";
    QSqlQuery query= madatabase.exec(queryStr);

    while (query.next())
    {
        QSqlRecord rec= query.record();
        ListeClients << BDDClient::RecupererClient(rec.value("Id_Client").toInt());
    }

    return ListeClients;
}
QList<BDDCommande*> BDDAffichage::AfficherCommandesParClient(int Client)
{
    QList<BDDCommande*> commandes;
    QString queryStr="SELECT Id_BDC FROM BDC WHERE Id_Client='"+QString::number(Client)+"' ORDER BY Id_BDC";
    QSqlQuery query= madatabase.exec(queryStr);

    while (query.next())
    {
        QSqlRecord rec= query.record();
        commandes << BDDCommande::RecupererCommande(rec.value("Id_BDC").toInt());
    }

    return commandes;
}
