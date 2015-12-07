#include "util.h"
#include "bddcommande.h"
#include "bddinfoscommande.h"
#include <QList>


util::util(QObject *parent) : QObject(parent)
{

}

QList<ProduitCom> util::ListeProduits(QList<BDDCommande * > commandes)
{
    QList<ProduitCom> tempListeProduits;

    for (int cpt = 0; cpt < commandes.count() ; cpt ++ )
    {
        for (int cptListeProduits = 0 ; cptListeProduits < commandes[cpt]->m_ListeProduits.count() ; cptListeProduits ++ )
        {
            QSharedPointer < BDDProduit > tempProduit = commandes[cpt]->m_ListeProduits[cptListeProduits].second;

            ProduitCom Produit;
            Produit.Nb_Produit = commandes[cpt]->m_ListeProduits[cptListeProduits].first;
            Produit.Nom_Produit = tempProduit->m_Nom;
            Produit.Ref = tempProduit->m_Ref;

            int position = tempListeProduits.indexOf(Produit,1);
            if ( position == -1)
            {
                tempListeProduits << Produit;

            } else
            {
                tempListeProduits[position].Nb_Produit ++;
            }

        }
    }
    return tempListeProduits;
}
QString util::ChangementDate(QString Date)
{
    QStringList temp = Date.split('.');
    if (temp.count()>2)
        return temp[2]+"."+temp[1]+"."+temp[0];
    else
        return Date;
}
