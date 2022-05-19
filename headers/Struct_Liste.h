#ifndef _STRUCT_LISTE_H
#define _STRUCT_LISTE_H

/* Structure d une file contenant un entier */

typedef struct cell_entier{
  int u;
  struct cell_entier *suiv;
} Cell_entier;

typedef Cell_entier * ListeEntier;

/* Initialisation d une file */
void Init_Liste(ListeEntier *L);

/* Teste si la file est vide */
int estLileVide(ListeEntier *L);

/* Ajoute un element don`e en tete de liste */
void ajoute_en_tete(ListeEntier* L, int u);

/* Ajoute un element don`e en tete de liste  mais ne modifie pas L et retourne la liste avec u en tete */
ListeEntier  ajoute_en_tete2(ListeEntier L, int u);

/* Vide et desalloue une liste */
void desalloue(ListeEntier *L);

/* Vide et desalloue un tableau de liste */
void desalloue_tab_liste(ListeEntier* tab_L, int taille);

/* desalloue  les indice  'element_a_effacer' d'un tableau de liste 'tab_L'  en evitant les indice de 'element_a_eviter'
les indices dans 'element_a_eviter' doivent etre dans le meme ordre que le leur ordre d'apparition dans 'element_a_effacer' */
void desalloue_tab_liste2(ListeEntier* tab_L, ListeEntier element_a_effacer , ListeEntier element_a_eviter);

/* affiche une liste */
void afficheListe(ListeEntier L);


#endif 
