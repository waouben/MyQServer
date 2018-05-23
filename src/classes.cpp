#include <string>
#include <iostream>
#include <ctime>
#include "classes.hpp"

using namespace std;

int Page::next_id = 0;

Page::Page(string URL_arg) //Premier draft
{
	nom = URL_arg;
	id = next_id;
	next_id++;
	URL = URL_arg;
	bytes = NULL;
	heure = time(NULL);
}

Page::~Page()
{
}

Fichier::Fichier(string URL_arg)
{
	type_t = fichier;
}

string Page::get_name()
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

time_t Page::get_heure()
{
	return heure;
}




