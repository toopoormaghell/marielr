#ifndef BDDCOMMANDE_H
#define BDDCOMMANDE_H

#include <QObject>
#include <QPair>
#include <QList>
#include <QSharedPointer>
#include <QMap>
class BDDClient;
class BDDInfosCommande;
class BDDProduit;

class BDDCommande : public QObject
{
    Q_OBJECT
public:
    explicit BDDCommande(const QList<QPair<int, QSharedPointer<BDDProduit> > > &ConstruireListeProduits, BDDInfosCommande const* Infos, QObject *parent = NULL);
    static BDDCommande* RecupererCommande(const int id);
    void updateBDD(int cpt);

    //Liste des Membres
    int m_id;
    BDDInfosCommande const* m_Infos;
    BDDClient const* m_Client;
    QList< QPair<int,QSharedPointer<BDDProduit> > > m_ListeProduits;

private :
    void ajouterBDD(int cpt);
    int recupererId(int cpt);

    //Constructeur avec une id
    BDDCommande(const int id, QObject* parent=NULL);
    QMap<int, int> ConstruireListeProduits();
    void supprimerenBDD(int Id);
};

#endif // BDDCOMMANDE_H
