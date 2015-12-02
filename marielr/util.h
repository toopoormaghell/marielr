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
    bool operator==( const ProduitCom& other ) const
    {
        return ( this->Ref == other.Ref );
    }
    bool operator<( const ProduitCom& other ) const
    {
        return ( this->Nb_Produit < other.Nb_Produit );
    }
};

typedef struct ProduitCom ProduitCom;

class BDDCommande;
class util: public QObject
{
    Q_OBJECT

public:
    explicit util(QObject *parent = 0);
    QList<ProduitCom> ListeProduits(QList<BDDCommande *> commandes);
};



#endif // UTIL_H
