#include "../headers/Reseau.h"
#include "../headers/SVGwriter.h"

void libererCommodites(CellCommodite *CellCommod)
{
    // libere l'espace une chaines de commodites
    CellCommodite *suiv;
    while (CellCommod)
    {
        suiv = CellCommod->suiv;
        free(CellCommod);
        CellCommod = suiv;
    }
}
void libererNoeud(Noeud *noeud)
{
    // libere un noeud ainsi et ses composantes
    CellNoeud *voisin = noeud->voisins;
    while (voisin)
    {
        noeud->voisins = voisin->suiv;
        free(voisin);
        voisin = noeud->voisins;
    }
    free(noeud);
}
void libererCellNoeuds(CellNoeud *noeuds)
{
    // libere un CellNoeuds et ses composantes
    CellNoeud *suiv;
    while (noeuds)
    {
        libererNoeud(noeuds->nd);
        suiv = noeuds->suiv;
        free(noeuds);
        noeuds = suiv;
    }
}
void libererReseau(Reseau **R)
{
    // libere un Reseau et ses composantes
    if (!(*R))
        return;
    libererCellNoeuds((*R)->noeuds);
    libererCommodites((*R)->commodites);
    free((*R));
    *R = NULL;
}
Noeud *creerNoeud(int num, double x, double y)
{
    // Retourne pointeur vers un noeud creer de numero 'num' d'attribut x 'x' et y 'y'
    // Retourne NULL en cas d'erreur
    Noeud *nd = (Noeud *)calloc(1, sizeof(Noeud));
    if (!nd)
    {
        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour un Noeud\n");
        return NULL;
    }
    nd->num = num;
    nd->x = x;
    nd->y = y;
    return nd;
}

CellNoeud *creerCellNoeud(int num, double x, double y)
{
    // Retourne pointeur vers un Cell noeud qui a un noeud de numero 'num' d'attribut x 'x' et y 'y'
    // Retourne NULL en cas d'erreur
    CellNoeud *cell = (CellNoeud *)calloc(1, sizeof(CellNoeud));
    if (!cell)
    {
        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour une CellNoeud\n");
        return NULL;
    }
    if (!(cell->nd = creerNoeud(num, x, y)))
    {
        free(cell);
        return NULL;
    }
    return cell;
}
CellNoeud *creerCellNoeud_nd(Noeud *nd)
{
    // Retourne un pointeur vers un CellNoeud qui a 'nd' comme noeud
    // Retourne NULL en cas d'erreur
    CellNoeud *cell = (CellNoeud *)calloc(1, sizeof(CellNoeud));
    if (!cell)
    {
        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour un CellNoeud\n");
        return NULL;
    }
    cell->nd = nd;
    return cell;
}

Noeud *rechercheCreeNoeudListe(Reseau *R, double x, double y)
{
    // retourne un Noeud du reseau R correspondant au point (x, y) dans la liste chaınee noeuds de R
    // Retourne NULL en cas d'erreur
    // si le point existe deja , le noeud correspondant et retourne, sinon le noeud est creer et R->nbNoeuds est incremente

    if (!R)
    {
        fprintf(stderr, "Erreur : Le Reseau n'est n'est pas defini\n");
        return NULL;
    }
    CellNoeud **chaineNoeuds = &(R->noeuds); // on prend un pointeur de pointeur au cas ou on doit ajouté un noeuds
    Noeud *nd;
    int i = 1; // cette variable sert simplement au cas ou on aurait un noeud qui n'est pas defini, sa position pourra etre indiquee dans le message d'erreur
    while (*chaineNoeuds)
    {
        if (!(nd = (*chaineNoeuds)->nd))
        {
            fprintf(stderr, "Erreur : Le Noeud %d du reseau n'est n'est pas defini\n", i);
            chaineNoeuds = &(*chaineNoeuds)->suiv; // le fait d'avoir un noeud non defini est anormale mais on peut quand poursuivre la recherche
            continue;
        }
        if ((nd->x == x) && (nd->y == y)) // on regarde la correspondance avec les coordonnees donnee en argument.
            return nd;

        chaineNoeuds = &(*chaineNoeuds)->suiv; // si on a pas trouvé on passe au suivant
        i++;
    }
    //si on arrive a la fin du while c'est que la recherche n'a pas permis d'identifier le noeud dans la liste , donc on doit le creer
    if (!(*chaineNoeuds = creerCellNoeud(R->nbNoeuds + 1, x, y)))
    {
        return NULL;
    }

    R->nbNoeuds++; // si ca a marche on incremente le nombre de noeud, puis on rend le nouveau noeud
    return (*chaineNoeuds)->nd;
}
CellCommodite *creerCommodite(Noeud *extrA, Noeud *extrB)
{
    /* Retourne une commodite ayait les attribut extrA et extrB */
    // Retourne NULL en cas d'erreur
    CellCommodite *commodite = (CellCommodite *)malloc(sizeof(CellCommodite));
    if (!commodite)
    {
        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour une commodite\n");
        return NULL;
    }
    commodite->extrA = extrA;
    commodite->extrB = extrB;
    commodite->suiv = NULL;
    return commodite;
}

