#ifndef BDDPRODUIT_H
#define BDDPRODUIT_H

#include <QObject>
#include "util.h"

class BDDProduit : public QObject
{
    Q_OBJECT
public:
    explicit BDDProduit(const ProduitCom &prod, QObject *parent = 0);
    virtual ~BDDProduit();

    void updateBDD();
    void supprimerenBDD() const;
    static BDDProduit* RecupererProduit(const int id);

    int m_id;
    QString m_Ref;
    QString m_Nom;
    QString m_PUHT;
    QString m_PUClient;
    QString m_TVA;

private:
    void ajouterBDD();
    void recupererId();

    //Constructeur avec une id
    BDDProduit(const int id,QObject* parent=NULL);
};

#endif // BDDPRODUIT_H
