#include<stdlib.h>
#include <stdio.h>
# include "../headers/Struct_Liste.h"

void Init_Liste(ListeEntier *L){
  *L=NULL;
}


int estLileVide(ListeEntier *L){
  return (*L==NULL);
}

void ajoute_en_tete(ListeEntier* L, int u){
  Cell_entier *nouvsom=(Cell_entier*) malloc(sizeof(Cell_entier));
  nouvsom->u=u;
  nouvsom->suiv=*L;
  *L=nouvsom;
}

ListeEntier ajoute_en_tete2(ListeEntier L, int u){
  Cell_entier *nouvsom=(Cell_entier*) malloc(sizeof(Cell_entier));
  nouvsom->u=u;
  nouvsom->suiv=L;
  return nouvsom;
}


void desalloue(ListeEntier *L){
  Cell_entier *cour,*prec;
  cour=*L;
  while(cour!=NULL){
    prec=cour;
    cour=cour->suiv;
    free(prec);
  }
  *L=NULL;
}

void desalloue_tab_liste(ListeEntier* tab_L, int taille){
  for( int i = 0; i< taille ; i++){
    if (tab_L[i] )  free(tab_L[i]);
  }
  free(tab_L);
}


void  desalloue_tab_liste2(ListeEntier* tab_L, ListeEntier element_a_effacer , ListeEntier element_a_eviter){
  /* desalloue  les indice  'element_a_effacer' d'un tableau de liste 'tab_L'  en evitant les indice de 'element_a_eviter'
  les indices dans 'element_a_eviter' doivent etre dans le meme ordre que le leur ordre d'apparition dans 'element_a_effacer'
  libere aussi 'element_a_effacer' 
  libere aussi 'tab_L'*/
    ListeEntier premier_element = element_a_effacer; // sera utilisé a la fin pour la liberation
    int num;
    while (element_a_effacer){
      num = element_a_effacer->u;
      if (num == element_a_eviter->u){
        element_a_eviter = element_a_eviter->suiv;
        element_a_effacer = element_a_effacer->suiv;
        continue;
      }
      free(tab_L[num]);
      element_a_effacer = element_a_effacer->suiv;
    }
    desalloue(&premier_element);
    free(tab_L);
}


void afficheListe(ListeEntier L){
    if(!L) printf("(liste vide)\n");
    else {
      printf("%d",L->u);
      L = L->suiv;
    }
    while (L){
      printf(", %d",L->u);
      L = L->suiv;
    }

    printf("\n");
}