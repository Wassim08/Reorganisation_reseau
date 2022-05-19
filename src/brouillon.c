# include "../headers/Reseau.h"
# include "../headers/SVGwriter.h"

void libererMatrice(char **matrice,int nblignes){
    // libere une mitrice 2D de 'nblignes' lignes et 'nbcollonnes' colonnes
    for (int i = 0; i<nblignes;i++){
            free(matrice[i]);
    }
}

void libererCommodites(CellCommodite * CellCommod){
    CellCommodite * suiv;
    while(CellCommod){
        suiv = CellCommod->suiv;
        free(CellCommod);
        CellCommod = suiv;
    }
}
void libererNoeud(Noeud* noeud){
    CellNoeud* voisin = noeud->voisins;
    while(voisin){
        noeud->voisins=voisin->suiv;
        free(voisin);
        voisin=noeud->voisins;
    }
    free (noeud);
}
void libererCellNoeuds(CellNoeud* noeuds){
    CellNoeud* suiv;
    while(noeuds){
        libererNoeud( noeuds->nd) ;
        suiv = noeuds->suiv;
        free(noeuds);
        noeuds = suiv;
    }
}
void libererReseau(Reseau** R){
    if (!(*R))  return;
    libererCellNoeuds((*R)->noeuds);
    libererCommodites((*R)->commodites);
    free((*R));
    *R = NULL;
}
Noeud* creerNoeud( int num , double x, double y  ){
    Noeud* nd = (Noeud*) calloc(1,sizeof(Noeud));
    if (!nd){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour un Noeud\n");
        return NULL;
    }
    nd->num = num ; nd->x = x ; nd->y = y;
    return nd;
}

CellNoeud* creerCellNoeud(int num ,double x, double y){
    CellNoeud* cell= (CellNoeud*) calloc(1,sizeof(CellNoeud));
    if (!cell){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour une CellNoeud\n");
        return NULL;
    }
    if (!(cell->nd = creerNoeud(  num , x,  y ))){
        free( cell);
        return NULL;
    }
    return cell;
}
CellNoeud* creerCellNoeud_nd(Noeud * nd){
    // creer un CellNoeud depuis un noeud
    CellNoeud* cell= (CellNoeud*) calloc(1,sizeof(CellNoeud));
    if (!cell){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour un CellNoeud\n");
        return NULL;
    }
    cell->nd=nd;
    return cell;


}

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    if (!R){
        fprintf(stderr,"Erreur : Le Reseau n'est n'est pas defini\n");
        return NULL;
    }
    CellNoeud ** chaineNoeuds = &(R->noeuds); // on prend un pointeur de pointeur au cas ou on doit ajouté un noeuds
    Noeud * nd;
    int i = 1;// cette variable sert simplement au cas ou on aurait un noeud qui n'est pas defini, sa position pourra etre indiquee dans le message d'erreur
    while(*chaineNoeuds){
        if (!( nd = (*chaineNoeuds)->nd)){
           fprintf(stderr,"Erreur : Le Noeud %d du reseau n'est n'est pas defini\n",i); 
           chaineNoeuds = &(*chaineNoeuds)->suiv; // le fait d'avoir un noeud non defini est anormale mais on peut quand poursuivre la recherche
           continue;
        }
        if ( (nd->x==x) && (nd->y==y) ) // on regarde la correspondance avec les coordonnees donnee en argument.
            return nd;

        chaineNoeuds = &(*chaineNoeuds)->suiv; // si on a pas trouvé on passe au suivant
        i++;


    }
    //si on arrive a la fin du while c'est que la recherche n'a pas permis d'identifier le noeud dans la liste , donc on doit le creer
    if (!(*chaineNoeuds = creerCellNoeud( R->nbNoeuds+1 , x,  y ))){
        return NULL;
    }

    R->nbNoeuds++;// si ca a marche on incremente le nombre de noeud, puis on rend le nouveau noeud
    return (*chaineNoeuds)->nd;
    

}
CellCommodite * creerCommodite(Noeud * extrA , Noeud * extrB){
    CellCommodite * commodite = (CellCommodite*) malloc(sizeof(CellCommodite));
    if (!commodite){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour une commodite\n");
        return NULL;
    }
    commodite->extrA= extrA;
    commodite->extrB= extrB;
    commodite->suiv=NULL;
    return commodite;
}

