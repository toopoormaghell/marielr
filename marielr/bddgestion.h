#ifndef BDDGESTION_H
#define BDDGESTION_H

#include <QObject>
#include "util.h"
class BDDCommande;
class BDDGestion : public QObject
{
    Q_OBJECT
public:
    explicit BDDGestion(QObject *parent = 0);

    void AjouterBDC(QString Nom_Client, QList<ProduitCom> ListeProduits, QString Num_Commande, QString Date, QString Promo, const QString &Paye, const QString &Commande, const QString &Livre);
    void MettreAJourProduits(QList<ProduitCom> ListeProduits);
    void AjouterBDCLR(QList<BDDCommande *> listecommandes, QString Date);

};

#endif // BDDGESTION_H
