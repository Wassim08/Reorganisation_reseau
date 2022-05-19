#include "../headers/Reseau.h"
#include "../headers/Hachage.h"

//=============================================================
int libererNoeudH(NoeudH *ndh)
{
    // Libere l'espace memoire occupé par un Noeuds ndh ainsi sa chaine de suivant et sa table de voisin
    //  Attention, libererNoeudH ne libere pas l'attribut nd car il doit etre liberer par la fonction libererReseau
    if (!ndh)
        return 0;
    if (ndh->voisins){
        libererTableHachage(ndh->voisins);
    }

    int nb_elem = 1 + libererNoeudH(ndh->suivant);

    free(ndh);

    return nb_elem;
}
//=============================================================
void libererTableHachage(TableHachage *T)
{
    //  Libere  l'espace memoire occupé par une Table de Hachage T ainsi que son tableau de NoeudH tab
    //  Attention, libererTableHachage  doit etre utiliser conjointement avec libererReseau afin de liberer l'attribut nd des NoeudH
    for (int i = 0; i < T->taille; i++)
    {
        if ((T->nE -= libererNoeudH(T->tab[i])) == 0)
            break;
        
    }
    free(T->tab);
    free(T);
}
//=============================================================
int fonction_clef(double x, double y)
{
    // retourne la clef pour les coordonnee 'x' et 'y' d'un point
    return (int)(y + (x + y) * (x + y + 1) / 2);
}
//=============================================================

