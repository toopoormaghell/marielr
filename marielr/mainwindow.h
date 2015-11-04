#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ongletbdcclients.h"
#include "ongletbdclr.h"
#include "ongletlivraisonclients.h"
#include "ongletrecapitulatif.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void changementOngletBDCClients();
    void changementOngletBDCLR();
private:
    Ui::MainWindow *ui;
    OngletBDCClients* m_ongletbdcclients;
    OngletBDCLR* m_ongletbdclr;
    OngletLivraisonClients* m_ongletlivraison;
    OngletRecapitulatif* m_ongletrecap;
};

#endif // MAINWINDOW_H
