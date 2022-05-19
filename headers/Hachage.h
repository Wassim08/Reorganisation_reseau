#ifndef __HACHAGE_H__
#define __HACHAGE_H__

# include "../headers/Reseau.h"

#define A 1.618  // constante utiliser dans la fonction de hachage
#define MAX_M 30000 // Taille maximal de la table
# define M_defaut chaines->nbChaines*5 // maniere de calculer la taille de la table par defaut

/* noeud dans une table de hachage */
typedef struct noeudh {
    Noeud *nd;

    struct tablehachage * voisins; // table des voisins de noeudh
    struct noeudh * suivant ; // noeudh suivant qui a la meme position dans ala table
} NoeudH ;

/* table de hachage */
typedef struct tablehachage {
    int nE ; /*nombre d’elements contenus dans la table de hachage */
    int taille ; /*taille de la table de hachage */
    NoeudH ** tab ; /*table de hachage avec resolution des collisions par chainage */
} TableHachage ;


Reseau *reconstitueReseauHachage(Chaines *C, int M);

//  Libere  l'espace memoire occupé par une Table de Hachage T ainsi que son tableau de NoeudH tab
//  Attention, libererTableHachage  doit etre utiliser conjointement avec libererReseau afin de liberer l'attribut nd des NoeudH
void libererTableHachage(TableHachage *T);
#endif