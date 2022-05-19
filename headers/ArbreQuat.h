#ifndef __ARBRE_QUAT_H__
#define __ARBRE_QUAT_H__
# include "../headers/Reseau.h"
/* Arbre quaternaire contenant les noeuds du reseau */
typedef struct arbreQuat{
    double xc, yc;          /* Coordonnees du centre de la cellule */	
    double coteX;           /* Longueur de la cellule */
    double coteY;           /* Hauteur de la cellule */
    Noeud* noeud;           /* Pointeur vers le noeud du reseau */
    struct arbreQuat *so;   /* Sous-arbre sud-ouest, pour x < xc et y < yc */
    struct arbreQuat *se;   /* Sous-arbre sud-est, pour x >= xc et y < yc */
    struct arbreQuat *no;   /* Sous-arbre nord-ouest, pour x < xc et y >= yc */
    struct arbreQuat *ne;   /* Sous-arbre nord-est, pour x >= xc et y >= yc */
} ArbreQuat;

// determine les coordonnees minimales et maximales des points constituant les differentes chaines du reseau
void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax); 

//creer une cellule de d’arbre quaternaire, de centre (xc, yc), de longueur coteX et de hauteur coteY. La fonction initialisa le nœud du reeseau, les arbres nord-ouest, nordest, sud-ouest et sud-est a NULL
// La fonction rend un pointeur vers l'arbre creer ou NULL en cas d'erreur
ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY);

//Insere un Noeud 'n'  dans un arbre quaternaire 'a'
// 'parent' est l'arbre parent de 'a'
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent);

// Reconstruit le reseau R à partir de la liste des chaines 'C' en utilisant un arbre quaternaire
// Retourne NULL en cas d'erreur
Reseau* reconstitueReseauArbre(Chaines* C);

#endif