CellNoeud **rechercheVoisinAjoute(Noeud *noeud, Noeud *noeudPrec, CellNoeud **P_insertionNoeudVoisin)
{
    /* Verifies si 'noeud' et 'noeudPrec' ne sont pas deja dans leurs listes de voisins l'un de l'autre
       si ils n'y sont pas les listes de voisins  sont mis a jour'
       'P_insertionNoeudVoisin' est le pointeur sur l'endroit ou inserer 'noeud' dans la liste des voisins de 'noeudPrec'
       La fonction retourne un pointeur sur ou inserer le prochain voisin de 'noeud'
       Retourne NULL en cas d'erreur */
    if (noeud == NULL || noeudPrec == NULL || P_insertionNoeudVoisin == NULL)
        return NULL;
    if (noeud == noeudPrec) // on verifie que les 2 noeud ne soit pas les meme
        return P_insertionNoeudVoisin;

    // on compare les noeuds de la liste des voisins de 'noeud' a 'noeudPrec'
    CellNoeud **noeudCompare = &(noeud->voisins);
    while (*noeudCompare)
    {
        if ((*noeudCompare)->nd == noeudPrec) // si une correspondance est trouve, on s'arrete
            return &(*noeudCompare)->suiv;

        noeudCompare = &(*noeudCompare)->suiv;
    } // si on arrive a la fin du while c'est que la relation de voisinage n'est pas encore creer donc on la fait
    CellNoeud *cell = creerCellNoeud_nd(noeudPrec);
    if (!cell)
        return NULL;

    // on insere 'noeudPrec' en queue de la liste des voisins de 'noeud'
    *noeudCompare = cell;

    // on met 'noeudCompare' comme l'adresse ou seras inserer le prochain voisin de 'noeud'
    noeudCompare = &cell->suiv;
    if (!(cell = creerCellNoeud_nd(noeud)))
        return NULL;
    if (*P_insertionNoeudVoisin)
    { // si il y a deja un noeud a l'emplacement on ne peut pas inserer directement
        cell->suiv = *P_insertionNoeudVoisin;
        *P_insertionNoeudVoisin = cell;
        return noeudCompare;
    }
    *P_insertionNoeudVoisin = cell; //si il y a pas de noeud on insere directement
    return noeudCompare;
}

