#ifndef BDDPRIX_H
#define BDDPRIX_H

#include <QObject>

class BDDPrix : public QObject
{
    Q_OBJECT

public:
    explicit BDDPrix(QString prix, QObject *parent=0);

    int m_prix;
    QString m_resultat;
    int m_TVA;
    float m_res;

   void Quantite(int qte);
   void ApplicationTVA(QString TVA);
   void ApplicationReduc(int Pourcent);
private:
    int EnInt(QString prix);

};

#endif // BDDPRIX_H
