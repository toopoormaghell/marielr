#ifndef BDDCOMDELR_H
#define BDDCOMDELR_H

#include <QObject>

class BDDComdeLR : public QObject
{
    Q_OBJECT
public:
    explicit BDDComdeLR(const QList<int> &Liste_Id_BDC, const QString &Date, QObject *parent = 0);
    static BDDComdeLR* RecupererCommande(const int id);

    //Liste des Membres
    int m_id;
    QString m_Date;
    QList< int > m_Liste_Id_BDC;


private:
    void ajouterBDD();
    void recupererId();

    //Constructeur avec une id
    BDDComdeLR(const int id,QObject* parent=NULL);

    void entrerProduits();
    int recupererProduitId(int BDC);
    void ajouterProduitBDD(int BDC);
};

#endif // BDDCOMDELR_H
