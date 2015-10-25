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

private:

    void ApplicationTVA();
    int EnInt(QString prix);
};

#endif // BDDPRIX_H
