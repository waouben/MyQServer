#ifndef _CLASSES_
#define _CLASSES_

#include <string>
#include <cstdlib>
#include <ctime>



class Page
{
public:
	enum type{fichier, repertoire, texte};
	Page(std::string URL);
	~Page();
	void* get_bytes();
	std::string get_name();
	int get_id();
	time_t get_heure();
	int get_taille();
	enum type get_type();
protected:
	enum type type_t;
private:
	std::string nom;
	int id;
	static int next_id;
	int taille;
	std::string URL;
	void* bytes;
	time_t heure;
};

class Fichier : public Page
{
	Fichier(std::string);
};

class Repertoire : public Page
{
	
};

class Text_Page : public Page
{
	
};


class Requete_old   //A supprimer
{
public:
	Requete_old();
	Page *recup_page();
	void* donnee_envoi();
	int get_heure();
private:
	std::string commande;
	long long heure;
};

class Requete
{
public:
	Requete(std::string commande);
	enum commande{read, /* exec,*/ ls, stats, cache, info, clear_cache, clear_stats};  //Je crois qu'il en manque, nottament pour les erreurs
	enum commande get_commande();
	std::string get_argument();
	long long get_heure();
	// A compléter
private:
	enum commande commande_t;
	std::string argument;
	time_t heure;
};

class Cache
{
public:
	Cache();
	~Cache();
	void clean();
	Page affiche();
	void print_name_page(int id);   //temporaire
	void print_id_page(int id); 	//temporaire
	int mem_restante();
	Page* get_page(int id);
	Page* affiche_page(Requete*);
	void add_page(Page*);
	void refresh_page(int id);
private:
	static const int total_mem = 1000000000;   //A voir
	static const int taille_cache = 2048;
	int free_mem;
	struct list_page{
		Page* pages[taille_cache]; // C'est peut être mieux d'allouer dynamiquement?
		int oldest;
		int next;
	}list_page_t;
};

int etat_serveur();


#endif
