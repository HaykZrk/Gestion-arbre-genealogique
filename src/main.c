#include "genealogie.h"

int main(int argc, char* argv[]) {
    printf("Test construction :\n");
    Genealogie g;
    genealogieInit(&g);
    ident ia = adj(&g, "Arthur", 0, 0, (date){ 9, 11, 1879 }, (date){ 12, 11, 1934 });
    ident im = adj(&g, "Marcel", ia, 0, (date){ 1, 7, 1928 }, (date){ 21, 9, 2004 });
    ident ic = adj(&g, "Clothilde", 0, 0, (date){ 30, 8, 1929 }, (date){ 26, 4, 2005 });
    ident ije = adj(&g, "Jeanne", im, ic, (date){ 13,4, 1948 }, (date){ 0,0,0 });
    ident ihe = adj(&g, "Henri", 0,0,(date){ 2,8, 1947 },(date){ 0,0,0 });
    ident ial = adj(&g, "Aline", 0,0,(date){ 7,9, 1943 },(date){ 0,0,0 });
    ident ipi = adj(&g, "Pierre", 0, 0,(date){ 26,6, 1941 },(date){ 0,0,0 });
    ident iju = adj(&g, "Julien", ipi, ial,(date){ 13,8, 1965 },(date){ 0,0,0 });
    ident ialex = adj(&g, "Alex", ipi, ial,(date){ 18,4, 1969 },(date){ 0,0,0 });
    ident iso = adj(&g, "Sophie", ihe, ije,(date){ 9,11, 1972 },(date){ 0,0,0 });
    ident icl = adj(&g, "Clementine", ihe, ije,(date){ 12,10, 1973 },(date){ 0,0,0 });
    ident ima = adj(&g, "Marion", ihe, ije,(date){ 5,5, 1976 },(date){ 0,0,0 });
    ident ich = adj(&g, "Christian", 0, 0,(date){ 13,2, 1971 },(date){ 0,0,0 });
    ident itho = adj(&g, "Thomas", ialex, iso,(date){ 18,10, 2012 },(date){ 0,0,0 });
    ident icloe = adj(&g, "Cloe", ialex, iso,(date){ 21,6, 2002 },(date){ 0,0,0 });
    ident ihu = adj(&g, "Hugo", ialex, iso,(date){ 12,5, 2005 },(date){ 0,0,0 });
    ident isa = adj(&g, "Isabelle", ich, ima,(date){ 28,4, 2003 },(date){ 0,0,0 });
    for (int k = 1; k <= g.id_cur; k++) {
        Individu i = *get(&g,k);
        printf("%d %s %d %d %d %d %d/%d/%d %d/%d/%d\n",k,i.nom,i.pere,i.mere,i.faine,i.cadet,i.naissance.jour,i.naissance.mois,i.naissance.annee,i.deces.jour,i.deces.mois,i.deces.annee);
    }

    printf("\nTest liens de parenté :\n");
    printf("Sophie et Clementine sont-elles soeurs ? (1) : %d\n", freres_soeurs(&g, iso, icl));
    printf("Julien et Christian sont-ils frères ? (0) : %d\n", freres_soeurs(&g, iju, ich));
    printf("Sophie et Marion sont-elles cousines ? (0) : %d\n", cousins(&g, iso, ima));
    printf("Hugo et Isabelle sont-ils cousin/cousine ? (1) : %d\n", cousins(&g, ihu, isa));

    printf("\nTest affichage :\n");
    // Tester les fonctions de la question 3
    printf("Les frères et soeurs de Sophie sont (Clementine, Marion) : \n");
    affiche_freres_soeurs(&g, iso);
    printf("\nLes enfants de Jeanne sont (Sophie, Clementine, Marion) : \n");
    affiche_enfants(&g, ije);
    printf("\nLes cousins/cousines de Isabelle sont : (Cloe, Hugo, Thomas) : \n");
    affiche_cousins(&g, isa);
    printf("\nLes oncles/tantes de Cloe sont (Julien, Clementine, Marion) : \n");
    affiche_oncles(&g, icloe);


    printf("\nTest parcours :\n");
    printf("Arthur est l'ancêtre de Thomas ? (1) : %d\n",ancetre(&g,ia,itho));
    printf("Aline est l'ancêtre d'Isabelle ? (0) : %d\n",ancetre(&g,ial,isa));
    printf("Cloe et Isabelle ont un ancêtre commun ? (1) : %d\n",ancetreCommun(&g,icloe,ic));
    printf("Julien et Isabelle ont un ancêtre commun ? (0) : %d\n", ancetreCommun(&g,iju,ic));
    printf("Ancêtre plus ancien de Cloe (Arthur) : %s\n", get(&g,plus_ancien(&g,icloe))->nom);
    printf("Ancêtre plus ancien de Julien (Pierre) : %s\n", get(&g,plus_ancien(&g,iju))->nom);
    printf("\nLa parenté de Thomas est la suivante : \n");
    affiche_parente(&g,itho);
    printf ("\nLa descendance d'Arthur est la suivante : \n");
    affiche_descendance(&g, ia);

    genealogieFree(&g);

    printf("\nTest fusion :\n");
    Genealogie g1,g2,f;
    genealogieInit(&g1);
    genealogieInit(&g2);
    genealogieInit(&f);

    ia = adj(&g1, "Arthur", 0, 0, (date){ 9, 11, 1879 }, (date){ 12, 11, 1934 });
    im = adj(&g1, "Marcel", ia, 0, (date){ 1, 7, 1928 }, (date){ 21, 9, 2004 });
    ic = adj(&g1, "Clothilde", 0, 0, (date){ 30, 8, 1929 }, (date){ 26, 4, 2005 });
    ije = adj(&g1, "Jeanne", im, ic, (date){ 13,4, 1948 }, (date){ 0,0,0 });
    ihe = adj(&g1, "Henri", 0,0,(date){ 2,8, 1947 },(date){ 0,0,0 });
    iso = adj(&g1, "Sophie", ihe, ije,(date){ 9,11, 1972 },(date){ 0,0,0 });
    adj(&g1, "Clementine", ihe, ije,(date){ 12,10, 1973 },(date){ 0,0,0 });
    ima = adj(&g1, "Marion", ihe, ije,(date){ 5,5, 1976 },(date){ 0,0,0 });
    ich = adj(&g1, "Christian", 0, 0,(date){ 13,2, 1971 },(date){ 0,0,0 });
    itho = adj(&g1, "Thomas", 0, iso,(date){ 18,10, 2012 },(date){ 0,0,0 });
    icloe = adj(&g1, "Cloe", 0, iso,(date){ 21,6, 2002 },(date){ 0,0,0 });
    adj(&g1, "Hugo", 0, iso,(date){ 12,5, 2005 },(date){ 0,0,0 });
    isa = adj(&g1, "Isabelle", ich, ima,(date){ 28,4, 2003 },(date){ 0,0,0 });

    ial = adj(&g2, "Aline", 0,0,(date){ 7,9, 1943 },(date){ 0,0,0 });
    ipi = adj(&g2, "Pierre", 0, 0,(date){ 26,6, 1941 },(date){ 0,0,0 });
    adj(&g2, "Julien", ipi, ial,(date){ 13,8, 1965 },(date){ 0,0,0 });
    ialex = adj(&g2, "Alex", ipi, ial,(date){ 18,4, 1969 },(date){ 0,0,0 });
    adj(&g2, "Cloe", ialex, 0,(date){ 21,6, 2002 },(date){ 0,0,0 });

    genealogieFusion(&f,&g1,&g2);
    for (int k = 1; k <= f.id_cur; k++) {
        Individu i = *get(&f,k);
        printf("%d %s %d %d %d %d %d/%d/%d %d/%d/%d\n",k,i.nom,i.pere,i.mere,i.faine,i.cadet,i.naissance.jour,i.naissance.mois,i.naissance.annee,i.deces.jour,i.deces.mois,i.deces.annee);
    }

    genealogieFree(&g1);
    genealogieFree(&g2);
    genealogieFree(&f);

    return 0;
}