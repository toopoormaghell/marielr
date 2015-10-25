#ifndef ONGLETLIVRAISONCLIENTS_H
#define ONGLETLIVRAISONCLIENTS_H

#include <QWidget>

namespace Ui {
class OngletLivraisonClients;
}

class OngletLivraisonClients : public QWidget
{
    Q_OBJECT

public:
    explicit OngletLivraisonClients(QWidget *parent = 0);
    ~OngletLivraisonClients();

private:
    Ui::OngletLivraisonClients *ui;
};

#endif // ONGLETLIVRAISONCLIENTS_H
