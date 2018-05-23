#include <string>
#include <iostream>
#include "classes.hpp"

using namespace std;


Cache::Cache()
{
	free_mem = total_mem;
	list_page_t.oldest = 0;
	list_page_t.next = 0;
}

Cache::~Cache()
{
}


int Cache::mem_restante()
{
	return free_mem;
}

void Cache::add_page(Page* page)  //Attention j'ai pas codé le fait de revenir au début du tableau
{
	if(mem_restante() < page->get_taille())
	{
		free_mem += list_page_t.pages[list_page_t.oldest]->get_taille();
		delete list_page_t.pages[list_page_t.oldest];
		list_page_t.oldest++;
	}
	list_page_t.pages[list_page_t.next] = page;
	free_mem -= list_page_t.pages[list_page_t.next]->get_taille();
	list_page_t.next++;
}
/*
Page Cache::affiche()
{
	//Peut etre redondant avec affiche_page()
}
*/
Page* Cache::affiche_page(Requete* rq)
{
	switch(rq->get_commande())
	{
		case Requete::read	:
		//case Page                 //Place réservé pour les erreurs
						return new Fichier("http://fichier.fr");
						break;
		case Requete::ls	:
		
						return new Repertoire("http://repertoire.com");
						break;
		case Requete::cache:
		case Requete::info	:
		case Requete::stats:
		
		
		
						return new Text_Page("http://text.org");
						break;
	}
}


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
