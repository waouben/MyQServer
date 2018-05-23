#include <QVector>
#include <string>
#include <iostream>

#include "classes.hpp"

using namespace std;

Stat::Stat()
{
    rq_recu = 0;
    rq_traite = 0;
    nb_client = 0;
    byte_recu = 0;
    byte_envoi = 0;

    list_error[404] = 0;
    list_error[500] = 0;
    list_rq = new QVector<Requete*>;
}

Stat::~Stat()
{
    clean();
    delete list_rq;
}

void Stat::clean()
{

}

int Stat::get_rq_recu()
{
    return rq_recu;
}

int Stat::get_rq_traite()
{
    return rq_traite;
}

int Stat::get_error(int error)
{
    return list_error.value(error);
}

int Stat::get_client()
{
    return nb_client;
}

int Stat::get_byte_recu()
{
    return byte_recu;
}

int Stat::get_byte_envoi()
{
    return byte_envoi;
}

void Stat::new_rq_recu()
{
    rq_recu++;
}

void Stat::new_rq_traite()
{
    rq_traite++;
}

void Stat::new_error(int error)
{
    list_error[error]++;
}

void Stat::new_client()
{
    nb_client++;
}

void Stat::new_byte_recu(int bytes)
{
    byte_recu+=bytes;
}

void Stat::new_byte_envoi(int bytes)
{
    byte_envoi+=bytes;
}

void Stat::new_rq(Requete* rq)
{
    list_rq->append(rq);
    new_rq_traite();
}

void Stat::new_fichier(QString nom)
{
    if (list_fichier.contains(nom))
        list_fichier[nom]++;
    else
        list_fichier.insert(nom, 1);
}
