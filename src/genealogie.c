/**
 * @file genealogie.c
 * @author ZARIKIAN Hayk et DUBERT Alexandre
 * @brief 
 * @version 0.1
 * @date 2022-04-13
 * 
 * 
 */

#include "genealogie.h"

/*###########################################################################*/
/*                 1. Construction de l’arbre généalogique                   */
/*###########################################################################*/

/**
 * @author Hayk Zarikian
 * @brief Initialise l'arbre genealogique.
 * 
 * @param[in] g 
 */
void genealogieInit(Genealogie *g) {
    g->taille_max_tab = TAILLE_INIT;
    g->id_cur = OMEGA;
    g->tab = malloc(sizeof(Individu) * g->taille_max_tab);
    if (g->tab == NULL) exit (1);
}

/**
 * @author Hayk Zarikian
 * @brief Libère la mémoire allouée.
 * 
 * @param[in] g 
 */
void genealogieFree(Genealogie *g) {
    free (g->tab);
    g->id_cur = OMEGA;
    g->taille_max_tab = OMEGA;
}

/**
 * @author Alexandre Dubert
 * @brief Ajoute un individu à la généalogie
 *
 * @param g pointeur sur la généalogie
 * @param s le nom de l'individu
 * @param p l'identifiant du père dans la généalogie
 * @param m l'identifiant de la mère dans la généalogie
 * @param n la date de naissance
 * @param d la date de décès
 *
 * @pre g != NULL
 * @return l'identifiant de ce nouvel individu
 * @post les champs faine des parents et les champs cadet des frères de 
 * l'individu sont mis à jour
*/
ident adj(Genealogie *g, char *s, ident p, ident m, date n, date d) {
    // Réallocation mémoire si nécessaire
    if (g->id_cur == g->taille_max_tab) {
        g->tab = realloc(g->tab, g->taille_max_tab * TAILLE_INIT);
        if (g->tab == NULL) exit(1);
    }

    // Création du nouvel individu
    Individu nv;
    strcpy(nv.nom, s);
    nv.pere = p;
    nv.mere = m;
    nv.naissance = n;
    nv.deces = d;
    nv.cadet = OMEGA;
    nv.faine = OMEGA;
    g->tab[g->id_cur] = nv;
    g->id_cur++;

    // Actualisation des champs faine et cadet
    ident id_parent = p != OMEGA ? p : m;
    if (id_parent != OMEGA) {
        ident fils_suiv = g->tab[id_parent-1].faine;
        if (fils_suiv == OMEGA) {
            if (p) g->tab[p-1].faine = g->id_cur;
            if (m) g->tab[m-1].faine = g->id_cur;
        } else {
            ident lst_enfant[15]; // On limite le nombre d'enfants à 15
            int i = 0;
            while (i != 14 && fils_suiv != OMEGA) {
                lst_enfant[i] = fils_suiv;
                fils_suiv = g->tab[fils_suiv-1].cadet;
                i++;
            }
            lst_enfant[i] = g->id_cur;
            tri_par_date(g,lst_enfant, i+1);
            for (int j = 0; j < i; j++) {
                g->tab[lst_enfant[j]-1].cadet = lst_enfant[j+1];
            }
            g->tab[lst_enfant[i]-1].cadet = OMEGA;
            if (p) g->tab[p-1].faine = lst_enfant[0];
            if (m) g->tab[m-1].faine = lst_enfant[0];
        }
    }
    return g->id_cur;
}

