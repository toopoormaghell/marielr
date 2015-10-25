#include "bddsingleton.h"

#include <QStringList>
#include <qdebug.h>
#include <QtSql>

BDDSingleton BDDSingleton::s_singleton;

BDDSingleton::BDDSingleton():
    m_database()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setHostName("localhost");
    m_database.setDatabaseName("MarieLR.db");
    m_database.setUserName("root");
    m_database.setPassword("");

    //Si la BDD n'est pas ouverte,elle affiche une erreur
    if(!m_database.open())
    {
        qDebug() << "Impossible d'ouvrir la base de données !";
    }

    QStringList tables = m_database.tables();
    if (tables.count()==0)
    {

        creationBase();
    }
}
BDDSingleton::~BDDSingleton()
{
    m_database.close();
}

BDDSingleton& BDDSingleton::getInstance()
{
    return s_singleton;
}
QSqlDatabase &BDDSingleton::getDataBase()
{
    return m_database;
}
void BDDSingleton::creationBase()
{
    QStringList tables;
    QSqlQuery query;
    tables << "CREATE TABLE Client ('Id_Client' INTEGER PRIMARY KEY,'Nom' VARCHAR(255),'Prenom' VARCHAR(255),'Adresse' TEXT, 'CodePostal' INTEGER, 'Ville' VARCHAR(255), 'Tel' INTEGER, 'Mail' VARCHAR(255))";
    tables << "CREATE TABLE Produit ('Id_Produit' INTEGER PRIMARY KEY,'Ref' VARCHAR(255),'Nom_Produit' VARCHAR(512),'PrixUniHT' FLOAT, 'PrixUniClient' FLOAT, 'TVA' FLOAT, 'Bloque' VARCHAR(255)  )";
    tables << "CREATE TABLE BDC ('Id_BDC' INTEGER PRIMARY KEY,'Num_Commande' VARCHAR(255),'Id_Client' INTEGER,'Date' VARCHAR(255),'Promo' VARCHAR(255), 'Paye' boolean, 'Commande' boolean, 'Livre' boolean)";
    tables << "CREATE TABLE Relations_Clients ('Id_Relation' INTEGER PRIMARY KEY,'Id_BDC' INTEGER,'Id_Produit' INTEGER, 'Nb_Produit' INTEGER)";
    tables << "CREATE TABLE Configuration ('Categorie' VARCHAR(255),'Valeur' VARCHAR(255))";
    tables << "INSERT INTO Configuration VALUES('Version','1.0')";
    tables << "CREATE Table Relations_LR ('Id_Relation' INTEGER PRIMARY KEY,'Id_BDC' INTEGER,'Id_LR' INTEGER)";
    tables << "CREATE TABLE LR('Id_LR' INTEGER PRIMARY KEY,'Date' VARCHAR(255))";
    for (int i=0;i<tables.size();i++)
    {
        query = madatabase.exec(tables[i]);
    }
}
void BDDSingleton::viderBDD()
{
    QStringList tables;
    QSqlQuery query;
    //Vidage de la base de données.
    tables <<  "Client" << "Produit" << "BDC" << "Relations_Clients" << "Configuration" << "Relations_LR" << "LR";
    int compt=0;
    while (compt<tables.size())
    {
        query = madatabase.exec("DROP Table " + tables[compt]);
        compt++;
    }

    creationBase();

}
