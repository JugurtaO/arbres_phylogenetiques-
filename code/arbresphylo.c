#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "arbres.h"
#include "arbresphylo.h"
#include "listes.h"

void analyse_arbre (arbre racine, int* nb_esp, int* nb_carac)
{
   if (racine==NULL){
         *nb_esp=0;
         *nb_carac=0;
   }
   else if (racine->gauche==NULL && racine->droit==NULL){
      *nb_esp=1;
      *nb_carac=0;
   }
   else{
      int esp1,esp2,carac1,carac2;
      analyse_arbre(racine->gauche,&esp1,&carac1);
      analyse_arbre(racine->droit,&esp2,&carac2);
      *nb_esp=esp1+esp2;
      *nb_carac=carac1+carac2+1;
   }

}






/* ACTE II */
/* Recherche l'espece dans l'arbre. Modifie la liste passée en paramètre pour y mettre les
 * caractéristiques. Retourne 0 si l'espèce a été retrouvée, 1 sinon.
 */
int rechercher_espece (arbre racine, char *espece, liste_t* seq)
{
   if (racine==NULL)
      return 1;
   else if (racine->droit==NULL && racine->gauche==NULL){
      int r = strcmp(espece,racine->valeur); // cas validé
      return r;
   }
   else{
      // if(racine->droit==NULL){
      //    int i=rechercher_espece(racine->gauche,espece,seq);
      //    return i;
      // }else if (racine->gauche==NULL){
      //    int i=rechercher_espece(racine->droit,espece,seq);
      //    if (i==0){
      //       ajouter_tete(seq,racine->valeur);
      //       return 0;
      //    }
      //    else 
      //       return 1;
      // }else{
         int i=rechercher_espece(racine->gauche,espece,seq);
         if (i==0)
            return 0;
         else{
            int i=rechercher_espece(racine->droit,espece,seq);
            if (i!=0)
               return 1;
            else{
               ajouter_tete(seq,racine->valeur);
               return 0;
            }

         }
      }
   }




/* Doit renvoyer 0 si l'espece a bien ete ajoutee, 1 sinon, et ecrire un 
 * message d'erreur.
 */
void creer_arbre(arbre* a, char* esp, cellule_t* seq){
  
   if (seq!=NULL)
   {
    noeud *save=(*a);
   noeud *cel=malloc(sizeof(noeud));
    cel->valeur=seq->val;
   cel->gauche=save;
   (*a)=cel;
   cellule_t *current = seq->suivant;
   noeud * prec_noeud= cel;
   while(current!=NULL){
      noeud * cel2=malloc(sizeof(noeud));
      cel2->valeur=current->val;
      cel2->gauche=NULL;
      cel2->droit=NULL;
      prec_noeud->droit=cel2;
      current=current->suivant;
      prec_noeud=cel2;
   }
   noeud * cel2=malloc(sizeof(noeud));
   cel2->valeur=esp;
   cel2->gauche=NULL;
   cel2->droit=NULL;
   prec_noeud->droit=cel2;
   }
   else{
   noeud * cel2=malloc(sizeof(noeud));
   cel2->valeur=esp;
   cel2->gauche=NULL;
   cel2->droit=NULL;
   (*a) = cel2;
   }
}
int ajouter_espece (arbre* a, char *espece, cellule_t* seq) {
   if ((*a)==NULL){
      creer_arbre(a,espece,seq);
      return 0;
   }
   else if ((*a)->gauche == NULL && (*a)->droit == NULL)
       {if (seq!=NULL){
          creer_arbre(a, espece, seq);
          return 0;}
       else
          return 1;
       }
   else{
      if (seq != NULL) {

         if (strcmp((*a)->valeur, seq->val)==0)
         {
         int x = ajouter_espece(&((*a)->droit), espece, seq->suivant);
         return x;
         }
         else
         {
         int x = ajouter_espece(&((*a)->gauche), espece, seq);
         return x;
         }
      }
   else{
       int x = ajouter_espece(&((*a)->gauche), espece, seq);
       return x;
     }
   }
}

