#include <string>
#include <iostream>
#include <QDir>
#include "classes.hpp"

using namespace std;

Cache::Cache()
{
	free_mem = total_mem;
    newest = "does not exist yet";
    oldest = "does not exist yet";
}

Cache::~Cache()
{
    clean();
}


int Cache::mem_restante()
{
	return free_mem;
}

int Cache::mem_occupe()
{
    return total_mem-mem_restante();
}

void Cache::add_page(Page* page, QString URL)  //Attention j'ai pas codé le fait de revenir au début du tableau
{

    while(mem_restante() < page->get_taille())
	{
        free_mem += hash.value(oldest)->page->get_taille();
        QString temp = oldest;
        oldest = hash.value(oldest)->next;

        delete hash.value(temp);
        hash.remove(temp);
	}
    struct list_page* new_page = new struct list_page;
    new_page->next = "does not exist yet";
    new_page->prev = newest;
    new_page->page = page;
    hash.insert(URL, new_page);
    if(!hash.contains(oldest))
        oldest = URL;
    if(hash.contains(newest))
    {
        hash.value(newest)->next = URL;
    }
    newest = URL;
    free_mem -= page->get_taille();
}
/*
Page Cache::affiche()
{
	//Peut etre redondant avec affiche_page()
}
*/
Page Cache::affiche_page(Requete* rq)
{
    QFile f(rq->get_chemin());
    QDir d(rq->get_chemin());
    QFile e503("./public_html/error_503.html");

    Text_Page* p;

    if(!stat_t->get_state())
    {
        rq->raise_error(503);

        return Fichier(rq, &e503);
    }

    switch(rq->get_commande())
	{
        case Requete::get	:
		//case Page                 //Place réservé pour les erreurs


                if( f.exists() == false &&  d.exists() == false ){
                    // ERREUR 404 LE FICHIER N'EXISTE PAS...
                    rq->raise_error(404);
                    if (hash.contains("erreur_404"))
                    {
                        up("erreur_404");
                        return *(hash.value("erreur_404")->page);
                    }
                    else
                    {
                        add_page(new Fichier(rq, new QFile("./public_html/error_404.html")), "erreur_404");
                        return *(hash.value("erreur_404")->page);
                    }

                }else if( d.exists() == true ){
                    // C'EST UN REPERTOIRE !

                    if (hash.contains(rq->get_chemin()))
                    {
                        up(rq->get_chemin());
                        return *(hash.value(rq->get_chemin())->page);
                    }
                    else
                    {
                        add_page(new Repertoire(rq, &d), rq->get_chemin());
                        return *(hash.value(rq->get_chemin())->page);
                    }


                }else if( f.exists() == true ){
                    stat_t->new_fichier(rq->get_chemin());
                    if (hash.contains(rq->get_chemin()))
                    {
                        up(rq->get_chemin());
                        return *(hash.value(rq->get_chemin())->page);
                    }
                    else if (f.size() < total_mem)
                    {
                        add_page(new Fichier(rq, &f), rq->get_chemin());
                        return *(hash.value(rq->get_chemin())->page);
                    }
                    else
                    {
                        return Fichier(rq, &f);
                    }
                }
                break;
		case Requete::cache:
            return affiche();
            break;
        case Requete::info :
            if (hash.contains(rq->get_chemin()))
            {
                up(rq->get_chemin());
                return *(hash.value(rq->get_chemin())->page);
            }
            else
            {
                p = new Text_Page(rq->get_chemin());
                p->start_html("Bienvenue");

                p->line("<b>Bonjour et bienvenue sur ce serveur</b>");
                p->line("Faites comme chez vous");

                p->break_line();
                p->line_nobreak("Le serveur a ete compile le ");
                p->line_nobreak(__DATE__);
                p->line_nobreak(" a ");
                p->line(__TIME__);

                p->break_line();
                p->break_line();
                p->line("Voici quelques liens utiles");

                p->break_line();
                p->line("<a href=\"/private/statistiques.html\">Statistiques du serveur</a>");
                p->line("<a href=\"/private/cache.html\">Etat du cache</a>");
                p->line("<a href=\"/\">Retour vers la racine</a>");
                p->break_line();
                p->break_line();
                p->line("Ecrit par Benjamin Simon et Youness Ilam");
                p->end_html();

                add_page(p, rq->get_chemin());
                return *(hash.value(rq->get_chemin())->page);
            }
            break;
		case Requete::stats:

            return stat_t->affiche();
            break;
        case Requete::activ:
            stat_t->activate();
            return Fichier(rq, &f);
	}
}

void Cache::clean()
{
    qDeleteAll(hash);
    hash.clear();
}

void Cache::refresh_page(Requete* rq)
{
    QFile f(rq->get_chemin());
    QDir d(rq->get_chemin());

    switch (hash.value(rq->get_chemin())->page->get_type()) {
    case Page::fichier:
        delete hash.value(rq->get_chemin());
        add_page(new Fichier(rq, &f), rq->get_chemin());
        break;
    case Page::repertoire:
        delete hash.value(rq->get_chemin());
        add_page(new Repertoire(rq, &d), rq->get_chemin());
        break;
    default:
        delete hash.value(rq->get_chemin());
        add_page(new Text_Page(rq->get_chemin()), rq->get_chemin());
        break;
    }
}

void Cache::up(QString URL)
{
    if (URL == newest)
        return;
    QString temp_prev = hash.value(URL)->prev;
    QString temp_next = hash.value(URL)->next;

    if (oldest == URL)
    {
        hash.value(temp_next)->prev = "Does not exist yet";
        oldest = temp_next;
    }
    else
    {
        hash.value(temp_prev)->next = temp_next;
        hash.value(temp_next)->prev = temp_prev;
    }

    hash.value(newest)->next = URL;
    hash.value(URL)->prev = newest;
    newest = URL;
    hash.value(URL)->next = "Does not exist yet";
}

Text_Page Cache::affiche()
{
    Text_Page p("./public_html/private/cache.html");
    p.start_html("Etat du cache");

    p.line("<b>Etat du cache</b>");
    p.break_line();
    p.line("Nombre de pages stockees en cache",hash.size());
    p.line("Memoire occupee en octet", mem_occupe());
    p.line("Memoire restante en octet", mem_restante());

    p.break_line();
    p.line("<b>Fichiers en cache du plus recent au plus ancien</b>");
    p.line_nobreak("<div style=\"height:400; overflow: auto;\">");
        QString current_page = newest;
        while (current_page != oldest)
        {
            p.line(current_page);
            current_page = previous_page(current_page);
        }
        p.line_nobreak(current_page);
    p.line_nobreak("</div>");
    p.end_html();
    return p;

}

QString Cache::previous_page(QString URL)
{
    return hash.value(URL)->prev;
}
Cache *cache_t;
