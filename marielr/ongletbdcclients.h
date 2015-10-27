#ifndef ONGLETBDCCLIENTS_H
#define ONGLETBDCCLIENTS_H

#include <QWidget>
#include <QStandardItemModel>
#include "util.h"
#include "bddaffichage.h"
namespace Ui {
class OngletBDCClients;
}
class BDDCommande;
class QAbstractButton;
class OngletBDCClients : public QWidget
{
    Q_OBJECT

public:
    explicit OngletBDCClients(QWidget *parent = 0);
    ~OngletBDCClients();

    void AjouterBDC();
    QList<ProduitCom> RecupererProduits();
private slots:

    void on_NouvelleCommande_clicked();

    void on_ListeClientsEnCours_currentRowChanged(int currentRow);

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_TableauProduits_clicked(const QModelIndex &index);

    void on_Promo10_clicked();

private:
    Ui::OngletBDCClients *ui;
    QList<BDDCommande*> m_commandesencours;
    int m_row;
    QStandardItemModel * modele;
    QString m_promo10;

    void CompletationClients();
    void AjustementOnglet();
    void AfficherListeBDC();
    void BDCSelectionne(int cpt);
    void ListeProduitsBDCSelectionne(int cpt);
    void Total();
    void viderBDC();
    void ModeleTableau();
    void BDCEnCours();
    BDDAffichage m_affich;
    float ApplicationPromo(float Total);
};

#endif // ONGLETBDCCLIENTS_H
