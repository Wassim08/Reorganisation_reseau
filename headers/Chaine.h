#ifndef __CHAINE_H__
#define __CHAINE_H__	
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
# include "../headers/OutilT.h"
#include <time.h>


/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;

/* retourner un pointeur vers une instance de  Chaines allouer dynamiquement a partir du contenu d'un flux 'f' */
Chaines* lectureChaines(FILE *f);

/*ecrit dans un flux 'f' le contenu d’une Chaines 'C'
  on considere que les coordonnees des points ont au maximum 9 exemple (123.456789) */
void ecrireChaines(Chaines *C, FILE *f);

/*ecrit dans un flux 'f' le contenu d’une Chaines 'C'
  on considere que les coordonnees des points ont au maximum 9 chiffres exemple (123.456789)
  cette fonction a la difference de (ecrireChaines) parcours 2 fois chaque chaine , 1 fois pour compter le nombre de point et une deuxieme fois pour ecrire la chaine */
void ecrireChaines_non_eff(Chaines *C, FILE *f);

//creer un fichier SVG en html de nom 'nomInstance' pour visualiser des chaines 'C'
void afficheChainesSVG(Chaines *C, char* nomInstance);


//  retrourne la longueur physique totale des chaines de 'C'
double longueurTotale(Chaines *C);

// Retourne le nombre total de points dans les chaines de 'C'
int comptePointsTotal(Chaines *C);

//desalloue l'espace de l'ensemble de chaines (Chaines) donné en parametre ainsi que celui de chacune de ses chaines
void libereChaines(Chaines* Chaines);

/* Retourne les chaines  contenu dans le fichier  'prefix'+'nomfichier'+'suffix' et retourne NULL en cas d'erreur
  si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier ou avec les chaines qu'il contient il vous seras demander si vous voulez redefinir le nom
  si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon ou que les chaines du fichier ne sont pas bonnes, l'ecriture seras abandonne
  si 'nomfichier' est NULL il vous sera demande d'entre le nom du fichier (meme si 'mode_silencieux' est vrai)
  'prefix' et 'suffix' peuvent etre NULL*/
Chaines * ChargerChainesFile(char * nomfichier ,char* prefix , char * suffix , int mode_silencieux);

/*Ecrit des chaines 'chaines' dans le fichier 'nomfichier'
  si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
  si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
  si 'mode_silencieux' est à 2 ou plus, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
  si 'nomfichier' est NULL il vous sera demande d'entre le nom du fichier (meme si 'mode_silencieux' est vrai) */
void EcrituresChainesFile(char * nomfichier, Chaines* chaines  , int mode_silencieux);

/*Creer un fichier SVG en html de nom 'nomfichier' pour visualiser les chaines 'chaines'
  si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
  si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
  si 'mode_silencieux' est à 2, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2.html
  si 'nomfichier' est NULL il vous sera demande d'entre le nom du fichier (meme si 'mode_silencieux' est vrai) */
void EcritureChainesSVG(char * nomfichier, Chaines *chaines , int mode_silencieux);

/*  Retourne le flux d'un fichier generer avec 'nbChaines' chaines, 'nbPointsChaine' points et des coordonnees de point qui oscillent entre ( 0,xmax) pour x et  (0,ymax) pour y 
    Retourne NULL en cas d'erreur
    'prefix' et 'suffix' peuvent etre NULL
    si 'nomfichier' est non NULL 
        le nom du fichier sauvegarde sera 'prefix'+'nomfichier'+'suffix'
        si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
        si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
        si 'mode_silencieux' est à 2, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
    si 'nomfichier' est NULL un fichier temporaire seras generer (qui sera supprimé une fois tout ses flux fermé) */
FILE* generationFilealeatoire(char* nomfichier ,char *prefix,char*suffix,int nbChaines,int nbPointsChaine,int xmax,int ymax,int mode_silencieux);

/* Retournes un ensemble de chaines genere aleatoirement avec 'nbChaines' chaines, qui ont chacune 'nbPointsChaine' points de coordonnees oscillant entre ( 0,xmax) pour x et  (0,ymax) pour y 
   Retourne NULL en cas d'erreur*/
Chaines* generationAleatoire(int nbChaines,int nbPointsChaine,int xmax,int ymax);
#endif	
