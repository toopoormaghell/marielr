#ifndef BDDAFFICHAGE_H
#define BDDAFFICHAGE_H

#include <QObject>
#include "util.h"
class BDDCommande;
class BDDComdeLR;
class BDDAffichage : public QObject
{
    Q_OBJECT

public:
    explicit BDDAffichage(QObject *parent = 0);
    QList<BDDCommande *> AfficherListeCommandesEncours();
    QStringList AfficherListeClients();
    QStringList AfficherListeRef();
    QList<ProduitCom> AfficherListeProduitsACommander(QList<BDDCommande *> commandes);

    QList<BDDComdeLR *> AfficherListeCommandesLREncours();
private:
    QString CreerQueryString(QList<BDDCommande *> commandes);
};

#endif // BDDAFFICHAGE_H
