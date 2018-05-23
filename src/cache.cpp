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

void Cache::add_page(Page* page, QString URL)  //Attention j'ai pas codé le fait de revenir au début du tableau
{
	if(mem_restante() < page->get_taille())
	{
        free_mem += hash.value(oldest)->page->get_taille();
        QString temp = oldest;
        oldest = hash.value(oldest)->next;

        delete hash.value(temp)->page;
        delete hash.value(temp);
        hash.remove(temp);
	}
    struct list_page* new_page = new struct list_page;
    new_page->next = "does not exist yet";
    new_page->page = page;
    hash.insert(URL, new_page);
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
Page* Cache::affiche_page(Requete* rq)
{
    QFile f(rq->get_chemin());
    QDir d(rq->get_chemin());

    switch(rq->get_commande())
	{
        case Requete::get	:
		//case Page                 //Place réservé pour les erreurs


                        if( f.exists() == false &&  d.exists() == false ){
                            // ERREUR 404 LE FICHIER N'EXISTE PAS...
                            rq->raise_error(404);
                            if (hash.contains("erreur_404"))
                                return hash.value("erreur_404")->page;
                            else
                            {
                                add_page(new Fichier(rq, new QFile("./public_html/error_404.html")), "erreur_404");
                                return hash.value("erreur_404")->page;
                            }

                        }else if( d.exists() == true ){
                            // C'EST UN REPERTOIRE !

                            if (hash.contains(rq->get_chemin()))
                                return hash.value(rq->get_chemin())->page;
                            else
                            {
                                add_page(new Repertoire(&d), rq->get_chemin());
                                return hash.value(rq->get_chemin())->page;
                            }


                        }else if( f.exists() == true ){

                            if (hash.contains(rq->get_chemin()))
                                return hash.value(rq->get_chemin())->page;
                            else
                            {
                                add_page(new Fichier(rq, &f), rq->get_chemin());
                                return hash.value(rq->get_chemin())->page;
                            }
                        }
						break;
		case Requete::cache:
        case Requete::info :
		case Requete::stats:
		
		
		
						return new Text_Page("http://text.org");
						break;
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
        add_page(new Repertoire(&d), rq->get_chemin());
        break;
    default:
        delete hash.value(rq->get_chemin());
        add_page(new Text_Page(rq->get_chemin()), rq->get_chemin());
        break;
    }
}
/*
void Cache::print_name_page(int id)
{
	cout<<list_page_t.pages[id]->get_name()<<"\n";
}

void Cache::print_id_page(int id)
{
	cout<<list_page_t.pages[id]->get_id()<<"\n";
}

Page* Cache::get_page(int id)
{
	return list_page_t.pages[id];
}
*/
Cache *cache_t;
