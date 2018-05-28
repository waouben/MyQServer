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
    f->close();
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
    file->close();
}







Repertoire::Repertoire(Requete* rq, QDir *dir) : Page(rq)
{
    type_t = repertoire;
    bytes.append("<html>\n<head>\n<title>\n");
    bytes.append(dir->dirName());
    bytes.append("</title>\n</head>\n<body>\n");
    for (int i = 0 ; i < dir->entryList().size() ; i++)
    {

        //QString temp = rq->get_chemin();
        QString lien = rq->get_chemin() + dir->entryList().at(i) + '/';
        QDir f(lien);
        bytes.append("<a href=\"");
        /*
        QString ligne = dir->dirName();
        if (ligne != "public_html")
        {
            bytes.append(ligne);
            bytes.append("/");
        }
        */

        bytes.append(dir->entryList().at(i));
        if (f.exists())
            bytes.append("/");
        bytes.append("\">");
        bytes.append(dir->entryList().at(i));
        bytes.append("</a>\n<br>\n");
    }
    bytes.append("</body>\n</html>");
    taille = bytes.size();
}



Repertoire::Repertoire(std::string)
{
    type_t = repertoire;
}







Text_Page::Text_Page(QString URL) : Page(URL)
{
    type_t = texte;
}

void Text_Page::start_html(QString titre)
{
    QString html("<html>\n<head>\n<title>");
    html.append(titre);
    html.append("</title>\n<body>\n");
    bytes.append(html);
}

void Text_Page::end_html()
{
    QString html("</body>\n</html>");
    bytes.append(html);
}

void Text_Page::line(QString texte)
{
    bytes.append(texte);
    bytes.append("\n<br>\n");
}

void Text_Page::line(QString champs, int valeur)
{
    bytes.append(champs + " : ");
    bytes.append(QString::number(valeur));
    bytes.append("\n<br>\n");
}

void Text_Page::line(QString champs, QString valeur)
{
    bytes.append(champs + " : ");
    bytes.append(valeur + '\n');
    bytes.append("\n<br>\n");
}

void Text_Page::line_nobreak(QString texte)
{
    bytes.append(texte);
}

void Text_Page::break_line()
{
    bytes.append("<br>\n");
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

Requete::Requete()
{
    heure = time(NULL);
    error = 200;
    chemin = "Unspecified";
    commande_t = info;
}

Requete::Requete(string cmde, QString _chemin)
{
    chemin = _chemin;
    heure = time(NULL);
    error = 200;                                        //OK
    if (cmde == string("GET"))
    {

        if (chemin == "./public_html/info.html")
        {
            commande_t = info;
        }
        else if (chemin == "./public_html/private/cache.html")
        {
            commande_t = cache;
        }
        else if (chemin == "./public_html/private/clear_cache.html")
        {
            commande_t = clear_cache;
        }
        else if (chemin == "./public_html/private/activate.html")
        {
            commande_t = activ;
        }
        else if (chemin == "./public_html/private/desactivate.html")
        {
            commande_t = desactiv;
        }
        else if (chemin == "./public_html/private/statistiques.html")
        {
            commande_t = stats;
        }
        else if (chemin == "./public_html/private/clear_stats.html")
        {
            commande_t = clear_stats;
        }
        else
            commande_t = get;
    }else{
            commande_t = input;
    }
}

Requete::Requete(string cmde, QString _chemin, QString _body) : Requete(cmde, _chemin)
{
    body = _body;
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
            if (chemin == "./public_html/info.html")
            {
                commande_t = info;
            }
            if (chemin == "./public_html/private/cache.html")
            {
                commande_t = cache;
            }
            if (chemin == "./public_html/private/clear_cache.html")
            {
                commande_t = clear_cache;
            }
            if (chemin == "./public_html/private/activate.html")
            {
                commande_t = activ;
            }
            if (chemin == "./public_html/private/desactivate.html")
            {
                commande_t = desactiv;
            }
            if (chemin == "./public_html/private/statistiques.html")
            {
                commande_t = stats;
            }
            if (chemin == "./public_html/private/clear_stats.html")
            {
                commande_t = clear_stats;
            }
            else
                commande_t = get;
        }
    }
}


enum Requete::commande Requete::get_commande() const
{
    return commande_t;
}

QString Requete::get_chemin() const
{
    return chemin;
}

QString Requete::get_body() const
{
    return body;
}

int Requete::get_error() const
{
    return error;
}

bool Requete::switch_to_get()
{
    if(get_commande() != input)
        return false;
    Requete temp("GET", get_chemin());
    commande_t = temp.get_commande();
    return true;
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
    case 503:
        return "HTTP/1.1 503 Service Unavailable";
        break;
    case 403:
        return "HTTP/1.1 403 Forbidden";
        break;
    case 500:
    default:
        return "HTTP/1.1 500 Internal Server Error\n\n";
        break;
    }
}

void Requete::raise_error(int _error)
{
    error = _error;
    stat_t->new_error(_error);
}