int fonction_hachage(TableHachage *tab, int k)
{
    // Retourne l'indice dans une table de hachage 'tab' d'une clef donne 'k'
    int M = tab->taille;
    return floor(M * (k * A - floor(k * A)));
}
NoeudH *CreerStructNoeudh(Noeud *nd)
{
    // Creer et retourne un NoeudH correpondant  avec le meme numero que le noeud 'nd' 
    NoeudH *ndh = (NoeudH *)malloc(sizeof(NoeudH));
    if (!ndh)
    {
        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour un NoeudH\n");
        return NULL;
    }
    ndh->nd = nd;
    ndh->voisins = NULL;
    ndh->suivant = NULL;
    return ndh;
}
//=============================================================
int insererNoeudTable(TableHachage *H, Noeud *nd, int pos)
{
    //Insere un NoeudH dans la table de hachage 'H' a la position 'pos' correspondant au noeud 'nd' et incremente le nombre d'element de table
    // Retourne 1 si l'insertion est reussi et 0 sinon
    if (H == NULL)
    {
        fprintf(stderr, "Erreur : La Table de hachage n'est pas defini\n");
        return 0;
    }
    // on creer le noeudh a inserer
    NoeudH *ndh = CreerStructNoeudh(nd);
    if (!ndh)
        return 0;
    // on verifie que pos est bon
    if (pos >= H->taille)
    {
        fprintf(stderr, "Erreur : La table de hachage n'est pas assez grande\n");
        free(ndh);
        return 0;
    }
    // on insere le noeudh dans la table
    // si la case est vide on insere directement
    if (H->tab[pos] == NULL)
        H->tab[pos] = ndh;
    else
    {// si la case n'est pas vide on fait un insertion en tete
        ndh->suivant = H->tab[pos];
        H->tab[pos] = ndh;
    }
    H->nE++; // on incremente le nombre d'elements
    return 1;
}
//=============================================================
Noeud *rechercheNoeudH(TableHachage *H, double x, double y, int *pos)
{
    // Recherche le noeud de coordonné 'x', 'y' dans la table 'H' et le met en tete de sa chaine
    // rend le NoeudH coorespondant si le noeud est dans la table  et NULL sinon
    // si un probleme est rencontre l'entier pointé par pos seras negatif

   
    // On recherche la position ou devrait etre le noeud dans la table
    int clef = fonction_clef(x, y);
    *pos = fonction_hachage(H, clef);

  
    NoeudH **H_Lnoeud = &(H->tab[*pos]); // on prend un pointeur de pointeur au cas ou on doit ajouté un noeuds

    while (*H_Lnoeud)
    { // on verifie pour chaque noeud chainé a cette position si les coordonnées correspondes
        if ((*H_Lnoeud)->nd->x == x && (*H_Lnoeud)->nd->y == y)
        {
            NoeudH *temp = *H_Lnoeud;
            if (H->tab[*pos] == temp) // Cas ou le noeud est deja en tete
                return temp->nd;
            // on le met en tete si il ne l'est pas
            *H_Lnoeud = temp->suivant;
            temp->suivant = H->tab[*pos];
            H->tab[*pos] = temp;
            return temp->nd;
        }
        H_Lnoeud = &((*H_Lnoeud)->suivant);
    }
    //si aucun noeud correspondant n'est trouvé, on rend NULL
    return NULL;
}
//=============================================================
Noeud *rechercheCreeNoeudHachage(Reseau *R, TableHachage *H, double x, double y)
{
    /*Retourne un Noeud du reseau 'R' correspondant au point ('x', 'y') dans la
        table de hachage 'H' 
        retourne NULL en cas d'erreur*/
  
    // On initalise pos a une valeur negative
    int pos = -1;

    Noeud *noeud = rechercheNoeudH(H, x, y, &pos); // on cherche un noeud correspondant aux coordonnées dans la table

    if (pos < 0) // Si pos est negatif c'est qu'il y a eu une erreur
        return NULL;
    if (noeud) // Si un noeud correspond, on peut le retourner
        return noeud;

    // si aucun noeud n'a été trouvée on doit le creer
    noeud = creerNoeud(++R->nbNoeuds, x, y);
    if (!noeud) // on s'assure que la fonction a bien fonctionnée
        return NULL;
    // on creer le CellNoeud pour le reseau
    CellNoeud *Cellnoeud_Reseau = creerCellNoeud_nd(noeud);
    if (!Cellnoeud_Reseau){ // on s'assure que la fonction a bien fonctionnée
        free(noeud);
        return NULL;
    }

    // Puis on insere le noeud dans la table et dans le Reseau
    if (!insererNoeudTable(H, noeud, pos)){
        free(noeud);
        free(Cellnoeud_Reseau);
        return NULL;
    }
    ajouter_cellnoeud(R, Cellnoeud_Reseau);

    return noeud;
}
//=============================================================
TableHachage *creerTableHachage(int taille)
{
    // Retourne un pointeur vers une table de hachage de taille 'taille'

    // on alloue l'espace necessaire pour la table
    TableHachage *H = (TableHachage *)malloc(sizeof(TableHachage));
   
    H->taille = taille;
    // on alloue l'espace necessaire pour le tableau
    NoeudH **tab = (NoeudH **)calloc(sizeof(NoeudH *), taille);
    if (!tab)
    {
        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour la table hachage 2\n");
        free(H);
        return NULL;
    }
    // on place le tableau dans la table
    H->tab = tab;
    H->nE=0;
    return H;
}
//=============================================================
int rechercheNoeudH_nd(TableHachage *H, Noeud *nd, int *pos)
{
    // Recherche le noeud 'nd' dans la table 'H'
    // rend 1 si le noeud est dans la table  et 0 sinon
    // si un probleme est rencontre l'entier pointé par pos seras negatif

    // On recherche la position ou devrait etre le noeud dans la table
    int clef = fonction_clef(nd->x, nd->y);
    *pos = fonction_hachage(H, clef);

   
    NoeudH *noeudH = H->tab[*pos];
    while (noeudH)
    { // on verifie pour chaque noeud chainé a cette position si il y en a un qui correspond à 'nd'
        if (nd == noeudH->nd)
            return 1;

        noeudH = noeudH->suivant;
    }
    //si aucun noeud correspondant n'est trouvé, on rend 0
    return 0;
}
//=============================================================
int rechercheVoisinAjouteHachage(NoeudH *noeudH, NoeudH *noeudHPrec, int m)
{
    /* Verifies si 'noeud' et 'noeudPrec' ne sont pas deja dans leurs listes de voisins l'un de l'autre
        si ils n'y sont pas les listes de voisins  sont mis a jour'
        si la table des voisins d'un des noeuds n'a pas ete creer, elle est creer et 'm' sera sa taille
        Retourne 0 en cas d'erreur et 1 sinon*/
    if (noeudH == NULL || noeudHPrec == NULL || m == 0)
        return 0;
    if (noeudH == noeudHPrec) // on verifie que les 2 noeud ne soit pas les meme
        return 1;
    int clef;
    int pos = -1;
    
    // on verifie si 'noeudH' a une table de ses voisins, sinon on la creer
    if (noeudH->voisins == NULL)
    {
        if (!(noeudH->voisins = creerTableHachage(m)))
            return 0;
        if (noeudHPrec->voisins == NULL)
        {
            if (!(noeudHPrec->voisins = creerTableHachage(m)))
                return 0;
        }
    }
    else if (noeudHPrec->voisins == NULL)
    {
        if (!(noeudHPrec->voisins = creerTableHachage(m)))
            return 0;
    }
    else if (noeudH->voisins->nE < noeudHPrec->voisins->nE) // on fait la recherche dans la table qui a le moins d'élements
    { 
        if (rechercheNoeudH_nd(noeudH->voisins, noeudHPrec->nd, &pos)) 
            return 1; // si noeudHPrec  est deja dans les voisin de noeudH on retourne 1
        if (pos < 0)
            return 0;
    }
    else
    {
        if (rechercheNoeudH_nd(noeudHPrec->voisins, noeudH->nd, &pos))
            return 1;// si noeudH  est deja dans les voisin de noeudHPrec  on retourne 1

        if (pos < 0)
            return 0;
    
    }
    

    // on a pas trouve les noeuds dans la table de leurs voisin, donc on les insere dans les tables l'un de l'autre
    clef = fonction_clef(noeudH->nd->x, noeudH->nd->y);
    pos = fonction_hachage(noeudHPrec->voisins, clef);
    insererNoeudTable(noeudHPrec->voisins, noeudH->nd, pos);
    clef = fonction_clef(noeudHPrec->nd->x, noeudHPrec->nd->y);
    pos = fonction_hachage(noeudH->voisins, clef);
    insererNoeudTable(noeudH->voisins, noeudHPrec->nd, pos);

    // puis on insere les noeuds dans leurs listes de voisin du Reseau
    CellNoeud *cell = creerCellNoeud_nd(noeudHPrec->nd);
    if (!cell)
        return 0;
    cell->suiv = noeudH->nd->voisins;
    noeudH->nd->voisins = cell;

    if (!(cell = creerCellNoeud_nd(noeudH->nd)))
        return 0;

    cell->suiv = noeudHPrec->nd->voisins;
    noeudHPrec->nd->voisins = cell;

    return 1;
}
//=============================================================
Reseau *reconstitueReseauHachage(Chaines *C, int M)
{
    /*Reconstruit et retourne un reseau a partir de la liste des chaınes C et en utilisant une table de hachage de
    taille M.*/
   
    // si la taille est trop grande on met 'M' a 'MAX_M'
    if (M>MAX_M) M = MAX_M;
    
    // 'm' est la taille de la table de hachage des voisins  des noeuds
    int m =(M>15 ? M/4:3);
    Reseau *R = (Reseau *)calloc(1, sizeof(Reseau));
    if (!R)
   
    // on creer la table de hachage
    TableHachage *H = creerTableHachage(M);
   
    R->gamma = C->gamma;
    int clef;
    int pos;
    CellChaine *chaine = C->chaines;
    CellPoint *point;
    NoeudH *noeudHPrec;                              //pointeur vers le noeud precedent dans la table, il va etre utiliser pour verifier si le noeud etudié apres est deja dans sa liste de vosin
    CellCommodite **P_commodites = &(R->commodites); // Pointeur vers l'endroit ou doit etre ajouter la prochaine commodité
    Noeud *noeud;                                    // Pointeur vers le noeud analysé couramment
    NoeudH *noeudH;                                  // Pointeur vers le noeud dans la table analysé couramment
    Noeud *premier_noeud;                            // Pointeur vers le premier de la chaine, il va etre utilisé pour creer la commodite
    // on parcourt les chaines
    for (int x = 0; x<C->nbChaines; x++){// on fait un for sur nbChaines au lieu de faire un while(chaine) pour s'assurer que le nombre de chaines indiqué correspond bien au nombre de chaines existante
        
        if (!chaine){// si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on arrete la boucle
            fprintf(stderr,"Erreur :  %d chaine(s) trouvee(s) au lieu de %d\n",x,C->nbChaines);
            printf("Annulation de la reconstitution\n");
            libererTableHachage(H);
            libererReseau(&R);
            return NULL;
        }
        if (!(point = chaine->points))
        { // on verifie si il y a bien des points dans la chaines, sinon en passe a la suivante (meme si en theorie il ne devrait pas y avoir de chaines sans points)
            chaine = chaine->suiv;
            continue;
        }
        // on cherche le premier noeud de la chaine
        premier_noeud = rechercheCreeNoeudHachage(R, H, point->x, point->y);
        if (!premier_noeud)
       
       
        if (!(point = point->suiv)) // Le cas ou la chaines n'est consitué que d'un point
        { 
            printf("La chaine %d n'est consituee que d'un point\n", chaine->numero);
            if (!(*P_commodites = creerCommodite(premier_noeud, premier_noeud)))
            { // on creer quand meme la commodité
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                libererTableHachage(H);
                return NULL;
            }
            P_commodites = &(*P_commodites)->suiv;
            chaine = chaine->suiv;
            continue;
        }
        
        
        clef = fonction_clef(premier_noeud->x, premier_noeud->y);
        pos = fonction_hachage(H, clef);
        noeudHPrec = H->tab[pos];//on sait que il est en tete grace a la fonction a la fonction (rechercheNoeudH) utilisé dans (rechercheCreeNoeudHachage)
        // on parcourt les points de la chaine
        
        
        while (point)
        {
            // on recupere le noeuds correspondant au point
            if ((noeud = rechercheCreeNoeudHachage(R, H, point->x, point->y)))
            {
                clef = fonction_clef(noeud->x, noeud->y);
                pos = fonction_hachage(H, clef);
                noeudH = H->tab[pos];
            }
            // on met a jour les voisins
            if (rechercheVoisinAjouteHachage(noeudH, noeudHPrec, m) == 0)
            {
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                libererTableHachage(H);
                return NULL;
            }
            noeudHPrec = noeudH;
            point = point->suiv;
        } // apres avoir parcouru tout les points de la chaine, on creer la comodité
        if (!(*P_commodites = creerCommodite(premier_noeud, noeud)))
        {
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            libererTableHachage(H);
            return NULL;
        }
        P_commodites = &(*P_commodites)->suiv;
        chaine = chaine->suiv;
    }
    
    
    
    if(H->nE!=R->nbNoeuds){// Verification pour s'assurer que tout s'est bien passé, on doit avoir autant de noeuds qu'il y a d'element dans la table
        fprintf(stderr,"Erreur : Le nombre de noeuds est different du nombre d'elements dans la table\n");
        printf("Annulation de la reconstitution\n");
        libererReseau(&R);
        libererTableHachage(H);
        return NULL;
    }
    libererTableHachage(H); // on a plus besoin de la table donc on la libere
    if (chaine){// si chaine n'est pas NULL c'est que nbChaines est inférieur au nombre de  chaine de chaines existants, donc  on rapport l'erreur 
        fprintf(stderr,"Erreur :  Il y a plus de chaine que le nombre indique\n");
        printf("Annulation de la reconstitution\n");
        libererReseau(&R);
        return NULL;
    }
    

    return R;
}
//=============================================================
//Exo 4 q 2 : Test de la fonction fonction_clef avec plusieurs valeurs entre 1 et 10 
void test_fonction_clef(){
	int clef=0 ;
	for(int i=1;i<=10;i++){
		for(int j=1;j<=10;j++){
			clef=fonction_clef(i,j);
			printf("(%d,%d)---->%d",i,j,clef);
		}
		printf("\n");
	}

}


