#include <stdlib.h>

typedef unsigned int ident;
typedef int bool;

typedef struct s_file {
    ident v;
    struct s_file *s;
} *File;

File filenouv();
File adjq(File f, ident x);
File supt(File f);
bool vide(File f);
File tete(File f);
bool appartient(File f, ident x);
void detruire(File f);
unsigned int lgr(File f);