/* Doit afficher la liste des caractéristiques niveau par niveau, de gauche
 * à droite, dans le fichier fout.
 * Appeler la fonction avec fout=stdin pour afficher sur la sortie standard.
 */
struct cellule2 {
	string val;
	struct cellule2* suivant;
   int niv;
   arbre a;
};

typedef struct cellule2 cellule_f;
struct file{
   cellule_f *tete;
   cellule_f *queue;
};
typedef struct file file_t;

void init_file(file_t* f){
   f->queue = NULL;
   f->tete = NULL;
}
void enfiler(file_t*f,char*carac,int n,arbre a){
   assert(f != NULL);
   cellule_f *cel = malloc(sizeof(cellule_f));
   cel->val = carac;
   cel->suivant = NULL;
   cel->niv = n;
   cel->a = a;
   if (f->queue != NULL)
   {
     f->queue->suivant = cel;
     f->queue = cel;
   }
   else{
      f->tete = cel;
      f->queue = cel;
   }
}
char* defiler(file_t* f,int* n,arbre* a){
   char *res = f->tete->val;
   *n = f->tete->niv;
   *a = f->tete->a;
   cellule_f *save = f->tete;
   f->tete = save->suivant;
   if (save->suivant==NULL)
      f->queue = NULL;
   free(save);
   return res;
}

int est_vide(file_t* f){
   return f->tete == NULL;
}
void afficher_par_niveau (arbre racine, FILE* fout) {
   file_t f;
   init_file(&f);
   arbre a = racine;
   assert(racine != NULL);
   assert(a->gauche != NULL && a->droit != NULL);
   int n = 0;
   int precn = 0;
   enfiler(&f, racine->valeur, n, a);
   while (!est_vide(&f))
   {
      char *carac = defiler(&f, &n,&a);
      if (precn!=n)
       printf("\n");
      printf("%s ", carac);
      precn = n;
      if (a->gauche != NULL && (a->gauche->gauche != NULL || a->gauche->droit != NULL))
       enfiler(&f, a->gauche->valeur, n + 1, a->gauche);
      if  (a->droit!=NULL && (a->droit->gauche!=NULL ||a->droit->droit!=NULL))
         enfiler(&f, a->droit->valeur, n + 1,a->droit);
   }

  
}


// Acte 4

int ajouter_carac(arbre* a, char* carac, cellule_t* seq) {
   if ((*a)==NULL){
      return 1;
   }
   if ((*a)->droit == NULL && (*a)->gauche == NULL)
   {
      if (strcmp(seq->val,(*a)->valeur) && seq->suivant==NULL){
         noeud *noeud = malloc(sizeof(noeud));
         noeud->valeur = carac;
         noeud->droit = (*a);
         noeud->gauche = NULL;
         (*a) = noeud;
         return 0;
      }
      else
         return 1;
   } else{
      int esp;
      int car;
      analyse_arbre((*a), &esp, &car);
      car++;
      if (esp == taille_seq(seq))
      {
         int drap = 0;
         cellule_t *current = seq;
         while(current!=NULL){
            liste_t *s= malloc(sizeof(liste_t));
            s->tete = NULL;
            if (!rechercher_espece((*a), seq->val, s))
            {
               drap = 1;
            }
            current = current->suivant;
         }
         if (drap){
            noeud *noeud = malloc(sizeof(noeud));
            noeud->valeur = carac;
            noeud->droit = (*a);
            noeud->gauche = NULL;
            (*a) = noeud;
            return 0;
         }
         else
         {
            return 1;
         }
      }
      else
      {

         int x1=ajouter_carac(&(*a)->gauche, carac, seq);
         if (x1==0)
            return 0;
         int x2 = ajouter_carac(&(*a)->droit, carac, seq);
         return x2;
      }
   }
     
   return 0;
}
