#include <stdio.h>
#include <string.h>
#include "file.h"

#define LG_MAX 64
#define OMEGA 0
#define TAILLE_INIT 50

typedef struct s_date { unsigned short jour, mois, annee; } date; 
typedef unsigned int Nat;

typedef struct s_individu {
    char nom[LG_MAX];
    date naissance, deces;
    ident pere, mere, cadet, faine; 
} Individu;

typedef struct s_genealogie {
    Individu *tab; // tableau alloué dynamiquement
    Nat id_cur; // identifiant actuel
    Nat taille_max_tab; // taille max du tableau
} Genealogie; 

void genealogieInit(Genealogie *g);
void genealogieFree(Genealogie *g);
ident adj(Genealogie *g, char *s, ident p, ident m, date n, date d);
void tri_par_date(Genealogie* g, ident lst[], int taille);
int compDate(date date1, date date2);
Individu *get(Genealogie *g, ident x);

bool freres_soeurs(Genealogie *g, ident x, ident y);
bool cousins(Genealogie *g, ident x, ident y);

void affiche_freres_soeurs(Genealogie *g, ident x);
void affiche_enfants(Genealogie *g, ident x);
void affiche_enfants_freres_soeurs(Genealogie *g, ident x);
void affiche_cousins(Genealogie *g, ident x);
void affiche_oncles(Genealogie *g, ident x);

bool ancetre(Genealogie *g, ident x, ident y);
bool ancetreCommun(Genealogie *g, ident x, ident y);
ident plus_ancien(Genealogie *g, ident x);
void affiche_parente(Genealogie *g, ident x);
void affiche_descendance(Genealogie *g, ident x);

void genealogieFusion(Genealogie *gres, Genealogie *a1, Genealogie *a2);
