#include "bddprix.h"
#include <QDebug>
#include <QtMath>

BDDPrix::BDDPrix(QString prix,QObject *parent):
    QObject(parent),
    m_prix(),
    m_resultat(),
    m_TVA()
{
    m_prix = EnInt(prix);
    m_resultat= prix;
}


int BDDPrix::EnInt(QString prix)
{
    float temp = std::floor(prix.toFloat()*100+0.5);
    return temp;
}

void BDDPrix::ApplicationTVA(QString TVA)
{
    m_TVA= EnInt(TVA);
    int res = m_prix*m_TVA/1000;
    float resultat = res;
    resultat = resultat/1000;

    TVA.setNum(resultat,'f',2);
    float PrixTTC = m_prix;
    PrixTTC = PrixTTC/100;
    m_resultat = m_resultat.setNum(PrixTTC,'f',2);
    PrixTTC = m_resultat.toFloat() + TVA.toFloat();

    m_resultat = m_resultat.setNum(PrixTTC,'f',2);
    m_res = m_resultat.toFloat();

}

void BDDPrix::ApplicationReduc(int Pourcent)
{
    m_TVA = Pourcent*100;
    int res = m_prix*m_TVA/1000;
    float resultat = res;
    resultat = resultat/1000;
    QString TVA;
    TVA.setNum(resultat,'f',2);
    float PrixTTC = m_prix;
    PrixTTC = PrixTTC/100;
    m_resultat = m_resultat.setNum(PrixTTC,'f',2);
    PrixTTC = m_resultat.toFloat() - TVA.toFloat();

    m_resultat = m_resultat.setNum(PrixTTC,'f',2);
}
void BDDPrix::Quantite(int qte)
{
     m_prix = EnInt(m_resultat);
    int temp = m_prix * qte;
    float resultat = temp;
    resultat = resultat/100;
    m_resultat = m_resultat.setNum(resultat,'f',2);
    m_res = m_resultat.toFloat();
}
