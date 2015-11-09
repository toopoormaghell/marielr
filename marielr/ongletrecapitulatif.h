#ifndef ONGLETRECAPITULATIF_H
#define ONGLETRECAPITULATIF_H

#include <QWidget>
#include "bddaffichage.h"
class BDDComdeLR;
namespace Ui {
class OngletRecapitulatif;
}

class OngletRecapitulatif : public QWidget
{
    Q_OBJECT

public:
    explicit OngletRecapitulatif(QWidget *parent = 0);
    ~OngletRecapitulatif();
    void ActualiserOnglet();
private slots:
    void on_ListeBDCLR_itemSelectionChanged();

private:
    Ui::OngletRecapitulatif *ui;
    void AfficherListeBDC();
    QList<BDDComdeLR*> m_commandesencours;
    BDDAffichage m_affich;
    float m_Marge;
    float m_TotalTTCClients;
    int ChoixLR();
    void Total();
    void AfficherBDCSelectionne();

};

#endif // ONGLETRECAPITULATIF_H
