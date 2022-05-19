#ifndef __OutilT_H__
#define __OutilT_H__

#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>


#define xstr(s) str(s) // perermet de stringifier une Macro
#define str(s) #s

/*  Rajoute a 'partie1' le contenu de 'partie' en fin de chaine
    'partie1' doit etre un pointeur vers une chaine de caractere allouer dynamiquement
    la taille de l'espace memoir reserve pour 'partie' sera modifie a len('partie1') + len('partie2') +1
    Retourne un pointeur vers le premier char du resulatat de la concatenation si celle si reussi et NULL sinon */
char * concatenate( char ** partie1, const char * partie2 );

    /*  Rajoute a 'partie2' le contenu de 'partie1' en debut de chaine
        'partie1' doit etre un pointeur vers une chaine de caractere allouer dynamiquement
        la taille de l'espace memoir reserve pour 'partie2' sera modifie a len('partie1') + len('partie2') +1
        Retourne un pointeur vers le premier char du resulatat de la concatenation si celle si reussi et NULL sinon */
char * r_concatenate( char**  partie2, const char * partie1 );

//Met en minisucule la chaine de caractere (src) passe en parametre
void toLower(char * src);

//Retourne une copie  en minisucule la chaine de caractere (src) passe en parametre (src n'est pas modifie)
// Attention a ne pas oublie de liberer la copie apres son utilisation
char *lowerOf(char * src);

//permet de vider le tampon apres une saisie
void  vide_tampon();

/* Permet de recuperer un entier  depuis l'entree standard
   La fonction retourne la valeur 0 si l'entree ne contient pas une représentation de valeur numérique. Du coup, il n'est pas possible de distinguer l'entree "0" d'une chaîne ne contenant pas un nombre entier
   etat n'est pas obligatoire, il peut etre mis a NULL */
int getInt();

/* Permet de recuperer un entier  depuis l'entree standard
    La fonction met le pointeur 'etat' à
            -2 si l'entree n'a pas pu etre interpretée
            -1 si aucun entier n'a été trouvée dans l'entree avant un caractere qui n'est pas espace
                0 si l'entree est composée uniquement d'un entier 
                1 si l'entree contient d'abord un entier puis autre que des espaces
    'etat' n'est obligatoire, il  peut etre entree a  NULL */
long getLong(int *etat);


// Fonction permettant de recuperer la reponse d'une question posé a l'utilisateur ou il doit repondre par y ou n
// les reponses oui, o, yes, non, n, no, sont aussi interpreté par la fonction peu importe la casse
// les reponses posives y,yes,o,oui rendent 1
// les reponses negatives n,no,non rendent -1 
// si apres 5 tentatives l'utilisateur ne donne pas une reponse interpretable la fonction rend 0
// en cas d'erreur du fgets on rend 2
int yes_no();



/* Retourne un long entre par l'utilisateur entre 'valMin' et 'valMax' compris
    si 'DefaultValue' est non NULL et que l'entree n'est pas un entier dans ['valMin' ,'valMax' ] ou que l'entree a ete mal faite, 'DefaultValue' est retourne 
    La fonction met le pointeur 'etat' à
            -2 si l'entree n'a pas pu etre interpretée
            -1 si aucun entier n'a été trouvée dans l'entree avant un caractere qui n'est pas espace
                0 si l'entree est composée uniquement d'un entier 
                1 si l'entree contient d'abord un entier puis autre que des espaces
                2 si l'entree n'est pas dans ['valMin' ,'valMax' ]
    'etat' n'est obligatoire, il  peut etre entree a  NULL */
long getLongbeetween(int *etat, long valMin, long valMax, long* DefaultValue);


/* Affiche 'affichage' a l'utilisateur et lui demande de rentre une valeur entre 'valMin' et 'valMax' compris
    l'utilisateur a 'essaiMax' - *'essai' restant pour rentrer une bonne valeur, si essai est NULL l'utilisateur n'a qu'un essai
    si au bout des essai l'entree de l'utilisateur n'est toujours pas bonne, 'DefaultValue' est retournee
    si 'failIndication' est vrai et que l'utisateur a fait une mauvaise entree, une indication sur l'erreur lui sera affiche
    affichage et essai peuvent etre NULL */
