#include <QVector>
#include <string>
#include <iostream>

#include "classes.hpp"

using namespace std;

Stat::Stat()
{
    rq_recu = 0;
    rq_traite = 0;
    byte_recu = 0;
    byte_envoi = 0;
    actif = true;

    list_error[404] = 0;
    list_error[500] = 0;
    list_rq = new QVector<Requete>;
    adresses = new QVector<QString>;
}

Stat::~Stat()
{
    delete list_rq;
    delete adresses;
}

void Stat::clean()
{
    list_rq->clear();
    adresses->clear();
    list_error.clear();
    list_fichier.clear();

    rq_recu = 0;
    rq_traite = 0;
    byte_recu = 0;
    byte_envoi = 0;
    activate();

    list_error[403] = 0;
    list_error[404] = 0;
    list_error[500] = 0;
    list_error[503] = 0;
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

void Stat::new_client(QString addr)
{
    adresses->append(addr);
}

void Stat::new_byte_recu(int bytes)
{
    byte_recu+=bytes;
}

void Stat::new_byte_envoi(int bytes)
{
    byte_envoi+=bytes;
}

void Stat::new_rq(Requete rq)
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

Page Stat::affiche()
{
    Text_Page p("./public_html/private/statistiques.html");
    p.start_html("Statistiques du serveur");

    p.line("<b>Page de statistiques du serveur</b>");
    p.break_line();
    p.line("Nombre de requetes recus", get_rq_recu());
    p.line("Nombre de requetes traitees", get_rq_traite());
    p.line("Nombre d'erreurs 403", get_error(403));
    p.line("Nombre d'erreurs 404", get_error(404));
    p.line("Nombre d'erreurs 500", get_error(500));
    p.line("Nombre d'erreurs 503", get_error(503));
    p.line("Nombre de clients qui se sont connectes", get_nb_clients());
    p.line("Donnees transmise en octet", get_byte_envoi());
    p.line("Donnees recues en octet", get_byte_recu());

    p.break_line();
    p.line_nobreak("<div>");
        p.line_nobreak("<table align=\"center\"><tr>");

        p.line_nobreak("<td>");
            p.line("<b>Liste des requetes</b>");
            p.line_nobreak("<div style=\"width:600; height:400; overflow: auto;\">");
            p.line_nobreak("<div style=\"width:500; height:400; overflow: auto;\">");
                for (int i =0 ; i<list_rq->size() ; i++)
                    p.line(affiche_rq(i));
            p.line_nobreak("</div></div>");
        p.line_nobreak("</td>");

        p.line_nobreak("<td>");
            p.line("<b>Liste des fichiers</b>");
            p.line_nobreak("<div style=\"width:600; height:400; overflow: auto;\">");
            p.line_nobreak("<div style=\"width:500; height:400; overflow: auto;\">");
                for (int i=0 ; i<list_fichier.size() ; i++)
                {
                    QList<QString> fichiers = list_fichier.keys();
                    p.line(affiche_nb_fichier(fichiers.at(i)));
                }
            p.line_nobreak("</div>");
        p.line_nobreak("</td>");

        p.line_nobreak("</tr></table>");
    p.line_nobreak("</div>");


    p.end_html();
    return p;
}


int Stat::get_nb_clients()
{
    return adresses->size();
}

bool Stat::has_connected(QString addr)
{
    return adresses->contains(addr);
}

QString Stat::affiche_rq(int i)
{
    QString retour;
    switch (list_rq->at(i).get_commande()) {
    case Requete::get:
    case Requete::info:
    case Requete::cache:
    case Requete::clear_cache:
    case Requete::activ:
    case Requete::desactiv:
    case Requete::stats:
    case Requete::clear_stats:
        retour.append("GET ");
        break;
    default:
        retour.append("POST ");
        break;
    }
    retour.append(list_rq->at(i).get_chemin());
    retour.append('\n');
    return retour;
}

QString Stat::affiche_nb_fichier(QString fichier)
{
    QString retour;
    retour.append("Telecharge ");
    retour.append(QString::number(list_fichier.value(fichier)));
    retour.append(" fois : ");

    retour.append(fichier + '\n');
    return retour;
}

void Stat::activate()
{
    actif = true;
}

void Stat::desactivate()
{
    actif = false;
}

bool Stat::get_state()
{
    return actif;
}

Stat* stat_t;