CellNoeud ** rechercheVoisinAjoute(Noeud * noeud ,Noeud * noeudPrec,CellNoeud ** P_insertionNoeudVoisin){
    if (noeud==NULL || noeudPrec== NULL || P_insertionNoeudVoisin== NULL)
        return NULL;
    if (noeud == noeudPrec)// on verifie que les 2 noeud ne soit pas les meme
        return P_insertionNoeudVoisin;

    CellNoeud ** noeudCompare = &(noeud->voisins);
    while (*noeudCompare){
        if ((*noeudCompare)->nd == noeudPrec)
            return &(*noeudCompare)->suiv;
        
        noeudCompare = &(*noeudCompare)->suiv;
    }// si on arrive a la fin du while c'est que la relation de voisinage n'est pas encore creer donc on la fait
    CellNoeud* cell = creerCellNoeud_nd ( noeudPrec );
    if (! cell) return NULL;
    *noeudCompare = cell;
    noeudCompare = &cell->suiv;
    if (!(cell = creerCellNoeud_nd ( noeud ))) return NULL;
    if (*P_insertionNoeudVoisin){// si il y a deja un noeud a l'emplacement on ne peut pas inserer directement
        cell->suiv= *P_insertionNoeudVoisin;
        *P_insertionNoeudVoisin = cell;
        return noeudCompare;
    }
    *P_insertionNoeudVoisin = cell;//si il y a pas de noeud on insere directement
    return noeudCompare;
}


int ajoutevoisint(Noeud * noeud1 ,Noeud * noeud2){
    /*Insere en tete 'noeud1' dans la liste des voisins de 'noeud2' et insere 'noeud2' en tete dans la liste des voisins de 'noeud1'
    La fonction retourne 0 si un erreur s'est produite et 1 sinon*/
    
    // On creer le CellNoeud
    CellNoeud* cell = creerCellNoeud_nd ( noeud1 );
    if(!cell) return 0;

    // puis on l'insere
    cell->suiv = noeud2->voisins;
    noeud2->voisins = cell;
    
    // on refait pareil en échangant noeud1 et noeud2
    if (!(cell = creerCellNoeud_nd ( noeud2 ))) return 0;

    cell->suiv = noeud1->voisins;
    noeud1->voisins = cell;
    return 1;

}
int rechercheVoisinAjoute3(char * matrice,Noeud * noeud1 ,Noeud * noeud2,int nbColonnes){
    /* verifies si 'noeud' et 'noeud2' ne sont pas deja dans leurs listes de voisins l'un de l'autre a l'aide de la matrice sommet-sommet 'matrice'
    si ils n'y sont pas les listes de voisins ainsi que 'matrice' sont mis a jour'
    'nbColonnes' pointe vers le nombre de colonne de 'matrice' qui peut etre agrandi en cas de besoin
    La fonction retourne 0 si un erreur s'est produite et 1 sinon*/

    if (noeud1==NULL || noeud2== NULL || matrice == NULL || nbColonnes == 0) return 0;

    // on recupere les numeros des noeuds
    int num1 = noeud1->num;
    int num2 = noeud2->num;

    // on recupere le minimum pour s'assurer qu'il n'est pas negatif
    int minMax = min(num1,num2);
    
    if (minMax<0){
        fprintf(stderr,"Le numeros des noeuds doit etre positif\n");
        return 0;
    }
    // on prend le max pour verifier que la taille de la matrice est bonne
    minMax  = max(num1,num2);
    
    if ( minMax > nbColonnes){ printf("\n\n\n ooooo \n \n \n"); return 0;}
    
    int index1 = getIndex(num1,num2,nbColonnes);

    // printf("index = %d;\tnum1 = %d,\tnum2 = %d\n",index1,num1,num2);
    if (matrice[index1]) return 1;

    int index2 = getIndex(num2,num1,nbColonnes);

    matrice[index1] = 'o';
    matrice[index2] = 'o';

    return ajoutevoisint(noeud1,noeud2);



}