long menu(int * essai, int essaiMax,long valMin, long valMax,long DefaultValue, char * affichage, int failIndication);

// Retourne une chaine de caractere entrer depuis l'entree standard
// Retourne NULL en cas d'erreur
// la chaine retourne doit etre liberer apres son utilisation
char * recup_str();

/*  Retourne un flux de lecture correspondant a 'prefix'+ *'P_nomfichier' + 'suffix'
    Retourn NULL si aucun flux n'a pu etre ouvert
    si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez redefinir le nom
    si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne
    si 'P_nomfichier' pointe vers un NULL il vous sera demande d'entrer le nom du fichier (meme si 'mode_silencieux' est vrai)
    si 'P_nomfichier' ne pointe pas vers un NULL, Il doit obligatoirement pointer vers un espace memoire allouer dynamiquement
    'prefix' et 'suffix' peuvent etre NULL
    si 'redef_chemin' est vrai  on considerera que une tentative d'ouverture a deja ete effectue et on demande directement a l'utilisateur de donner un nouveau nom de fichier
    'redef_chemin' vrai annule le mode_silencieux
    Il faudra liberer la valeurs pointer par P_nomfichier apres la fin de son utilisation  */
FILE * Open_read_only(char ** P_nomfichier,char* prefix,char* suffix ,int redef_chemin , int mode_silencieux);

/*  Retourne un flux d'ecriture correspondant a 'prefix'+ *'P_nomfichier' + 'suffix'
    Retourn NULL si aucun flux n'a pu etre ouvert
    si 'P_nomfichier' pointe vers un NULL il vous sera demande d'entrer le nom du fichier (meme si 'mode_silencieux' est vrai)
    si 'P_nomfichier' ne pointe pas vers un NULL, Il doit obligatoirement pointer vers un espace memoire allouer dynamiquement
    si 'P_nomfichier' pointe vers une chaine egal a "stdout" (peut importe la casse) un flux vers la sortie standart sera rendu (peu importe le prefix et le suffix)
    si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
    si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
    si 'mode_silencieux' est à 2 ou plus, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
    'prefix' et 'suffix' peuvent etre NULL
    Il faudra liberer la valeurs pointer par P_nomfichier apres la fin de son utilisation  */
FILE * Open_write_only(char ** P_nomfichier,char *prefix,char * suffix,int mode_silencieux);

/* Ecrit dans le fichier 'nomfichier' une instance de 'objet' avec l'aide de la fonction d'ecriture 'fonc_decriture'
    si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
    si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
    si 'mode_silencieux' est à 2 ou plus, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
    si 'nomfichier' est NULL il vous sera demande d'entrer le nom du fichier (meme si 'mode_silencieux' est vrai)
    si 'nomfichier'  a "stdout" (peut importe la casse) l'ecriture sera faite dans la sortie standart */
void EcrituresFile(char * nomfichier,void (*fonc_decriture)() ,  void* objet,  int mode_silencieux);

/* Ecrit dans le fichier 'nomfichier' une instance de 'objet' au format SVG avec l'aide de la fonction d'ecriture 'fonc_decriture'
    si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
    si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
    si 'mode_silencieux' est à 2 ou plus, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
    si 'nomfichier' est NULL il vous sera demande d'entrer le nom du fichier (meme si 'mode_silencieux' est vrai) */
void EcritureSVG(char * nomfichier, void (*fonc_decriture)() ,void *objet , int mode_silencieux);
/*  Retourne l'indice dans un tableau a 1 dimension que aurait [ligne][colonne] dans un tableau a un dimension*/
int getIndex(int ligne, int colonne , int nb_colonne);
/* Retourne le maximum entre 'num1' et 'num2'*/
int max(int num1 , int num2);

/* Retourne le minimum entre 'num1' et 'num2'*/
int min(int num1 , int num2);
#endif	