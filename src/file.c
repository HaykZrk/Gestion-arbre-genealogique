/**
 * @file file.c
 * @author Alexandre Dubert
 * @brief 
 * @version 0.1
 * @date 2022-04-13
 * 
 * 
 */

#include "file.h"

File filenouv() { return NULL; }

File adjq(File f, ident x) {
    File nv = malloc(sizeof(struct s_file));
    if (nv == NULL) exit(1);
    nv->v = x;
    nv->s = f;
    return nv;
}

File supt(File f) {
    if (vide(f)) return f;
    if (vide(f->s)) {
        free(f);
        return NULL;
    }
    File f_cp = f;
    while (!vide(f->s->s)) f = f->s;
    free(f->s);
    f->s = NULL;
    return f_cp;
}

bool vide(File f) { return f == NULL; }

File tete(File f) {
    if (vide(f)) return f;
    while(!vide(f->s)) f = f->s;
    return f;
}

bool appartient(File f, ident x) {
    if (vide(f)) return 0;
    if (f->v == x) return 1;
    return appartient(f->s,x);
}

void detruire(File f) {
    while (!vide(f)) {
        File tmp = f->s;
        free(f);
        f = tmp;
    }
}

unsigned int lgr(File f) {
    if (vide(f)) return 0;
    else return 1 + lgr(f->s);
}