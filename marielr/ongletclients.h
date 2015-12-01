#ifndef ONGLETCLIENTS_H
#define ONGLETCLIENTS_H

#include <QWidget>
#include "util.h"
#include "bddaffichage.h"
#include <QListWidgetItem>
#include "bddcommande.h"

namespace Ui {
class OngletClients;
}
class BDDClients;
class OngletClients : public QWidget
{
    Q_OBJECT

public:
    explicit OngletClients(QWidget *parent = 0);
    ~OngletClients();
    void ActualiserOnglet();

private slots:

    void on_ListeClients_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_ListeBDC_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void AfficherListeClients();
    Ui::OngletClients *ui;
    BDDAffichage m_affich;
    int m_cptclient;
    QList< BDDClient * > m_ListeClients ;
    void AfficheRenseignementsClient();
    void AfficherListeBDC();
    QList<BDDCommande *> m_Commandes;
    void AfficherBDCSelectionne(int id);
};

#endif // ONGLETCLIENTS_H
