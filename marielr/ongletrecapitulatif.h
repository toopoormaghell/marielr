#ifndef ONGLETRECAPITULATIF_H
#define ONGLETRECAPITULATIF_H

#include <QWidget>
#include "bddaffichage.h"
class BDDComdeLR;
class BDDProduit;
class util;
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
    QString ChoixLR();
    void AffichageMarge();
    void AfficherBDCSelectionne();
    util m_util;
    QList<BDDComdeLR*> m_commandesmois;
    void AfficherProduit(QSharedPointer<BDDProduit> tempProduit, int row, int Qte, int id_client);
    void RecupererCommandesMois();
    void CalculMarge();
    void AfficherPeriode();
};

#endif // ONGLETRECAPITULATIF_H
