#ifndef BDDBONCOMMANDE_H
#define BDDBONCOMMANDE_H

#include <QObject>
#include "bddproduit.h"


class BDDInfosCommande : public QObject
{
    Q_OBJECT
public:
    explicit BDDInfosCommande(const QString& NumCommande, const int& client, const QString& Date, const QString& Promo, const QString &Paye, const QString &Commande, const QString &Livre, QObject *parent = 0);
    static BDDInfosCommande* RecupererBonCommande(const int id);

    //Liste des Membres
    int m_id;
    QString m_NumCommande;
    int m_id_client;
    QString m_Date;
    QString m_Promo;
    QString m_Paye;
    QString m_Commande;
    QString m_Livre;

    void updateBDD();
private:
    void ajouterBDD();
    void recupererId();

    //Constructeur avec une id
    BDDInfosCommande(const int id,QObject* parent=NULL);

};

#endif // BDDBONCOMMANDE_H
