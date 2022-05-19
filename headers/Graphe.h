#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include  <stdlib.h>
#include  <stdio.h>
# include "../headers/Struct_Liste.h"
# include "../headers/Struct_File.h"
# include "../headers/Reseau.h"
# include "../headers/Hachage.h"
# include "../headers/OutilT.h"
# include "../headers/ArbreQuat.h"

typedef struct{
  int u,v;        /* Numeros des sommets extremité */
} Arete;


typedef struct cellule_arete{
  Arete *a;     /* pointeur sur l'arete */
  struct cellule_arete *suiv;
} Cellule_arete;


typedef struct {
  int num;                 /* Numero du sommet (le meme que dans T_som) */
  double x, y;
  Cellule_arete* L_voisin;  /* Liste chainee des voisins */
} Sommet;


typedef struct{
  int e1,e2;    /* Les deux extremites de la commodite */
} Commod;


typedef struct{
  int nbsom;        /* Nombre de sommets */
  Sommet* *T_som;    /* Tableau de pointeurs sur sommets */
  int gamma;     
  int nbcommod;     /* Nombre de commodites */
  Commod *T_commod; /* Tableau des commodites */
} Graphe;



/* Cree un graphe a partir d’un reseau */
Graphe* creerGraphe(Reseau* r); 

/* retourne vrai si pour toute arete du graphe, le nombre de chaines qui passe par cette
arete est inferieur a γ, et faux sinon.
Attention, la fonction retourne vrai si le 'r' est NULL*/
int reorganiseReseau(Reseau* r);


/* Retourne Un reseau econstitué a partir des chaines 'C' en utilisant la structure 'structure', ou NULL en cas d'erreur
structure 1: liste, structure 2: table de hachage, structure 3 Arbre quaternaire
si la structure 2 est choisi, la valeur de 'M' sera utilisé pour choisir la taille de la table de hachage
Si 'verification' est vrai , la coherence des aretes avec la valeur de gamma seras verifié et le reseau ne sera retourné que si c'est cohérent*/
Reseau* reconstitueReseau(Chaines *C,int structure ,int M, int verification);
    


#endif
