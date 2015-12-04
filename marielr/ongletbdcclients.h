#ifndef ONGLETBDCCLIENTS_H
#define ONGLETBDCCLIENTS_H

#include <QWidget>
#include <QTableWidgetItem>
#include "util.h"
#include "bddaffichage.h"
#include <QSignalMapper>

namespace Ui {
class OngletBDCClients;
}
class BDDCommande;
class QAbstractButton;
class util;
class OngletBDCClients : public QWidget
{
    Q_OBJECT

public:
    explicit OngletBDCClients(QWidget *parent = 0);
    ~OngletBDCClients();

    void AjouterBDC();
    QList<ProduitCom> RecupererProduits();
signals:
    void actu();
private slots:
    void on_NouvelleCommande_clicked();
    void on_ListeClientsEnCours_currentRowChanged(int currentRow);
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_Promo10_clicked();
    void chargementProduit(int row, int column);
    void on_TableauProduits_cellClicked(int row, int column);

    void suppressionProduit(int row);
private:
    Ui::OngletBDCClients *ui;
    QList<BDDCommande*> m_commandesencours;
    int m_row;
    QString m_promo10;
    BDDAffichage m_affich;
    QSignalMapper * m_mapper;
    void CompletationClients();
    void AfficherListeBDC();
    void BDCSelectionne(int cpt);
    void ListeProduitsBDCSelectionne(int cpt);
    void Total();
    void viderBDC();
    float ApplicationPromo(float Total);
    util m_util;
    void MAJOnglet();
    void PrixEnEuros();
    void AjoutLigneProduitVide(int row);
};

#endif // ONGLETBDCCLIENTS_H