int ajoutevoisint(Noeud *noeud1, Noeud *noeud2)
{
    /*Insere en tete 'noeud1' dans la liste des voisins de 'noeud2' et insere 'noeud2' en tete dans la liste des voisins de 'noeud1'
    La fonction retourne 0 si une erreur s'est produite et 1 sinon*/

    // On creer le CellNoeud
    CellNoeud *cell = creerCellNoeud_nd(noeud1);
    if (!cell)
        return 0;

    // puis on l'insere
    cell->suiv = noeud2->voisins;
    noeud2->voisins = cell;

    // on refait pareil en échangant noeud1 et noeud2
    if (!(cell = creerCellNoeud_nd(noeud2)))
        return 0;

    cell->suiv = noeud1->voisins;
    noeud1->voisins = cell;
    return 1;
}
int rechercheVoisinAjoute2(char *matrice, Noeud *noeud1, Noeud *noeud2, int nbColonnes)
{
    /* verifies si 'noeud' et 'noeud2' ne sont pas deja dans leurs listes de voisins l'un de l'autre a l'aide de la matrice sommet-sommet 'matrice'
    si ils n'y sont pas les listes de voisins ainsi que 'matrice' sont mis a jour'
    'nbColonnes' indique le nombre de colonne de 'matrice'
    La fonction retourne 0 si un erreur s'est produite et 1 sinon*/

    if (noeud1 == NULL || noeud2 == NULL || matrice == NULL || nbColonnes == 0)
        return 0;

    // on recupere les numeros des noeuds
    int num1 = noeud1->num;
    int num2 = noeud2->num;

    // on recupere le minimum pour s'assurer qu'il n'est pas negatif
    int minMax = min(num1, num2);

    if (minMax < 0)
    {
        fprintf(stderr, "Le numeros des noeuds doit etre positif\n");
        return 0;
    }
    // on prend le max pour verifier que les noeud peuvent bien rentrer dans la matrice
    minMax = max(num1, num2);

    if (minMax >= nbColonnes)
    {
        fprintf(stderr, "Erreur: le noeud %d est hors des bornes de la marice sommet-sommet\n", minMax);
        return 0;
    }

    int index1 = getIndex(num1, num2, nbColonnes);

    // si matrice[index1] est vrai c'est que les voisins ont deja été ajouté
    if (matrice[index1])
        return 1;

    int index2 = getIndex(num2, num1, nbColonnes);

    matrice[index1] = 'o';
    matrice[index2] = 'o';

    return ajoutevoisint(noeud1, noeud2);
}

