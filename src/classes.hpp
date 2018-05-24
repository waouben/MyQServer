#ifndef _CLASSES_
#define _CLASSES_

#include <string>
#include <cstdlib>
#include <ctime>

#include <QString>
#include <QFile>
#include <QDir>
#include <QHash>


class Requete
{
public:
    Requete(std::string cmde, QString chemin);
    Requete(std::string cmde, QString _chemin, bool isFile);
    enum commande{get, stats, cache, info, clear_cache, clear_stats, activ, desactiv};  //Je crois qu'il en manque, nottament pour les erreurs
    enum commande get_commande();
    QString get_chemin();
    long long get_heure();
    int get_error();
    const char *http_reponse();
    void raise_error(int);
private:
    enum commande commande_t;
    int error;
    QString chemin;
    time_t heure;
};



class Page
{
public:
	enum type{fichier, repertoire, texte};
    Page();
    Page(QString URL);
    Page(Requete* rq);
	~Page();
    QByteArray get_bytes();
    QString get_name();
	int get_id();
	time_t get_heure();
	int get_taille();
    enum type get_type();
protected:
	enum type type_t;
    QByteArray bytes;
    int taille;
private:
    QString nom;
    QString URL;
	int id;
	static int next_id;
	time_t heure;
};

class Fichier : public Page
{
public:
    Fichier(QString);
    Fichier(Requete* rq, QFile* file);
};

class Repertoire : public Page
{
public:
    Repertoire(std::string);
    Repertoire(Requete *rq, QDir* dir);
};

class Text_Page : public Page
{
public:
    Text_Page(QString URL);
    void start_html(QString titre);
    void end_html();
    void line(QString);
    void line(QString, int);
    void line(QString, QString);
    void break_line();
};





class Cache
{
public:
	Cache();
	~Cache();
	void clean();
    Text_Page affiche();
	int mem_restante();
    int mem_occupe();
    Page affiche_page(Requete *);
    void add_page(Page*, QString URL);
    void refresh_page(Requete*);
    void up(QString);
private:
    static const int total_mem = 15000000;   //A voir
	int free_mem;
    QString oldest;
    QString newest;
    struct list_page{
        Page* page;
        QString next;
        QString prev;
        ~list_page(){delete page;}
    };
    QHash<QString, list_page*> hash;
};

class Stat
{
public:
    Stat();
    ~Stat();

    int get_rq_recu();
    int get_rq_traite();
    int get_error(int error);
    int get_client();
    int get_byte_recu();
    int get_byte_envoi();

    void new_rq_recu();
    void new_error(int error);
    void new_client();          //a faire
    void new_byte_recu(int);    //a faire
    void new_byte_envoi(int);
    void new_rq(Requete *);
    void new_fichier(QString);

    Page affiche();
    void clean();
private:
    void new_rq_traite();
    int rq_recu;
    int rq_traite;
    int nb_client;
    int byte_recu;
    int byte_envoi;

    QHash<int, int> list_error;
    QHash<QString, int> list_fichier;
    QVector<Requete*> *list_rq;

};

int etat_serveur();

extern Cache *cache_t;
extern Stat *stat_t;
#endif
