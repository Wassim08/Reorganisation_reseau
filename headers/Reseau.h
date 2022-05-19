#ifndef __RESEAU_H__
#define __RESEAU_H__
#include "Chaine.h"
typedef struct noeud Noeud;

/* Liste chainee de noeuds (pour la liste des noeuds du reseau ET les listes des voisins de chaque noeud) */
typedef struct cellnoeud
{
    Noeud *nd;              /* Pointeur vers le noeud stock\'e */
    struct cellnoeud *suiv; /* Cellule suivante dans la liste */
} CellNoeud;

/* Noeud du reseau */
struct noeud
{
    int num;            /* Numero du noeud */
    double x, y;        /* Coordonnees du noeud*/
    CellNoeud *voisins; /* Liste des voisins du noeud */
};

/* Liste chainee de commodites */
typedef struct cellCommodite
{
    Noeud *extrA, *extrB;       /* Noeuds aux extremites de la commodite */
    struct cellCommodite *suiv; /* Cellule suivante dans la liste */
} CellCommodite;

/* Un reseau */
typedef struct
{
    int nbNoeuds;              /* Nombre de noeuds du reseau */
    int gamma;                 /* Nombre maximal de fibres par cable */
    CellNoeud *noeuds;         /* Liste des noeuds du reseau */
    CellCommodite *commodites; /* Liste des commodites a relier */
} Reseau;

// retourne un Noeud du reseau R correspondant au point (x, y) dans la liste chaınee noeuds de R
// Retourne NULL en cas d'erreur
// si le point existe deja , le noeud correspondant et retourne, sinon le noeud est creer et R->nbNoeuds est incremente
Noeud *rechercheCreeNoeudListe(Reseau *R, double x, double y);

/* Retourne un Reseau creer a partir des chaines 'C'*/
// Retourne NULL en cas d'erreur
Reseau *reconstitueReseauListe(Chaines *C);

/* Retourne un Reseau creer a partir des chaines 'C'*/
/* celui si a la difference de 'reconstitueReseauListe' utilise une matrice pour eviter les doublons dans les listes de voisins */
// Retourne NULL en cas d'erreur
Reseau *reconstitueReseauListe2(Chaines *C);

// Ecrit un Reseau 'R' dans le flux du donnes indique par 'f'
void ecrireReseau(Reseau *R, FILE *f);
// retourne le nombre de Liaisons de 'R'
int nbLiaisons(Reseau *R);
// retourne le nombre de commodites de 'R'
int nbCommodites(Reseau *R);

//creer un fichier SVG en html de nom 'nomInstance' pour visualiser le reseau 'R'
void afficheReseauSVG(Reseau *R, char *nomInstance);

// libere un Reseau et ses composantes
void libererReseau(Reseau **R);

/* fait un ajout en tete de 'cell' dans le reseau 'r' */
void ajouter_cellnoeud(Reseau *r, CellNoeud *n);

// Retourne un pointeur vers un CellNoeud qui a 'nd' comme noeud
// Retourne NULL en cas d'erreur
CellNoeud *creerCellNoeud_nd(Noeud *nd);

// Retourne pointeur vers un noeud creer de numero 'num' d'attribut x 'x' et y 'y'
// Retourne NULL en cas d'erreur
Noeud *creerNoeud(int num, double x, double y);

// Retourne pointeur vers un Cell noeud qui a un noeud de numero 'num' d'attribut x 'x' et y 'y'
// Retourne NULL en cas d'erreur
CellNoeud *creerCellNoeud(int num, double x, double y);

/* Retourne une commodite ayait les attribut extrA et extrB */
// Retourne NULL en cas d'erreur
CellCommodite *creerCommodite(Noeud *extrA, Noeud *extrB);

/* Verifies si 'noeud' et 'noeudPrec' ne sont pas deja dans leurs listes de voisins l'un de l'autre
    si ils n'y sont pas les listes de voisins  sont mis a jour'
    'P_insertionNoeudVoisin' est le pointeur sur l'endroit ou inserer 'noeud' dans la liste des voisins de 'noeudPrec'
    La fonction retourne un pointeur sur ou inserer le prochain voisin de 'noeud'
    Retourne NULL en cas d'erreur */
CellNoeud **rechercheVoisinAjoute(Noeud *noeud, Noeud *noeudPrec, CellNoeud **P_insertionNoeudVoisin);

/*Ecrit un reseau R dans le fichier 'nomfichier'
    si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
    si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
    si 'mode_silencieux' est à 2, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
    si 'nomfichier' est NULL il vous sera demande d'entre le nom du fichier (meme si 'mode_silencieux' est vrai) */
void EcrituresReseauFile(char *nomfichier, Reseau *R, int mode_silencieux);

/*Creer un fichier SVG en html de nom 'nomfichier' pour visualiser le reseau 'R'
    si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
    si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
    si 'mode_silencieux' est à 2, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2.html
    si 'nomfichier' est NULL il vous sera demande d'entre le nom du fichier (meme si 'mode_silencieux' est vrai) */
void EcritureReseauSVG(char *nomfichier, Reseau *R, int mode_silencieux);

#endif
