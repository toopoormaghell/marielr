#ifndef UTIL_H
#define UTIL_H

#include <QObject>

struct ProduitCom
{
    QString Ref;
    QString Nom_Produit;
    QString PUHT;
    QString PUC;
    QString TVA;
    int Nb_Produit;

    ProduitCom():
        Ref(),
        Nom_Produit(),
        PUHT(),
        PUC(),
        TVA(),
        Nb_Produit(0)
    {}
};
typedef struct ProduitCom ProduitCom;
#endif // UTIL_H
