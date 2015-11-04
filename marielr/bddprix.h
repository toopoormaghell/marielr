#ifndef BDDPRIX_H
#define BDDPRIX_H

#include <QObject>

class BDDPrix : public QObject
{
    Q_OBJECT

public:
    explicit BDDPrix(QString prix, QString TVA, QObject *parent = 0);

    int m_prix;
    QString m_resultat;
    int m_TVA;
float m_res;
explicit BDDPrix(QString prix, int Pourcent, QObject *parent=0);
private:

    void ApplicationTVA();
    int EnInt(QString prix);
    void ApplicationReduc();
};

#endif // BDDPRIX_H