// Usage interne
void permut(ident* c1, ident* c2) {
	ident tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

/**
 * @author Alexandre Dubert
 * @brief Tri une liste d'individu par ordre croissant des dates de naissance
 *
 * @param g pointeur sur la généalogie
 * @param lst liste d'idientifiants des individus
 * @param taille longueur de lst
 * @post lst est trié
 */
void tri_par_date(Genealogie* g, ident lst[], int taille) {
	bool est_trie = 0; 
    for (int i = 0; i < taille && !est_trie; i++) {
		est_trie = 1;
		for (int j = 0; j < taille - 1 - i; j++) {
			if (compDate(g->tab[lst[j]-1].naissance,
                g->tab[lst[j+1]-1].naissance) == 1) {
				permut(&lst[j], &lst[j+1]);
				est_trie = 0;
			}
		}
    }
}

/**
 * @author Alexandre Dubert
 * @brief vérifie si la première date est postérieure à la deuxième
 *
 * @param date1
 * @param date2
 * @return 1 si date1 > date2, 0 si elles sont égales et -1 si date1 < date2
 */
int compDate(date date1, date date2) {
    if (date1.annee > date2.annee) return 1;
    else if (date1.annee < date2.annee) return -1;
    else {
        if (date1.mois > date2.mois) return 1;
        else if (date1.mois < date2.mois) return -1;
        else {
            if (date1.jour > date2.jour) return 1;
            else if (date1.jour < date2.jour) return -1;
        }
    }
    return 0; // Même date de naissance
}

/**
 * @author Hayk Zarikian
 * @brief Accès à un individu selon l'identifiant.
 * 
 * @param[in] g 
 * @param[in] x 
 * @return Individu* 
 */
Individu *get(Genealogie *g, ident x) {
    return &g->tab[x - 1];
}

/*###########################################################################*/
/*                            2. Liens de parenté                            */
/*###########################################################################*/

/**
 * @author Hayk Zarikian
 * @brief Vérifie si x et y sont frères ou soeurs.
 * 
 * @param[in] g 
 * @param[in] x 
 * @param[in] y 
 * @return true 
 * @return false 
 */
bool freres_soeurs(Genealogie *g, ident x, ident y) {
    if (x == OMEGA || y == OMEGA)
        return 0;

    ident pere_x = get(g, x)->pere;
    ident pere_y = get(g, y)->pere;
    ident mere_x = get(g, x)->mere;
    ident mere_y = get(g, y)->mere;

    // Teste dans le cas où l'id du père x et du père y est égal à 0 
    // sans que les pères soient identique (resp. mere x et mere y).
    // Un individu sans parent, ni mère ni père n'est pas fils donc pas de 
    // frère et soeur
    if (pere_x == 0 && pere_y == 0) {
        if (mere_x == 0 && mere_y == 0)
            return 0;
        else if (mere_x == mere_y)
            return 1;
    }
    else if (mere_x == 0 && mere_y == 0) {
        if (pere_x == 0 && pere_y == 0)
            return 0;
        if (pere_x == pere_y)
            return 1;
    }
    else {
        if (pere_x == pere_y || mere_x == mere_y)
            return 1;
    }

    return 0;
}

/**
 * @author Hayk Zarikian
 * @brief Vérifie si x et y sont cousins.
 * 
 * @param[in] g 
 * @param[in] x 
 * @param[in] y 
 * @return true 
 * @return false 
 */
bool cousins(Genealogie *g, ident x, ident y) {
    if (x == OMEGA || y == OMEGA)
        return 0;
    
    ident pere_x = get(g, x)->pere;
    ident pere_y = get(g, y)->pere;
    ident mere_x = get(g, x)->mere;
    ident mere_y = get(g, y)->mere;
    
    // Si x et y ont le même père ou mère alors ils sont frères
    // => pas cousins
    if (pere_x == pere_y || mere_x == mere_y)
        return 0;

    if (freres_soeurs(g, pere_x, pere_y) ||
        freres_soeurs(g, pere_x, mere_y) ||
        freres_soeurs(g, mere_x, pere_y) ||
        freres_soeurs(g, mere_x, mere_y)) {
            return 1;
        }

    return 0;
}

/*###########################################################################*/
/*                               3. Affichage                                */
/*###########################################################################*/

/**
 * @author Hayk Zarikian
 * @brief Affiche les frères et soeurs de x.
 * 
 * @param[in] g 
 * @param[in] x 
 */
void affiche_freres_soeurs(Genealogie *g, ident x) {
    if (x == OMEGA) 
        return;

    ident parent_x;
    ident faine_a_cadet;
    if ((parent_x = get(g, x)->pere) != 0)
        faine_a_cadet = get(g, parent_x)->faine;
    else if ((parent_x = get(g, x)->mere) != 0)
        faine_a_cadet = get(g, parent_x)->faine;
    else 
        return;
  
    while (faine_a_cadet != 0) {
        if (faine_a_cadet != x)
            printf ("%s\n", get(g, faine_a_cadet)->nom);
        faine_a_cadet = get(g, faine_a_cadet)->cadet;
    }
}

/**
 * @author Hayk Zarikian
 * @brief Affiche les enfants de x.
 * 
 * @param[in] g 
 * @param[in] x 
 */
void affiche_enfants(Genealogie *g, ident x) {
    if (x == OMEGA)
        return;

    ident faine_a_cadet = get(g, x)->faine;
    while (faine_a_cadet != 0)
    {
        printf ("%s\n", get(g, faine_a_cadet)->nom);
        faine_a_cadet = get(g, faine_a_cadet)->cadet;
    }
}

/**
 * @author Hayk Zarikian
 * @brief Affiche les enfants des frères et soeurs de x.
 * 
 * @param[in] g 
 * @param[in] x 
 */
void affiche_enfants_freres_soeurs(Genealogie *g, ident x) {
    if (x == OMEGA)
        return;

    ident pere_x = get(g, x)->pere;
    ident mere_x = get(g, x)->mere;
    if (pere_x != 0) {
        ident faine_a_cadet = get(g, pere_x)->faine;
        while (faine_a_cadet != 0) {
            if (faine_a_cadet != x)
                affiche_enfants(g, faine_a_cadet);
            faine_a_cadet = get(g, faine_a_cadet)->cadet;
        }
    }
    else if (mere_x != 0) {
        ident faine_a_cadet = get(g, mere_x)->faine;
        while (faine_a_cadet != 0) {
            if (faine_a_cadet != x)
                affiche_enfants(g, faine_a_cadet);
            faine_a_cadet = get(g, faine_a_cadet)->cadet;
        }
    }
}

    
/**
 * @author Hayk Zarikian
 * @brief Affiche les cousins de x.
 * 
 * @param[in] g 
 * @param[in] x 
 */
void affiche_cousins(Genealogie *g, ident x) {
    if (x == OMEGA)
        return;

    ident pere_x = get(g, x)->pere;
    ident mere_x = get(g, x)->mere;

    affiche_enfants_freres_soeurs (g, pere_x);
    affiche_enfants_freres_soeurs (g, mere_x);

}

/**
 * @author Hayk Zarikian
 * @brief Affiche les oncles et tantes de x.
 * 
 * @param[in] g 
 * @param[in] x 
 */
void affiche_oncles(Genealogie *g, ident x) {
    if (x == OMEGA)
        return;

    ident pere_x = get(g, x)->pere;
    ident mere_x = get(g, x)->mere;

    affiche_freres_soeurs(g, pere_x);
    affiche_freres_soeurs(g, mere_x);
}

/*###########################################################################*/
/*                    4. Parcours de l’arbre généalogique                    */
/*###########################################################################*/

/**
 * @author Alexandre Dubert
 * @brief vérifie si x est ancêtre de y
 *
 * @param g pointeur sur la généalogie
 * @param x id de x
 * @param y id de y
 * @return 1 si x est l'ancêtre de y sinon 0
 */
bool ancetre(Genealogie *g, ident x, ident y) {
    if (x == y) return 1;
    if (y == OMEGA) return 0;
    return ancetre(g,x,get(g,y)->pere) || ancetre(g,x,get(g,y)->mere);
}

/**
 * @author Alexandre Dubert
 * @brief vérifie si x et y ont un ancêtre commun
 *
 * @param g pointeur sur la généalogie
 * @param x id de x
 * @param y id de y
 * @return 1 si x et y partagent un ancêtre 0 sinon
 */
bool ancetreCommun(Genealogie *g, ident x, ident y) {
    if (x == OMEGA) return 0;
    return ancetre(g,x,y) || ancetreCommun(g,get(g,x)->pere,y) 
        || ancetreCommun(g,get(g,x)->mere,y);
}

/**
 * @author Alexandre Dubert
 * @brief plus vieil ancêtre de x.
 * 
 * @param g 
 * @param x 
 * @return ident 
 */
ident plus_ancien(Genealogie *g, ident x) {
    ident papa = get(g,x)->pere;
    ident maman = get(g,x)->mere;

    if (papa == OMEGA && maman == OMEGA) return x;

    if (papa != OMEGA && maman == OMEGA) return plus_ancien(g,papa);
    if (papa == OMEGA && maman != OMEGA) return plus_ancien(g,maman);

    ident tmp1 = plus_ancien(g,papa);
    ident tmp2 = plus_ancien(g,maman);
    if (compDate(get(g,tmp1)->naissance,get(g,tmp2)->naissance) == 1)
        return tmp2;
    else 
        return tmp1;
}

// usage interne
ident getAine(Genealogie *g, ident x) {
    ident papa = get(g,x)->pere;
    ident maman = get(g,x)->mere;

    if (!(papa||maman)) return x;
    ident parent = maman == OMEGA ? papa : maman;
    return get(g,parent)->faine;
}

void affiche_cadet(Genealogie *g, ident x) {
    if (x == OMEGA) return;
    printf("%s\n",get(g,x)->nom);
    affiche_cadet(g,get(g,x)->cadet);
}

/**
 * @author Alexandre Dubert
 * @brief affiche l'ensemble des parents de x
 *
 * @param g pointeur sur la généalogie
 * @param x id de x
 */
void affiche_parente(Genealogie *g, ident x) {
    int generation = 1;
    ident papa = get(g,x)->pere;
    ident maman = get(g,x)->mere;
    File f = filenouv();
    File dejavu = filenouv();

    if (papa != OMEGA) {
        f = adjq(f,getAine(g,papa));
        dejavu = adjq(dejavu,getAine(g,papa));
    }
    if (maman != OMEGA) {
        f = adjq(f,getAine(g,maman));
        dejavu = adjq(dejavu,getAine(g,maman));
    }

    while (!vide(f)) {
        printf("- %d:\n",generation);
        int taille_generation = lgr(f);
        while (taille_generation-- != 0) {
            x = tete(f)->v;
            f = supt(f);
            affiche_cadet(g,x);

            papa = get(g,x)->pere;
            maman = get(g,x)->mere;
            if (papa != OMEGA && !appartient(dejavu,getAine(g,papa))) {
                f = adjq(f,getAine(g,papa));
                dejavu = adjq(dejavu,getAine(g,papa));
            }
            if (maman != OMEGA && !appartient(dejavu,getAine(g,maman))) {
                f = adjq(f,getAine(g,maman));
                dejavu = adjq(dejavu,getAine(g,maman));
            }
        }
        generation++;
    }

    detruire(f);
    detruire(dejavu);
}

/**
 * @author Hayk Zarikian
 * @brief Affiche descendance de x.
 * 
 * @param[in] g 
 * @param[in] x 
 */
void affiche_descendance(Genealogie *g, ident x) {

    static int generation = 1;

    if (x == OMEGA) 
    {
        generation = 1;
        return;
    }

    ident descendance = get(g, x)->faine;

    if (descendance != 0) {
        printf("- %d:\n", generation);
        affiche_enfants(g, x);
        affiche_enfants_freres_soeurs(g, x);
        generation++;
    }

    affiche_descendance(g, descendance);
}

/*###########################################################################*/
/*                     5. Fusion d’arbres généalogiques                      */
/*###########################################################################*/

/**
 * @author Hayk Zarikian
 * @brief Cherche l'individu dans la généalogie
 * 
 * @param g pointeur sur la généalogie
 * @param toto individu
 * @return l'ident de l'individu dans g ou OMEGA s'il n'est pas dans g
 */
ident estDansGenealogie(Genealogie *g, Individu toto) {
    for (int i = 1; i <= g->id_cur; i++) {
        if (! (compDate(get(g,i)->naissance,toto.naissance) ||
            strcmp(get(g,i)->nom,toto.nom)) ) {
            return i;
        }
    }
    return OMEGA;
}

/**
 * @author Hayk Zarikian
 * @brief Cherche x l'ident de x dans l'autre généalogie
 * 
 * @param tabA le tableau des ident d'une généalogie
 * @param tabB la correspondance de ces ident dans une autre généalogie
 * @param x l'ident de l'individu recherché
 * @param taille la taille des tableaux
 * @return l'indice de l'individu dans tabB
 */
int corres (ident tabA[], ident tabB[], ident x, int taille) {
    if (x == OMEGA) return OMEGA;

    for (int i = 0; i < taille; i++) {
        if (tabA[i] == x)
            return tabB[i];
    }
    return OMEGA;
}

/**
 * @author Alexandre Dubert
 * @brief union des parents d'un individu avec ses frères
 *
 * @param g pointeur vers la généalogie
 * @param x l'identifiant de l'individu
 * @param type 1 s'il ont veut mettre à jour le père, 0 pour la mère
 */
void miseAJourParents(Genealogie* g, ident x, int type) {
    ident parent = type ? get(g,x)->mere : get(g,x)->pere;
    ident aine = get(g,parent)->faine;
    while (aine != OMEGA) {
        get(g,aine)->pere = get(g,x)->pere;
        get(g,aine)->mere = get(g,x)->mere;
        aine = get(g,aine)->cadet;
    }
}

/**
 * @author Alexandre Dubert
 * @brief Copie d'une généalogie
 *
 * @pre dst != NULL
 * @param dst la copie
 * @param src l'originale
 * @post src est copié dans dst
 */
void copieGenealogie(Genealogie* dst, Genealogie* src) {
    for (int i = 1; i <= src->id_cur; i++) {
        adj(dst,get(src,i)->nom,get(src,i)->pere,get(src,i)->mere,
            get(src,i)->naissance,get(src,i)->deces);
    }
}

/**
 * @author Alexandre Dubert
 * @brief Fusionne deux généalogie
 *
 * @pre gres != NULL && a1 != NULL && a2 != NULL
 * @param gres la fusion
 * @param a1 une des généalogie à fusionner
 * @param a2 l'autre généalogie
 * @post gres contient la fusion d'a1 et d'a2, a1 et a2 sont non modifiées
 */
void genealogieFusion(Genealogie *gres, Genealogie *a1, Genealogie *a2) {
    if (a1->id_cur > a2->id_cur)
        copieGenealogie(gres,a1);
    else 
        copieGenealogie(gres,a2);
    Genealogie arbreACopier = (a1->id_cur > a2->id_cur) ? *a2 : *a1;

    // Initialisation des tableau d'ident
    int taille = arbreACopier.id_cur;
    ident *id_o, *id_n;
    id_o = malloc(taille*sizeof(ident));
    id_n = malloc(taille*sizeof(ident));
    if (id_o == NULL || id_n == NULL) exit(1);
    int n = 0;

    for (int i = 1; i <= arbreACopier.id_cur; i++) {
        ident cur_individu = estDansGenealogie(gres,*get(&arbreACopier,i));
        id_o[n] = i;
        if (cur_individu == OMEGA) {
            // Elements non compris dans l'intersection
            ident p = corres(id_o,id_n,get(&arbreACopier,i)->pere,taille);
            ident m = corres(id_o,id_n,get(&arbreACopier,i)->mere,taille);
            id_n[n] = adj(gres, get(&arbreACopier,i)->nom, p, m, 
                get(&arbreACopier,i)->naissance, get(&arbreACopier,i)->deces);
        } else {
            // Doublons
            id_n[n] = cur_individu;
            if (get(gres,cur_individu)->pere == OMEGA) {
                get(gres,cur_individu)->pere = 
                    corres(id_o,id_n,get(&arbreACopier,i)->pere,taille);
                miseAJourParents(gres,cur_individu,1);
            }
            if (get(gres,cur_individu)->mere == OMEGA) {
                get(gres,cur_individu)->mere = 
                    corres(id_o,id_n,get(&arbreACopier,i)->mere,taille);
                miseAJourParents(gres,cur_individu,0);
            }
        }
        n++;
    }
}