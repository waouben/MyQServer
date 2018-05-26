#include <string>
#include <iostream>
#include <QDir>
#include <QTextStream>
#include <QElapsedTimer>
#include <QInputDialog>
#include <QtGui>
#include "classes.hpp"
using namespace std;

QElapsedTimer timer;
int j = 0;
int millisecondes = 0;
char buf[1024];
 QFile file("password");
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
Page Cache::affiche_page(Requete* rq)
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
                                return *(hash.value("erreur_404")->page);
                            else
                            {
                                add_page(new Fichier(rq, new QFile("./public_html/error_404.html")), "erreur_404");
                                return *(hash.value("erreur_404")->page);
                            }

                        }else if( d.exists() == true ){
                            // C'EST UN REPERTOIRE !
                            QString tempo = rq->get_chemin();
                            if (tempo.contains("private") ){
                                bool ok;
                                QFile file("password.txt");
                                QString buf;
                            //    char buf[1024];
                                if (file.open(QFile::ReadOnly)) {
                                    QTextStream flux(&file);
                                    while(!flux.atEnd())
                                          buf += flux.readLine();
                                    //qint64 lineLength = file.readLine(buf, sizeof(buf));

                                }
                                else
                                {
                                      QMessageBox::critical(0,"Erreur","Le fichier "," ne peut être ouvert.");
                                }


                                if (j == 0)
                            {
                                QString text;
                                //cout << ok << endl;
                                while ( (text != buf))
                                {
                                text = QInputDialog::getText(0, ("password"), ("Enter Password:"), QLineEdit::Password);
                                if (text == NULL)
                                {
                                    cout << "ok3" << endl;
                                    if (hash.contains("./public_html"))
                                    {
                                        cout << "ok2" << endl;
                                         return *(hash.value("./public_html")->page);

                                    }
                                    else
                                    {
                                        cout << "ok1" << endl;
                                        add_page(new Repertoire(rq, &d), "./public_html");
                                         return *(hash.value("./public_html")->page);

                                    }
                                    cout << "ok4" << endl;

                                }
                                }
                                    //cout << ok << endl;
                                    timer.start();
                                    if (hash.contains(rq->get_chemin()))
                                {
                                     j++;
                                    return *(hash.value(rq->get_chemin())->page);
                                }
                                    else
                                {
                                     j++;
                                    add_page(new Repertoire(rq, &d), rq->get_chemin());
                                    return *(hash.value(rq->get_chemin())->page);
                                }
                                }


                                else
                                {
                                    if (timer.elapsed() < 10000)
                                   {
                                        if (hash.contains(rq->get_chemin()))
                                        {
                                            return *(hash.value(rq->get_chemin())->page);
                                        }
                                        else
                                        {
                                            add_page(new Repertoire(rq, &d), rq->get_chemin());
                                            return *(hash.value(rq->get_chemin())->page);
                                        }
                                    }
                                   else
                                  {
                                        j = j-j;
                                if (hash.contains(rq->get_chemin()))
                                {
                                    return *(hash.value(rq->get_chemin())->page);
                                }
                                else
                                {
                                    add_page(new Repertoire(rq, &d), rq->get_chemin());
                                    return *(hash.value(rq->get_chemin())->page);
                                }

                                  }

                                }

                            }

                            else if (hash.contains(rq->get_chemin()))
                            {
                                return *(hash.value(rq->get_chemin())->page);
                            }
                            else
                            {
                                add_page(new Repertoire(rq, &d), rq->get_chemin());
                                return *(hash.value(rq->get_chemin())->page);
                            }


                        }else if( f.exists() == true ){

                            if (hash.contains(rq->get_chemin()))
                                return *(hash.value(rq->get_chemin())->page);
                            else
                            {
                                add_page(new Fichier(rq, &f), rq->get_chemin());
                                return *(hash.value(rq->get_chemin())->page);
                            }
                        }
						break;
		case Requete::cache:
            return affiche();
            break;
        case Requete::info :

            break;
		case Requete::stats:

            return stat_t->affiche();
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
        add_page(new Repertoire(rq, &d), rq->get_chemin());
        break;
    default:
        delete hash.value(rq->get_chemin());
        add_page(new Text_Page(rq->get_chemin()), rq->get_chemin());
        break;
    }
}

Text_Page Cache::affiche()
{

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
