#include <string>
#include <iostream>
#include <ctime>

#include <QFile>

#include "classes.hpp"

using namespace std;

int Page::next_id = 0;

Page::Page()
{
    id = next_id;
    next_id++;
    heure = time(NULL);
}

Page::Page(QString URL_arg) //Premier draft
{
    nom = URL_arg;
	id = next_id;
	next_id++;
    URL = URL_arg;
	heure = time(NULL);
}

Page::Page(Requete* rq)
{
    nom = rq->get_chemin();
    URL = rq->get_chemin();
    id = next_id;
    next_id++;
    heure = time(NULL);
}

Page::~Page()
{
}

Fichier::Fichier(QString URL_arg)
{
	type_t = fichier;
    QFile* f = new QFile(URL_arg);
    taille = (int)(f->size());
    bytes = QByteArray(f->readAll());
}

Fichier::Fichier(Requete* rq, QFile *file) : Page(rq)
{

    type_t = fichier;
    taille = (int)(file->size());

    if (!file->open(QIODevice::ReadOnly))
    {
            delete file;
            rq->raise_error(500);
            return;
    }

    bytes = QByteArray(file->readAll());
}


Repertoire::Repertoire(QDir *dir)
{
    type_t = repertoire;
    for (int i = 0 ; i < dir->entryList().size() ; i++)
    {
        bytes.append(dir->entryList().at(i));
        bytes.append('\n');
    }
    taille = bytes.size();
}



Repertoire::Repertoire(std::string)
{
    type_t = repertoire;
}

Text_Page::Text_Page(QString URL)
{
    type_t = texte;
}

QString Page::get_name()
{
	return nom;
}

int Page::get_id()
{
	return id;
}

int Page::get_taille()
{
	return taille;
}

Page::type Page::get_type()
{
    return type_t;
}


time_t Page::get_heure()
{
	return heure;
}

QByteArray Page::get_bytes()
{
    return bytes;
}


Requete::Requete(string cmde, QString _chemin)
{
    chemin = _chemin;
    heure = time(NULL);
    error = 200;                                        //OK
    if (cmde == string("GET"))
    {

        if (chemin == "/")
        {
            commande_t = info;
        }
        if (chemin == "/private/cache.html")
        {
            commande_t = cache;
        }
        if (chemin == "/private/clear_cache.html")
        {
            commande_t = clear_cache;
        }
        if (chemin == "/private/activate.html")
        {
            commande_t = activ;
        }
        if (chemin == "/private/desactivate.html")
        {
            commande_t = desactiv;
        }
        if (chemin == "/private/statistiques.html")
        {
            commande_t = stats;
        }
        if (chemin == "/private/clear_stats.html")
        {
            commande_t = clear_stats;
        }
        else
            commande_t = get;
    }
}


Requete::Requete(string cmde, QString _chemin, bool isFile)
{
    chemin = _chemin;
    heure = time(NULL);
    error = 200;                                        //OK
    if (cmde == string("GET"))
    {
        if (isFile)
        {
            if (chemin == "/")
            {
                commande_t = info;
            }
            if (chemin == "/private/cache.html")
            {
                commande_t = cache;
            }
            if (chemin == "/private/clear_cache.html")
            {
                commande_t = clear_cache;
            }
            if (chemin == "/private/activate.html")
            {
                commande_t = activ;
            }
            if (chemin == "/private/desactivate.html")
            {
                commande_t = desactiv;
            }
            if (chemin == "/private/statistiques.html")
            {
                commande_t = stats;
            }
            if (chemin == "/private/clear_stats.html")
            {
                commande_t = clear_stats;
            }
            else
                commande_t = get;
        }
    }
}


enum Requete::commande Requete::get_commande()
{
    return commande_t;
}

QString Requete::get_chemin()
{
    return chemin;
}

int Requete::get_error()
{
    return error;
}

const char* Requete::http_reponse()
{
    switch (get_error()) {
    case 200:
        return "HTTP/1.1 200 OK\n\n";
        break;
    case 404:
        return "HTTP/1.1 404 Not Found\n\n";
        break;
    default:
        return "HTTP/1.1 500 Internal Server Error\n\n";
        break;
    }
}

void Requete::raise_error(int _error)
{
    error = _error;
}
