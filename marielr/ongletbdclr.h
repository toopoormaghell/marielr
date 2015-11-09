#ifndef ONGLETBDCLR_H
#define ONGLETBDCLR_H

#include <QWidget>
#include "bddaffichage.h"
#include <QAbstractButton>
class BDDCommande;
namespace Ui {
class OngletBDCLR;
}
class BDDProduit;
class OngletBDCLR : public QWidget
{
    Q_OBJECT

public:
    explicit OngletBDCLR(QWidget *parent = 0);
    ~OngletBDCLR();

    void ActualiserOnglet();
signals:
    void actu();
private slots:
    void on_TableauProduits_clicked(const QModelIndex &index);

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::OngletBDCLR *ui;
    int m_row;
    BDDAffichage m_affich;
    QList<BDDProduit*> m_produits;
    void RemplirTableau();
    void Total();
    void AjouterBDC();
    QList<ProduitCom> RecupererProduits();
    void AfficherListeBDC();
    QList<BDDCommande*> m_commandesencours;
    QList<int> RecupererCommandes();
};

#endif // ONGLETBDCLR_H
