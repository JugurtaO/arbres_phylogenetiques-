#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "listes.h"

/* fichier à compléter au besoin */

void init_liste_vide(liste_t* L) {
   L->tete=NULL;
    /* a completer */
}

void liberer_liste(liste_t* L) {
   if (L->tete!=NULL){
        cellule_t * prec=L->tete;
        cellule_t * cel=prec->suivant;
        while(prec!=NULL){
            free(prec);
            prec=cel;
            if (cel!=NULL)
                cel=cel->suivant;
        }

   }
    /* a completer */
}


int ajouter_tete(liste_t* L, string c) { /* retourne 0 si OK, 1 sinon  */
   if (L!=NULL){
        cellule_t*nc=malloc(sizeof(cellule_t));
        nc->val=c;
        nc->suivant=L->tete;
        L->tete=nc;
        return 0;
   }else
        return 1;
}



int taille_seq(cellule_t *l){
//    assert(l != NULL);
   int x = 0;
   cellule_t *current = l;
   while(current!=NULL){
     x++;
     current = current->suivant;
   }
   return x;
}