Reseau *reconstitueReseauListe(Chaines *C)
{
    /* Retourne un Reseau creer a partir des chaines 'C'*/
    // Retourne NULL en cas d'erreur

    if (!C)
    { // Si C n'est pas defini on arrete
        fprintf(stderr, "Erreur : L'Ensemble de chaines n'est pas defini\n");
        printf("La reconstitution est abandonnee\n");
        return NULL;
    }
    Reseau *R = (Reseau *)calloc(1, sizeof(Reseau));

    if (!R)
    {
        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour un Reseau\n");
        printf("Reconstitution abandonnee\n");
        return NULL;
    }
    R->gamma = C->gamma;
    CellChaine *chaine = C->chaines;
    CellPoint *point;
    Noeud *noeudPrec;                                //pointeur vers le noeud precedent, il va etre utiliser pour verifier si le noeud etudié apres est deja dans sa liste de voisin
    CellCommodite **P_commodites = &(R->commodites); // Pointeur vers l'endroit ou doit etre ajouter la prochaine commodité
    Noeud *noeud;                                    // Pointeur vers le noeud analysé couramment
    Noeud *premier_noeud;                            // Pointeur vers le premier de la chaine, il va etre utilisé pour creer la commodite
    CellNoeud **P_insertionNoeudVoisin;              // pointeur vers l'endroit ou sera inserer le noeud dans la liste des noeuds voisins du noeuds precedent
    // on parcourt les chaines
    for (int x = 0; x < C->nbChaines; x++)
    { // on fait un for sur nbChaines au lieu de faire un while(chaine) pour s'assurer que le nombre de chaines indiqué correspond bien au nombre de chaines existante

        if (!chaine)
        { // si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on arrete la boucle
            fprintf(stderr, "Erreur :  %d chaine(s) trouvee(s) au lieu de %d\n", x, C->nbChaines);
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        if (!(point = chaine->points))
        { // on verifie si il y a bien des points dans la chaines, sinon en passe a la suivante (meme si en theorie il ne devrait pas y avoir de chaines sans points)
            chaine = chaine->suiv;
            continue;
        }
        P_insertionNoeudVoisin = NULL;

        // on cherche le premier noeud de la chaine
        noeudPrec = rechercheCreeNoeudListe(R, point->x, point->y);
        premier_noeud = noeudPrec;
        if (!noeudPrec)
        {
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        if (!(point = point->suiv))// Le cas ou la chaines n'est consitué que d'un point
        { 
            printf("La chaine %d n'est consituee que d'un point\n", chaine->numero);
            if (!(*P_commodites = creerCommodite(noeudPrec, noeudPrec))) // on creer quand meme la commodité
            { 
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                return NULL;
            }
            P_commodites = &(*P_commodites)->suiv;
            chaine = chaine->suiv;
            continue;
        }
        P_insertionNoeudVoisin = &noeudPrec->voisins;
        // on parcourt les points de la chaine
        while (point)
        {
            // on recupere le noeuds correspondant au point
            noeud = rechercheCreeNoeudListe(R, point->x, point->y);
            // on met a jour les voisins
            P_insertionNoeudVoisin = rechercheVoisinAjoute(noeud, noeudPrec, P_insertionNoeudVoisin);
            if (!P_insertionNoeudVoisin)
            {
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                return NULL;
            }
            noeudPrec = noeud;
            point = point->suiv;
        } // apres avoir parcouru tout les points de la chaine, on creer la comodité
        if (!(*P_commodites = creerCommodite(premier_noeud, noeud)))
        {
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        P_commodites = &(*P_commodites)->suiv;
        chaine = chaine->suiv;
    }
    
    if (chaine)
    { // si chaine n'est pas NULL c'est que nbChaines est inférieur au nombre de  chaine de chaines existants, donc  on rapport l'erreur
        fprintf(stderr, "Erreur :  Il y a plus de chaine que le nombre indique\n");
        printf("Annulation de la reconstitution\n");
        libererReseau(&R);
        return NULL;
    }
    return R;
}

int nbCommodites(Reseau *R)
{
    // retourne le nombre de commodites de 'R'
    int nb = 0;
    CellCommodite *commod = R->commodites;
    while (commod)
    {
        nb++;
        commod = commod->suiv;
    }
    return nb;
}

int nbLiaisons(Reseau *R)
{
    // retourne le nombre de Liaisons de 'R'
    int nb = 0;
    CellNoeud *noeuds = R->noeuds;
    CellNoeud *noeud;
    while (noeuds)
    {
        noeud = noeuds->nd->voisins;
        while (noeud)
        {
            nb++;
            noeud = noeud->suiv;
        }
        noeuds = noeuds->suiv;
    }
    return nb / 2;
}

void ecrireReseau(Reseau *R, FILE *f)
{
    // Ecrit un Reseau 'R' dans le flux du donnes indique par 'f'
    if (!R)
    {
        fprintf(stderr, "Erreur : Le Reseau n'est pas defini\n");
        printf("L'ecriture est abandonne\n");
        return;
    }
    if (!f)
    {
        fprintf(stderr, "Erreur : Le fichier de destination n'est pas defini\n");
        printf("L'ecriture est abandonne\n");
        return;
    }

    // on ecrit d'abord le nombre de commodites ainsi que le nombre de liaisons
    int NbLiaisons = nbLiaisons(R);
    int NbCommodites = nbCommodites(R);
    if (fprintf(f, "NbNoeuds: %d\nNbLiaisons: %d\nNbCommodites: %d\nGamma: %d\n\n", R->nbNoeuds, NbLiaisons, NbCommodites, R->gamma) < 52)
    { // 52 correspond aux nombre de caractere minimum que peux prendre ce fprinft
        fprintf(stderr, "Erreur lors de l'ecriture du nombre de chaines et de Gamma\nL'ecriture est arretee\n");
        return;
    }
    int liaisonPlace = 1;
    int temp = R->nbNoeuds;
    while ((temp /= 10))
        liaisonPlace++;                  // permet de savoir le nombre de caractere maximum que utilise le 'num' d'un noeud, il va servir plus tard a savoir l'espace a allouer pour le buffer des liaisons
    liaisonPlace = liaisonPlace * 2 + 5; // la liaison est composé de 2 numéros + 'l' + 2 espaces + '\n'+'\0'
    int nb = 0;                          // permet de savoir ou on doit ecrire dans le buffer
    int verifSprintf;

    int espace = 30 * sizeof(char *);      //espace allouer dans le malloc de bufffer
    char *buffer = (char *)malloc(espace); // le buffer va garder en memoir les liaisons, pour eviter d'avoir a parourire 2 fois lz liste des noeuds du reseau
    char *temp2;

    if (!buffer)
    {
        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour le buffer de chaines\n");
        printf("L'ecriture est abandonee\n");
        return;
    }
    CellNoeud *noeuds = R->noeuds;
    int num;
    CellNoeud *voisin;
    int verifNbliaisons = 0;
    // on parcour les noeuds du reseau
    for (int x = 0; x < R->nbNoeuds; x++) // on fait un for sur nbNoeuds au lieu de faire un while(noeuds) pour s'assurer que le nombre de noeuds indiqué correspond bien au nombre de noeuds ecrit
    {
        if (!noeuds)
        { // si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on stop l'ecriture
            fprintf(stderr, "Erreur :  %d noeud(s) trouve(s) au lieu de %d\n", x, R->nbNoeuds);
            printf("L'ecriture est abandonee\n");
            free(buffer);
            return;
        }
        num = noeuds->nd->num;
        if (fprintf(f, "v %d %lf %lf\n", num, noeuds->nd->x, noeuds->nd->y) < 22)
        {
            fprintf(stderr, "Erreur lors de l'ecriture du noeud %d\nL'ecriture est arretee\n", num);
            printf("L'ecriture est abandonee\n");
            free(buffer);
            return;
        }
        voisin = noeuds->nd->voisins;
        // on parcour les voisin de  'noeuds'
        while (voisin)
        {
            if (num < voisin->nd->num)
            { // on ajoute la liaison que si num est inférieur a celui du voisin pour eviter les doublons
                if ((nb + liaisonPlace) * sizeof(char) > espace)
                { // on s'assure qu'il y a assez de place dans le buffer pour la nouvelle liaison, sinon on l'agrandit
                    espace += 30;
                    if (!(temp2 = (char *)realloc(buffer, espace)))
                    {
                        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour le buffer de la liaison l %d %d\n", num, voisin->nd->num);
                        printf("L'ecriture est abandonee\n");
                        free(buffer);
                        return;
                    }
                    buffer = temp2;
                }
                verifSprintf = nb + 6; // permet de verifier si le sprint a bien fonctionne
                if ((nb += sprintf(buffer + nb, "l %d %d\n", num, voisin->nd->num)) < verifSprintf)
                {
                    fprintf(stderr, "Erreur lors de l'ecriture de la liaison l %d %d\n", num, voisin->nd->num);
                    free(buffer);
                    return;
                }
                verifNbliaisons++;
            }
            voisin = voisin->suiv;
        }
        noeuds = noeuds->suiv;
    }
    if (noeuds)
    { // si noeuds n'est pas NULL c'est que nbNoeuds est inférieur au nombre de noeuds du reseau, donc  on rapport l'erreur et on arrete l'ecriture
        fprintf(stderr, "Erreur :  Il y a plus de noeuds que le nombre indique\n");
        free(buffer);
        return;
    }
    fprintf(f, "\n");
    if (verifNbliaisons != NbLiaisons)
    {
        fprintf(stderr, "Erreur %d liaisons trouvee au lieu alors que %d a ete indique\n", verifNbliaisons, NbLiaisons);
        printf("L'ecriture est abandonne\n");
        free(buffer);
        return;
    }
    if (verifNbliaisons)
    { // on peut maintenant ecrire le contenu du buffer dans le flux
        if (fprintf(f, "%s\n", buffer) != nb + 1)
        {
            fprintf(stderr, "Erreur lors de l'ecriture des liaisons\n");
            printf("L'ecriture est abandonne\n");
            free(buffer);
            return;
        }
    }
    free(buffer);
    CellCommodite *commod = R->commodites;
    // on parcour les commodite du Reseau
    for (int x = 0; x < NbCommodites; x++)
    { // on fait un for sur NbCommodites au lieu de faire un while(commod) pour s'assurer que le nombre de commodité indiqué correspond bien au commodité ecrite
        if (fprintf(f, "k %d %d\n", commod->extrA->num, commod->extrB->num) < 6)
        {
            fprintf(stderr, "Erreur lors de l'ecriture de la commodite k %d %d\n", commod->extrA->num, commod->extrB->num);
            printf("L'ecriture est abandonne\n");
            return;
        }
        commod = commod->suiv;
    }
    if (commod) // si la commodite n'est pas NULL c'est que NbCommodites est inférieur au nombre de commodites du reseau, donc  on rapport l'erreur
        fprintf(stderr, "Erreur :  Il y a plus de commodites que le nombre indique\n");
}

void afficheReseauSVG(Reseau *R, char *nomInstance)
{
    //creer un fichier SVG en html de nom 'nomInstance' pour visualiser le reseau 'R'
    CellNoeud *courN, *courv;
    SVGwriter svg;
    double maxx = 0, maxy = 0, minx = 1e6, miny = 1e6;

    courN = R->noeuds;
    while (courN != NULL)
    {
        if (maxx < courN->nd->x)
            maxx = courN->nd->x;
        if (maxy < courN->nd->y)
            maxy = courN->nd->y;
        if (minx > courN->nd->x)
            minx = courN->nd->x;
        if (miny > courN->nd->y)
            miny = courN->nd->y;
        courN = courN->suiv;
    }
    SVGinit(&svg, nomInstance, 500, 500);
    courN = R->noeuds;
    while (courN != NULL)
    {
        SVGpoint(&svg, 500 * (courN->nd->x - minx) / (maxx - minx), 500 * (courN->nd->y - miny) / (maxy - miny));
        courv = courN->nd->voisins;
        while (courv != NULL)
        {
            if (courv->nd->num < courN->nd->num)
                SVGline(&svg, 500 * (courv->nd->x - minx) / (maxx - minx), 500 * (courv->nd->y - miny) / (maxy - miny), 500 * (courN->nd->x - minx) / (maxx - minx), 500 * (courN->nd->y - miny) / (maxy - miny));
            courv = courv->suiv;
        }
        courN = courN->suiv;
    }
    SVGfinalize(&svg);
}

void ajouter_cellnoeud(Reseau *r, CellNoeud *cell)
{
    /* fait un ajout en tete de 'cell' dans le reseau 'r' */
    if (r->noeuds == NULL)
    {
        r->noeuds = cell;
    }
    else
    {
        cell->suiv = r->noeuds;
        r->noeuds = cell;
    }
}

void EcrituresReseauFile(char *nomfichier, Reseau *R, int mode_silencieux)
{
    //Ecrit un reseau R dans le fichier 'nomfichier'
    //si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
    //si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
    //si 'mode_silencieux' est à 2, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
    //si 'nomfichier' est NULL il vous sera demande d'entre le nom du fichier (meme si 'mode_silencieux' est vrai)
    EcrituresFile(nomfichier, ecrireReseau, R, mode_silencieux);
}

void EcritureReseauSVG(char *nomfichier, Reseau *R, int mode_silencieux)
{
    //Creer un fichier SVG en html de nom 'nomfichier' pour visualiser le reseau 'R'
    //si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
    //si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
    //si 'mode_silencieux' est à 2, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2.html
    //si 'nomfichier' est NULL il vous sera demande d'entre le nom du fichier (meme si 'mode_silencieux' est vrai)
    EcritureSVG(nomfichier, afficheReseauSVG, R, mode_silencieux);
}

Reseau *reconstitueReseauListe2(Chaines *C)
{
    /* Retourne un Reseau creer a partir des chaines 'C'*/
    /* celui si a la difference de 'reconstitueReseauListe' utilise une matrice pour eviter les doublons dans les listes de voisins */
    // Retourne NULL en cas d'erreur

    if (!C)
    { // Si C n'est pas defini on arrete
        fprintf(stderr, "Erreur : L'Ensemble de chaines n'est pas defini\n");
        printf("La reconstitution est abandonnee\n");
        return NULL;
    }
    Reseau *R = (Reseau *)calloc(1, sizeof(Reseau));

    if (!R)
    {
        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour un Reseau\n");
        printf("Reconstitution abandonnee\n");
        return NULL;
    }
    R->gamma = C->gamma;
    int taille = comptePointsTotal(C) + 1;
    char *matrice = calloc((taille) * (taille), sizeof(char));
    if (!matrice)
    {
        fprintf(stderr, "Impossible d'allouer l'espace pour  la matrice sommet-sommet\n");
        printf("Reconstitution abandonnee\n");
        free(R);
        return NULL;
    }

    CellChaine *chaine = C->chaines;
    CellPoint *point;
    Noeud *noeudPrec;                                //pointeur vers le noeud precedent, il va etre utiliser pour verifier si le noeud etudié apres est deja dans sa liste de voisin
    CellCommodite **P_commodites = &(R->commodites); // Pointeur vers l'endroit ou doit etre ajouter la prochaine commodité
    Noeud *noeud;                                    // Pointeur vers le noeud analysé couramment
    Noeud *premier_noeud;                            // Pointeur vers le premier de la chaine, il va etre utilisé pour creer la commodite
    for (int x = 0; x < C->nbChaines; x++)
    { // on fait un for sur nbChaines au lieu de faire un while(chaine) pour s'assurer que le nombre de chaines indiqué correspond bien au nombre de chaines existante

        if (!chaine)
        { // si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on arrete la boucle
            fprintf(stderr, "Erreur :  %d chaine(s) trouvee(s) au lieu de %d\n", x, C->nbChaines);
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            free(matrice);
            return NULL;
        }
        if (!(point = chaine->points))
        { // on verifie si il y a bien des points dans la chaines, sinon en passe a la suivante (meme si en theorie il ne devrait pas y avoir de chaines sans points)
            chaine = chaine->suiv;
            continue;
        }
        noeudPrec = rechercheCreeNoeudListe(R, point->x, point->y);
        premier_noeud = noeudPrec;
        if (!noeudPrec)
        {
            printf("Annulation de la reconstitution\n");
            free(matrice);
            libererReseau(&R);
            return NULL;
        }
        if (!(point = point->suiv))
        { // Le cas ou la chaines n'est consitué que d'un point
            printf("La chaine %d n'est consituee que d'un point\n", chaine->numero);
            if (!(*P_commodites = creerCommodite(noeudPrec, noeudPrec)))
            { // on creer quand meme la commodité
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                free(matrice);
                return NULL;
            }
            P_commodites = &(*P_commodites)->suiv;
            chaine = chaine->suiv;
            continue;
        }
        while (point)
        {
            noeud = rechercheCreeNoeudListe(R, point->x, point->y);
            if (!rechercheVoisinAjoute2(matrice, noeud, noeudPrec, taille))
            {
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                free(matrice);
                return NULL;
            }
            noeudPrec = noeud;
            point = point->suiv;
        } // apres avoir parcouru tout les points de la chaine, on creer la comodité
        if (!(*P_commodites = creerCommodite(premier_noeud, noeud)))
        {
            printf("Annulation de la reconstitution\n");
            free(matrice);
            libererReseau(&R);
            return NULL;
        }
        P_commodites = &(*P_commodites)->suiv;
        chaine = chaine->suiv;
    }
    if (chaine)
    { // si chaine n'est pas NULL c'est que nbChaines est inférieur au nombre de  chaine de chaines existants, donc  on rapport l'erreur
        fprintf(stderr, "Erreur :  Il y a plus de chaine que le nombre indique\n");
        printf("Annulation de la reconstitution\n");
        free(matrice);
        libererReseau(&R);
        return NULL;
    }
    free(matrice);
    return R;
}