Reseau *reconstitueReseauHachage2(Chaines *C, int M)
{
    /*Reconstruit et retourne un reseau a partir de la liste des chaınes C et en utilisant une table de hachage de
    taille M.*/
    if (!M){
        fprintf(stderr, "Erreur:La taille de la table de Hachage ne peut pas etre nulle\n");
        printf("Reconstitution abandonnee\n");
        return NULL;
    }
    // si la taille est trop grande on met 'M' a 'MAX_M'
    if (M>MAX_M) M = MAX_M;
    
    // 'm' est la taille de la table de hachage des voisins  des noeuds
    int m =(M>15 ? M/4:3);
    Reseau *R = (Reseau *)calloc(1, sizeof(Reseau));
    if (!R)
    {
        fprintf(stderr, "Erreur lors de l'allocation de l'espace pour un Reseau\n");
        printf("Reconstitution abandonnee\n");
        return NULL;
    }
    // on creer la table de hachage
    TableHachage *H = creerTableHachage(M);
    if (!H){
        printf("Reconstitution abandonnee\n");
        free(R);
        return NULL;
    }
    R->gamma = C->gamma;
    int clef;
    int pos;
    CellChaine *chaine = C->chaines;
    CellPoint *point;
    NoeudH *noeudHPrec;                              //pointeur vers le noeud precedent dans la table, il va etre utiliser pour verifier si le noeud etudié apres est deja dans sa liste de vosin
    CellCommodite **P_commodites = &(R->commodites); // Pointeur vers l'endroit ou doit etre ajouter la prochaine commodité
    Noeud *noeud;                                    // Pointeur vers le noeud analysé couramment
    NoeudH *noeudH;                                  // Pointeur vers le noeud dans la table analysé couramment
    Noeud *premier_noeud;                            // Pointeur vers le premier de la chaine, il va etre utilisé pour creer la commodite
    // on parcourt les chaines
    for (int x = 0; x<C->nbChaines; x++){// on fait un for sur nbChaines au lieu de faire un while(chaine) pour s'assurer que le nombre de chaines indiqué correspond bien au nombre de chaines existante
        
        if (!chaine){// si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on arrete la boucle
            fprintf(stderr,"Erreur :  %d chaine(s) trouvee(s) au lieu de %d\n",x,C->nbChaines);
            printf("Annulation de la reconstitution\n");
            libererTableHachage(H);
            libererReseau(&R);
            return NULL;
        }
        if (!(point = chaine->points))
        { // on verifie si il y a bien des points dans la chaines, sinon en passe a la suivante (meme si en theorie il ne devrait pas y avoir de chaines sans points)
            chaine = chaine->suiv;
            continue;
        }
        // on cherche le premier noeud de la chaine
        premier_noeud = rechercheCreeNoeudHachage(R, H, point->x, point->y);
        if (!premier_noeud)
        {
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            libererTableHachage(H);
            return NULL;
        }
        if (!(point = point->suiv)) // Le cas ou la chaines n'est consitué que d'un point
        { 
            printf("La chaine %d n'est consituee que d'un point\n", chaine->numero);
            if (!(*P_commodites = creerCommodite(premier_noeud, premier_noeud)))
            { // on creer quand meme la commodité
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                libererTableHachage(H);
                return NULL;
            }
            P_commodites = &(*P_commodites)->suiv;
            chaine = chaine->suiv;
            continue;
        }
        clef = fonction_clef(premier_noeud->x, premier_noeud->y);
        pos = fonction_hachage(H, clef);
        noeudHPrec = H->tab[pos];//on sait que il est en tete grace a la fonction a la fonction (rechercheNoeudH) utilisé dans (rechercheCreeNoeudHachage)
        // on parcourt les points de la chaine
        while (point)
        {
            // on recupere le noeuds correspondant au point
            if ((noeud = rechercheCreeNoeudHachage(R, H, point->x, point->y)))
            {
                clef = fonction_clef(noeud->x, noeud->y);
                pos = fonction_hachage(H, clef);
                noeudH = H->tab[pos];
            }
            // on met a jour les voisins
            if (rechercheVoisinAjouteHachage(noeudH, noeudHPrec, m) == 0)
            {
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                libererTableHachage(H);
                return NULL;
            }
            noeudHPrec = noeudH;
            point = point->suiv;
        } // apres avoir parcouru tout les points de la chaine, on creer la comodité
        if (!(*P_commodites = creerCommodite(premier_noeud, noeud)))
        {
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            libererTableHachage(H);
            return NULL;
        }
        P_commodites = &(*P_commodites)->suiv;
        chaine = chaine->suiv;
    }
    
    if(H->nE!=R->nbNoeuds){// Verification pour s'assurer que tout s'est bien passé, on doit avoir autant de noeuds qu'il y a d'element dans la table
        fprintf(stderr,"Erreur : Le nombre de noeuds est different du nombre d'elements dans la table\n");
        printf("Annulation de la reconstitution\n");
        libererReseau(&R);
        libererTableHachage(H);
        return NULL;
    }
    libererTableHachage(H); // on a plus besoin de la table donc on la libere
    if (chaine){// si chaine n'est pas NULL c'est que nbChaines est inférieur au nombre de  chaine de chaines existants, donc  on rapport l'erreur 
        fprintf(stderr,"Erreur :  Il y a plus de chaine que le nombre indique\n");
        printf("Annulation de la reconstitution\n");
        libererReseau(&R);
        return NULL;
    }
    

    return R;
}
