#include "bddprix.h"
#include <QDebug>
#include <QtMath>

BDDPrix::BDDPrix(QString prix, QString TVA,  QObject *parent) :
    QObject(parent),
    m_prix(),
    m_resultat(),
    m_TVA()

{
    m_prix= EnInt(prix);
    m_TVA = EnInt(TVA);

    ApplicationTVA();
}
int BDDPrix::EnInt(QString prix)
{

    float temp = std::floor(prix.toFloat()*100+0.5);


    return temp;
}

void BDDPrix::ApplicationTVA()
{
    int res = m_prix*m_TVA/1000;
    float resultat = res;
    resultat = resultat/1000;
    QString TVA;
    TVA.setNum(resultat,'f',2);
    float PrixTTC = m_prix;
    PrixTTC = PrixTTC/100;
    m_resultat = m_resultat.setNum(PrixTTC,'f',2);
    PrixTTC = m_resultat.toFloat() + TVA.toFloat();

    m_resultat = m_resultat.setNum(PrixTTC,'f',2);

}

