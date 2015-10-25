#ifndef BDDCLIENT_H
#define BDDCLIENT_H

#include <QObject>

class BDDClient : public QObject
{
    Q_OBJECT
public:
    explicit BDDClient(const QString& Nom, const QString &Prenom, const QString &Adresse, const QString &Mail, const QString &Ville, const int &CodePostal, const int &Tel, QObject *parent = 0);
    virtual ~BDDClient();

    void updateBDD();
    void supprimerenBDD() const;
    static BDDClient* RecupererClient(const int Id);


    int m_id;
    QString m_nom;
    QString m_prenom;
    QString m_adresse;
    QString m_mail;
    QString m_ville;
    int m_codepostal;
    int m_tel;

private:
    void ajouterBDD();
    void recupererId();

    //Constructeur avec une id
    BDDClient(const int id,QObject* parent= NULL);
};

#endif // BDDCLIENT_H