Reseau* reconstitueReseauListe(Chaines *C){

    if (!C){// Si C n'est pas defini on arrete
        fprintf(stderr,"Erreur : L'Ensemble de chaines n'est pas defini\n");
        printf("La reconstitution est abandonnee\n");
        return NULL;
    }
    Reseau* R =(Reseau*) calloc(1, sizeof(Reseau));
    
    if (!R){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour un Reseau\n");
        printf("Reconstitution abandonnee\n");
        return NULL;
    }
    R->gamma = C->gamma;
    CellChaine * chaine = C->chaines;
    CellPoint * point ;
    Noeud*  noeudPrec; //pointeur vers le noeud precedent, il va etre utiliser pour verifier si le noeud etudié apres est deja dans sa liste de voisin
    CellCommodite ** P_commodites = &(R->commodites); // Pointeur vers l'endroit ou doit etre ajouter la prochaine commodité
    Noeud * noeud;// Pointeur vers le noeud analysé couramment
    Noeud * premier_noeud;// Pointeur vers le premier de la chaine, il va etre utilisé pour creer la commodite
    CellNoeud ** P_insertionNoeudVoisin; // pointeur vers l'endroit ou sera inserer le noeud dans la liste des noeuds voisins du noeuds precedent
    for (int x = 0; x<C->nbChaines; x++){// on fait un for sur nbChaines au lieu de faire un while(chaine) pour s'assurer que le nombre de chaines indiqué correspond bien au nombre de chaines existante
        
        if (!chaine){// si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on arrete la boucle
            fprintf(stderr,"Erreur :  %d chaine(s) trouvee(s) au lieu de %d\n",x,C->nbChaines);
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        if ( ! ( point = chaine->points) ){// on verifie si il y a bien des points dans la chaines, sinon en passe a la suivante (meme si en theorie il ne devrait pas y avoir de chaines sans points)
            chaine = chaine->suiv;
            continue;
        }
        P_insertionNoeudVoisin=NULL;
        noeudPrec = rechercheCreeNoeudListe(R, point->x, point->y);
        premier_noeud = noeudPrec;
        if(!noeudPrec){
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        if (!(point = point->suiv)){// Le cas ou la chaines n'est consitué que d'un point
            printf("La chaine %d n'est consituee que d'un point\n",chaine->numero);
            if (!(*P_commodites = creerCommodite(noeudPrec,noeudPrec))){// on creer quand meme la commodité
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                return NULL;
            }
            P_commodites= &(*P_commodites)->suiv;
            chaine = chaine->suiv;
            continue;

        }
        P_insertionNoeudVoisin = &noeudPrec->voisins;
        while( point ){
            noeud =rechercheCreeNoeudListe(R,point->x,point->y);
            P_insertionNoeudVoisin = rechercheVoisinAjoute(noeud ,noeudPrec,  P_insertionNoeudVoisin);
            if (!P_insertionNoeudVoisin){
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                return NULL;
            }
            noeudPrec = noeud;
            point = point->suiv;
        }// apres avoir parcouru tout les points de la chaine, on creer la comodité
        if (!(*P_commodites = creerCommodite(premier_noeud,noeud))){
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        P_commodites= &(*P_commodites)->suiv;
        chaine = chaine->suiv;

    }
    if (chaine){// si chaine n'est pas NULL c'est que nbChaines est inférieur au nombre de  chaine de chaines existants, donc  on rapport l'erreur 
        fprintf(stderr,"Erreur :  Il y a plus de chaine que le nombre indique\n");
        printf("Annulation de la reconstitution\n");
        libererReseau(&R);
        return NULL;
    }
    return R;
}

int rechercheVoisinAjoute2(char *** matrice,Noeud * noeud1 ,Noeud * noeud2,int* nbColonnes){
    /* verifies si 'noeud' et 'noeud2' ne sont pas deja dans leurs listes de voisins l'un de l'autre a l'aide de la matrice sommet-sommet 'matrice'
    si ils n'y sont pas les listes de voisins ainsi que 'matrice' sont mis a jour'
    'nbColonnes' pointe vers le nombre de colonne de 'matrice' qui peut etre agrandi en cas de besoin
    La fonction retourne 0 si un erreur s'est produite et 1 sinon*/

    if (noeud1==NULL || noeud2== NULL || matrice == NULL || nbColonnes == NULL) return 0;

    // on recupere les numeros des noeuds
    int num1 = noeud1->num;
    int num2 = noeud2->num;

    // on recupere le minimum pour s'assurer qu'il n'est pas negatif
    int minMax = min(num1,num2);
    
    if (minMax<0){
        fprintf(stderr,"Le numeros des noeuds doit etre positif\n");
        return 0;
    }
    // on prend le max pour verifier que la taille de la matrice est bonne
    minMax  = max(num1,num2);
    
    if ( minMax >= *nbColonnes){// on agrandi la matrice si elle n'est pas assez grande
        char ** temp = realloc(*matrice,(minMax + 1)*sizeof(char*));
        if (!temp){
            fprintf(stderr,"Impossible d'allouer assez d'espace pour agrandire la matrice sommet-sommet 1\n");
            libererMatrice(*matrice,*nbColonnes);
            return 0;
        }
        *matrice = temp;
        char * temp2;

        for (int i=0; i< *nbColonnes ;i++){
            
            if (!(temp2 = realloc(temp[i],(minMax + 1)*sizeof(char)))){
                fprintf(stderr,"Impossible d'allouer assez d'espace pour agrandire la matrice sommet-sommet 2\n");
                libererMatrice(*matrice,*nbColonnes);
                free(*matrice);
                return 0;

            }
            temp[i] = temp2;
            //  on inialise les nouvelles cases
            for (int j=*nbColonnes; j<=minMax ; j++){
                (*matrice)[i][j] = '\0';
            }
        }
        for (int i=*nbColonnes; i <=minMax   ;i++){
            
            if (!(temp[i] = calloc((minMax + 1) , sizeof(char)))){
                fprintf(stderr,"Impossible d'allouer assez d'espace pour agrandire la matrice sommet-sommet\n");
                libererMatrice(*matrice,i);
                free(*matrice);
                return 0;

            }

            //  on inialise les nouvelles cases
            for (int j=*nbColonnes; j<=minMax ; j++){
                (*matrice)[i][j] = '\0';
            }
        }
        // on met a jour nbColonnes et la matrice
        *nbColonnes = minMax+1;
        

    }
    
    if ((*matrice)[num1][num2]) return 1;

    (*matrice)[num1][num2] = 'o';
    (*matrice)[num1][num2] = 'o';

    return ajoutevoisint(noeud1,noeud2);








}

Reseau* reconstitueReseauListe2(Chaines *C){

    if (!C){// Si C n'est pas defini on arrete
        fprintf(stderr,"Erreur : L'Ensemble de chaines n'est pas defini\n");
        printf("La reconstitution est abandonnee\n");
        return NULL;
    }
    Reseau* R =(Reseau*) calloc(1, sizeof(Reseau));
    
    if (!R){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour un Reseau\n");
        printf("Reconstitution abandonnee\n");
        return NULL;
    }
    R->gamma = C->gamma;
    int taille = C->nbChaines*10;
    char **matrice = calloc(taille,sizeof(char*));
    for (int i=0; i < taille ; i++){
        matrice[i] = calloc(taille,sizeof(char));
    }
    CellChaine * chaine = C->chaines;
    CellPoint * point ;
    Noeud*  noeudPrec; //pointeur vers le noeud precedent, il va etre utiliser pour verifier si le noeud etudié apres est deja dans sa liste de voisin
    CellCommodite ** P_commodites = &(R->commodites); // Pointeur vers l'endroit ou doit etre ajouter la prochaine commodité
    Noeud * noeud;// Pointeur vers le noeud analysé couramment
    Noeud * premier_noeud;// Pointeur vers le premier de la chaine, il va etre utilisé pour creer la commodite
    CellNoeud ** P_insertionNoeudVoisin; // pointeur vers l'endroit ou sera inserer le noeud dans la liste des noeuds voisins du noeuds precedent
    for (int x = 0; x<C->nbChaines; x++){// on fait un for sur nbChaines au lieu de faire un while(chaine) pour s'assurer que le nombre de chaines indiqué correspond bien au nombre de chaines existante
        
        if (!chaine){// si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on arrete la boucle
            fprintf(stderr,"Erreur :  %d chaine(s) trouvee(s) au lieu de %d\n",x,C->nbChaines);
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        if ( ! ( point = chaine->points) ){// on verifie si il y a bien des points dans la chaines, sinon en passe a la suivante (meme si en theorie il ne devrait pas y avoir de chaines sans points)
            chaine = chaine->suiv;
            continue;
        }
        P_insertionNoeudVoisin=NULL;
        noeudPrec = rechercheCreeNoeudListe(R, point->x, point->y);
        premier_noeud = noeudPrec;
        if(!noeudPrec){
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        if (!(point = point->suiv)){// Le cas ou la chaines n'est consitué que d'un point
            printf("La chaine %d n'est consituee que d'un point\n",chaine->numero);
            if (!(*P_commodites = creerCommodite(noeudPrec,noeudPrec))){// on creer quand meme la commodité
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                return NULL;
            }
            P_commodites= &(*P_commodites)->suiv;
            chaine = chaine->suiv;
            continue;

        }
        P_insertionNoeudVoisin = &noeudPrec->voisins;
        while( point ){
            noeud =rechercheCreeNoeudListe(R,point->x,point->y);
            if (!rechercheVoisinAjoute2(&matrice,noeud ,noeudPrec,&taille)){
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                return NULL;
            }
            noeudPrec = noeud;
            point = point->suiv;
        }// apres avoir parcouru tout les points de la chaine, on creer la comodité
        if (!(*P_commodites = creerCommodite(premier_noeud,noeud))){
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        P_commodites= &(*P_commodites)->suiv;
        chaine = chaine->suiv;

    }
    if (chaine){// si chaine n'est pas NULL c'est que nbChaines est inférieur au nombre de  chaine de chaines existants, donc  on rapport l'erreur 
        fprintf(stderr,"Erreur :  Il y a plus de chaine que le nombre indique\n");
        printf("Annulation de la reconstitution\n");
        libererReseau(&R);
        return NULL;
    }
    libererMatrice(matrice,taille);
    free(matrice);
    return R;
}


int nbCommodites(Reseau *R){
    int nb=0;
    CellCommodite * commod = R->commodites;
    while(commod){
        nb++;
        commod = commod->suiv;
    }
    return nb;
}

int nbLiaisons(Reseau *R){
    int nb = 0;
    CellNoeud* noeuds= R->noeuds;
    CellNoeud* noeud;
    while (noeuds)
    {
        noeud= noeuds->nd->voisins;
        while (noeud)
        {
            nb++;
            noeud = noeud->suiv;
        }
        noeuds = noeuds->suiv;
        
    }
    return nb/2;
    
}

void ecrireReseau(Reseau *R, FILE *f){
    if (!R){
        fprintf(stderr,"Erreur : Le Reseau n'est pas defini\n");
        printf("L'ecriture est abandonne\n");
        return;
    }
    if (!f){
        fprintf(stderr,"Erreur : Le fichier de destination n'est pas defini\n");
        printf("L'ecriture est abandonne\n");
        return;
    }
    int NbLiaisons = nbLiaisons(R);
    int NbCommodites = nbCommodites(R);
    if (fprintf(f,"NbNoeuds: %d\nNbLiaisons: %d\nNbCommodites: %d\nGamma: %d\n\n",R->nbNoeuds,NbLiaisons,NbCommodites,R->gamma)<52){
        fprintf(stderr,"Erreur lors de l'ecriture du nombre de chaines et de Gamma\nL'ecriture est arretee\n");
        return;
    }
    int liaisonPlace =1;
    int temp = R->nbNoeuds;
    while((temp/=10)) liaisonPlace ++;// permet de savoir le nombre de caractere maximum que utilise le 'num' d'un noeuds, il va servir plus tard a savoir l'espace a allouer pour le buffer des liaisons
    liaisonPlace = liaisonPlace*2+5 ;// la liaison est composé de 2 numéros + 'l' + 2 espaces + '\n'+'\0'
    int nb = 0; // permet de savoir ou on doit ecrire dans le buffer
    int verifSprintf;
    
    int espace = 30*sizeof(char*); //espace allouer dans le malloc de bufffer
    char* buffer = (char *) malloc(espace);
    char * temp2;
    
    if (! buffer){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour le buffer de chaines\n");
        printf("L'ecriture est abandonee\n");
        return;
    }
    CellNoeud* noeuds= R->noeuds;
    int num;
    CellNoeud* noeud;
    int verifNbliaisons = 0;
    for (int x = 0; x<R->nbNoeuds;x++)// on fait un for sur nbNoeuds au lieu de faire un while(noeuds) pour s'assurer que le nombre de noeuds indiqué correspond bien au nombre de noeuds ecrit
    {
        if (!noeuds){// si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on stop l'ecriture
            fprintf(stderr,"Erreur :  %d noeud(s) trouve(s) au lieu de %d\n",x,R->nbNoeuds);
            printf("L'ecriture est abandonee\n");
            free(buffer);
            return;
        }
        num = noeuds->nd->num;
        if (fprintf(f,"v %d %lf %lf\n",num,noeuds->nd->x,noeuds->nd->y)<22){
            fprintf(stderr,"Erreur lors de l'ecriture du noeud %d\nL'ecriture est arretee\n",num);
            printf("L'ecriture est abandonee\n");
            free(buffer);
            return;
        }
        noeud= noeuds->nd->voisins;
        while (noeud)
        {
            if (num<noeud->nd->num){// on ajoute la liaison que si num est inférieur a celui du voisin pour eviter les doublons
                if ((nb+liaisonPlace)*sizeof(char)>espace){// on s'assure qu'il y a assez de place dans le buffer pour la nouvelle liaison
                    espace+=30;
                    if (!(temp2 = (char *) realloc(buffer,espace))){
                        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour le buffer de la liaison l %d %d\n",num,noeud->nd->num);
                        printf("L'ecriture est abandonee\n");
                        free(buffer);
                        return;
                    }
                    buffer = temp2;
                }
                verifSprintf = nb + 6; // permet de verifier si le sprint a bien fonctionne
                if ((nb += sprintf(buffer+nb,"l %d %d\n",num,noeud->nd->num))<verifSprintf){
                    fprintf(stderr,"Erreur lors de l'ecriture de la liaison l %d %d\n",num,noeud->nd->num);
                    free(buffer);
                    return;
                }
                verifNbliaisons++;

                
            }
            noeud = noeud->suiv;
        }
        noeuds = noeuds->suiv;
        
    }
    if (noeuds){// si noeuds n'est pas NULL c'est que nbNoeuds est inférieur au nombre de noeuds du reseau, donc  on rapport l'erreur et on arrete l'ecriture
        fprintf(stderr,"Erreur :  Il y a plus de noeuds que le nombre indique\n");
        free(buffer);
        return;
    }
    fprintf(f,"\n");
    if (verifNbliaisons != NbLiaisons){
        fprintf(stderr,"Erreur %d liaisons trouvee au lieu alors que %d a ete indique\n",verifNbliaisons,NbLiaisons);
        printf("L'ecriture est abandonne\n");
        free(buffer);
        return;
    }
    if (verifNbliaisons){
        if (fprintf(f,"%s\n",buffer)!=nb+1){
            fprintf(stderr,"Erreur lors de l'ecriture des liaisons\n");
            printf("L'ecriture est abandonne\n");
            free(buffer);
            return;
        }
    }
    free(buffer);
    CellCommodite * commod = R->commodites;
    for (int x = 0; x<NbCommodites; x++){// on fait un for sur NbCommodites au lieu de faire un while(commod) pour s'assurer que le nombre de commodité indiqué correspond bien au commodité ecrite
        if (fprintf(f,"k %d %d\n",commod->extrA->num,commod->extrB->num)<6){
            fprintf(stderr,"Erreur lors de l'ecriture de la commodite k %d %d\n",commod->extrA->num,commod->extrB->num);
            printf("L'ecriture est abandonne\n");
            return;
        }
        commod = commod->suiv;
    }
    if (commod)// si la commodite n'est pas NULL c'est que NbCommodites est inférieur au nombre de commodites du reseau, donc  on rapport l'erreur
        fprintf(stderr,"Erreur :  Il y a plus de commodites que le nombre indique\n");


}

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}



void ajouter_cellnoeud(Reseau *r , CellNoeud *cell){
    if(r->noeuds==NULL){
        r->noeuds=cell;
    }
    else{
        cell->suiv=r->noeuds;
        r->noeuds=cell;
    }

}

void EcrituresReseauFile(char * nomfichier, Reseau* R,  int mode_silencieux){
    EcrituresFile(nomfichier,ecrireReseau,R,mode_silencieux);    
}

void EcritureReseauSVG(char * nomfichier, Reseau *R , int mode_silencieux){
    EcritureSVG(nomfichier,afficheReseauSVG,R,mode_silencieux);
}



Reseau* reconstitueReseauListe3(Chaines *C){

    if (!C){// Si C n'est pas defini on arrete
        fprintf(stderr,"Erreur : L'Ensemble de chaines n'est pas defini\n");
        printf("La reconstitution est abandonnee\n");
        return NULL;
    }
    Reseau* R =(Reseau*) calloc(1, sizeof(Reseau));
    
    if (!R){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour un Reseau\n");
        printf("Reconstitution abandonnee\n");
        return NULL;
    }
    R->gamma = C->gamma;
    int taille =comptePointsTotal(C) ;
    char *matrice = calloc((taille+1)*(taille+1),sizeof(char));
    // printf("taille = %d\n",taille);
    if (!matrice) printf("chien\n");

    CellChaine * chaine = C->chaines;
    CellPoint * point ;
    Noeud*  noeudPrec; //pointeur vers le noeud precedent, il va etre utiliser pour verifier si le noeud etudié apres est deja dans sa liste de voisin
    CellCommodite ** P_commodites = &(R->commodites); // Pointeur vers l'endroit ou doit etre ajouter la prochaine commodité
    Noeud * noeud;// Pointeur vers le noeud analysé couramment
    Noeud * premier_noeud;// Pointeur vers le premier de la chaine, il va etre utilisé pour creer la commodite
    CellNoeud ** P_insertionNoeudVoisin; // pointeur vers l'endroit ou sera inserer le noeud dans la liste des noeuds voisins du noeuds precedent
    for (int x = 0; x<C->nbChaines; x++){// on fait un for sur nbChaines au lieu de faire un while(chaine) pour s'assurer que le nombre de chaines indiqué correspond bien au nombre de chaines existante
        
        if (!chaine){// si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on arrete la boucle
            fprintf(stderr,"Erreur :  %d chaine(s) trouvee(s) au lieu de %d\n",x,C->nbChaines);
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        if ( ! ( point = chaine->points) ){// on verifie si il y a bien des points dans la chaines, sinon en passe a la suivante (meme si en theorie il ne devrait pas y avoir de chaines sans points)
            chaine = chaine->suiv;
            continue;
        }
        P_insertionNoeudVoisin=NULL;
        noeudPrec = rechercheCreeNoeudListe(R, point->x, point->y);
        premier_noeud = noeudPrec;
        if(!noeudPrec){
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        if (!(point = point->suiv)){// Le cas ou la chaines n'est consitué que d'un point
            printf("La chaine %d n'est consituee que d'un point\n",chaine->numero);
            if (!(*P_commodites = creerCommodite(noeudPrec,noeudPrec))){// on creer quand meme la commodité
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                return NULL;
            }
            P_commodites= &(*P_commodites)->suiv;
            chaine = chaine->suiv;
            continue;

        }
        P_insertionNoeudVoisin = &noeudPrec->voisins;
        while( point ){
            noeud =rechercheCreeNoeudListe(R,point->x,point->y);
            if (!rechercheVoisinAjoute3(matrice,noeud ,noeudPrec,taille)){
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                return NULL;
            }
            noeudPrec = noeud;
            point = point->suiv;
        }// apres avoir parcouru tout les points de la chaine, on creer la comodité
        if (!(*P_commodites = creerCommodite(premier_noeud,noeud))){
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            return NULL;
        }
        P_commodites= &(*P_commodites)->suiv;
        chaine = chaine->suiv;

    }
    if (chaine){// si chaine n'est pas NULL c'est que nbChaines est inférieur au nombre de  chaine de chaines existants, donc  on rapport l'erreur 
        fprintf(stderr,"Erreur :  Il y a plus de chaine que le nombre indique\n");
        printf("Annulation de la reconstitution\n");
        libererReseau(&R);
        return NULL;
    }
    free(matrice);
    return R;
}
