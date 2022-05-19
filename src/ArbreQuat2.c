# include "../headers/ArbreQuat.h"

void LibererArbreQuat( ArbreQuat * Aq){
    // libere  l'espace memoire occupé par  un arbre quaternaire 'Aq'  ainsi que ses fils
    //  Attention, LibererArbreQuat  doit etre utiliser conjointement avec libererReseau afin de liberer l'attribut noeud
    if (Aq->ne)
        LibererArbreQuat(Aq->ne);
    if (Aq->no)
        LibererArbreQuat(Aq->no);
    if (Aq->se)
        LibererArbreQuat(Aq->se);
    if (Aq->so)
        LibererArbreQuat(Aq->so);
    free(Aq);
}

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    // determine les coordonnees minimales et maximales des points constituant les differentes chaines du reseau

    if (!C){// Si C n'est pas defini on arrete
        fprintf(stderr,"Erreur : L'Ensemble de chaines n'est pas defini\n");
        return;
    }
    CellChaine* chaine= C->chaines;

    while(chaine){
        
        CellPoint* point =chaine->points;
        while(point){
            // Pour chaque point on fait la comparaison entre les minimums,maximums, et ses coordonnees
            if (point->x < *xmin)
                *xmin = point->x;
            if (point->y < *ymin)
                *ymin = point->y;
            if (point->x > *xmax)
                *xmax = point->x;
            if (point->y > *ymax)
                *ymax = point->y;

            point = point->suiv;
            
        }
        chaine = chaine ->suiv;

        
    }

}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    //creer une cellule de d’arbre quaternaire, de centre (xc, yc), de longueur coteX et de hauteur coteY. La fonction initialisa le nœud du reeseau, les arbres nord-ouest, nordest, sud-ouest et sud-est a NULL
    // La fonction rend un pointeur vers l'arbre creer ou NULL en cas d'erreur
    
    ArbreQuat* Aq = (ArbreQuat*) calloc(1,sizeof(ArbreQuat)); // on utilise calloc pour mettre les attributs à NULL
    if (!Aq){// Si le calloc echoue, il rend le pointeur NULL
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour un arbre quaternaire\n");
        return NULL;
    }
    // puis on initialse les attribut connus
    Aq->xc = xc ; Aq->yc = yc;

    Aq->coteX = coteX;

    Aq->coteY = coteY;
    return Aq;
    

}
ArbreQuat** determine_emplacement(Noeud* n, ArbreQuat* parent){
    // determine a quel emplacement doit etre insere le noeud 'n' dans 'parent'
    if (n->x < parent->xc){ // cas ouest
        
        if (n->y< parent->yc) // cas sud ouest
            return &parent->so;
        // cas nord ouest
        return  &parent->no;
    }
    // cas est
    if (n->y< parent->yc) // cas sud est
        return &parent->se;
    // cas nord est
    return  &parent->ne;
}
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
    //Insere un Noeud 'n'  dans un arbre quaternaire 'a'
    // 'parent' est l'arbre parent de 'a'
    if (!n || !a || !parent)
        return;

    if (!*a){// cas de l'arbre Vide
        if (a==&parent->ne)// cas ou 'a' est un pointeur sur le nord est de parent
            *a = creerArbreQuat(parent->xc + parent->coteX/4.0 , parent->yc + parent->coteY/4.0,parent->coteX/2.0,parent->coteY/2.0);
        else if (a==&parent->no)// cas ou 'a' est un pointeur sur le nord ouest de parent
            *a = creerArbreQuat(parent->xc - parent->coteX/4.0 , parent->yc + parent->coteY/4.0,parent->coteX/2.0,parent->coteY/2.0);
        else if (a==&parent->se)// cas ou 'a' est un pointeur sur le sud est de parent
            *a = creerArbreQuat(parent->xc + parent->coteX/4.0 , parent->yc - parent->coteY/4.0,parent->coteX/2.0,parent->coteY/2.0);
        else if (a==&parent->so)// cas ou 'a' est un pointeur sur le sud ouest de parent
            *a = creerArbreQuat(parent->xc - parent->coteX/4.0 , parent->yc - parent->coteY/4.0,parent->coteX/2.0,parent->coteY/2.0);
        
        if (!*a){// c'est que la creation de l'arbre n'a pas fonctionnee
            fprintf(stderr,"Erreur : L'Ensemble de chaines n'est pas defini\n");
            return;
        }
        // et si ca a fonctionne on met tout simplement le noeud à l'emplacement
        (*a)->noeud = n;

        return;
    }
    // Cas ou on est sur une feuille
    if ((n->x == ((*a)->noeud->x) ) && (n->y == ((*a)->noeud->y) ))  // il faut s'assurer que ils ont des coordonnees differentes sinon les appels recursif seront infini
        return;
    // on insere le noeud 'n'
    ArbreQuat ** P_Aq =determine_emplacement(n,*a);
    insererNoeudArbre(n, P_Aq , *a );

    if(!(*P_Aq)){ // On s'assure que la fonction a bien fonctionnee
        LibererArbreQuat(*a); // si elle a pas fonctionne on libere l'abre pointe par a et on le met a NULL pour signaler a la fonction appelante qu'il y eu une erreur
        *a = NULL;
        return;
    }

    // on insere le noeud qui etait là precedement

    P_Aq = determine_emplacement((*a)->noeud,*a);
    insererNoeudArbre((*a)->noeud, P_Aq , *a );
    if(!(*P_Aq)){ // On s'assure que la fonction a bien fonctionnee
        LibererArbreQuat(*a); // si elle a pas fonctionne on libere l'abre pointe par a et on le met a NULL pour signaler a la fonction appelante qu'il y eu une erreur
        *a = NULL;
        return;
    }
    // on passe le noeud a NULL
    (*a)->noeud = NULL;


    
}
Noeud* rechercheNoeudArbre(ArbreQuat ***Aq, double x, double y, ArbreQuat ** parent){
    // Retourne un Noeud correspondant au point (x, y) dans l'arbre quaternaire 'Aq'
    // si le noeud n'y existe pas, le pointeur NULL est retourne

    if (!(**Aq) ) //si l'abre est NULL, on est arrivé a la fin de l'arbre et on a pas trouve de correspondance
        return NULL;

    if ((**Aq)->noeud){ // si le noeud n'est pas NULL c'est qu'on est arrive a une feuille donc on verifie si elle correspond aux coordonnée
        if ((**Aq)->noeud->x == x && (**Aq)->noeud->y == y) //Cas ou on trouve une correspondance
            return  (**Aq)->noeud;
        // si elle ne correspond pas on retourne NULL
        return NULL;
    }
    *parent =  **Aq;   // on met a jour parent
    // on continue le parcourt de l'arbre
    
    if (x < (*parent)->xc){ // cas ouest
        
        if (y< (*parent)->yc)    // cas sud ouest
            *Aq=&(*parent)->so;

        else     // cas nord ouest
            *Aq=&(*parent)->no;
    }
    // cas est
    else if (y< (*parent)->yc) // cas sud est
        *Aq=&(*parent)->se;
        
    else // cas nord est
        *Aq=&(*parent)->ne;
    return rechercheNoeudArbre(Aq, x , y, parent);
}
Noeud *rechercheCreeNoeudArbre(Reseau *R, ArbreQuat *Aq, double x, double y){
    //Retourne un Noeud du reseau R correspondant au point (x, y) dans l'arbre quaternaire 'Aq'
    // si ce point existe dans Aq, la fonction retourne un point existant dans Aq, dans le cas contraire, la fonction cree un nœud et l’ajoute dans Aq ainsi que dans la liste des nœuds du reseau de R
    ArbreQuat ** P_Aq = &Aq;
    ArbreQuat * parent;
    Noeud *noeud = rechercheNoeudArbre(&P_Aq,x,y,&parent);  // on cherche un noeud correspondant aux coordonnées dans l'arbre
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
    
    // Puis on insere le noeud dans l'abre et dans le Reseau
    insererNoeudArbre(noeud,P_Aq,parent);
    if(!(*P_Aq)){ // On s'assure que la fonction a bien fonctionnee
        free(noeud);
        free(Cellnoeud_Reseau);
        return NULL;
    }
    ajouter_cellnoeud(R, Cellnoeud_Reseau);
    return noeud;
}
Reseau* reconstitueReseauArbre(Chaines* C){
    // Reconstruit le reseau R à partir de la liste des chaines 'C' en utilisant un arbre quaternaire
    // Retourne NULL en cas d'erreur

    if (!C){// Si C n'est pas defini on arrete
     
    Reseau* R =(Reseau*) calloc(1, sizeof(Reseau));

    if (!R){
     
    
    // prepare les coordonnees de l'arbre
    double xmin = HUGE_VAL, ymin = HUGE_VAL, xmax =0  , ymax =0;

    chaineCoordMinMax( C,  &xmin,  &ymin,  &xmax,  &ymax);
    // on creer l'arbre
    ArbreQuat* Aq = creerArbreQuat((xmax + xmin)/2.0,(ymin + ymax)/2.0,xmax - xmin,ymax - ymin);
    
    if (!Aq){
      

    R->gamma = C->gamma;
    CellChaine * chaine = C->chaines;
    CellPoint * point ;
    Noeud*  noeudPrec; //pointeur vers le noeud precedent, il va etre utiliser pour verifier si le noeud etudié apres est deja dans sa liste de voisin
    CellCommodite ** P_commodites = &(R->commodites); // Pointeur vers l'endroit ou doit etre ajouter la prochaine commodité
    Noeud * noeud;// Pointeur vers le noeud analysé couramment
    Noeud * premier_noeud;// Pointeur vers le premier de la chaine, il va etre utilisé pour creer la commodite
    CellNoeud ** P_insertionNoeudVoisin; // pointeur vers l'endroit ou sera inserer le noeud dans la liste des noeuds voisins du noeuds precedent
    // on parcourt les chaines
    for (int x = 0; x<C->nbChaines; x++){// on fait un for sur nbChaines au lieu de faire un while(chaine) pour s'assurer que le nombre de chaines indiqué correspond bien au nombre de chaines existante
        if (!chaine){// si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on arrete la boucle
            fprintf(stderr,"Erreur :  %d chaine(s) trouvee(s) au lieu de %d\n",x,C->nbChaines);
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            LibererArbreQuat(Aq);
            return NULL;
        }
        if ( ! ( point = chaine->points) ){// on verifie si il y a bien des points dans la chaines, sinon en passe a la suivante (meme si en theorie il ne devrait pas y avoir de chaines sans points)
            chaine = chaine->suiv;
            continue;
        }
        P_insertionNoeudVoisin=NULL;
        // on cherche le premier noeud de la chaine
        noeudPrec = rechercheCreeNoeudArbre(R,Aq, point->x, point->y);
        premier_noeud = noeudPrec;
        if(!noeudPrec){
            
        }
        
        if (!(point = point->suiv)){// Le cas ou la chaines n'est consitué que d'un point
            printf("La chaine %d n'est consituee que d'un point\n",chaine->numero);
            if (!(*P_commodites = creerCommodite(noeudPrec,noeudPrec))){// on creer quand meme la commodité
                printf("Annulation de la reconstitution\n");
                libererReseau(&R);
                LibererArbreQuat(Aq);
                return NULL;
            }
            P_commodites= &(*P_commodites)->suiv;
            chaine = chaine->suiv;
            continue;

        }
        
        
        P_insertionNoeudVoisin = &noeudPrec->voisins;
        
        // on parcourt les points de la chaine
        while( point ){
            // on recupere le noeuds correspondant au point
            noeud =rechercheCreeNoeudArbre(R,Aq,point->x,point->y);
            // on met a jour les voisins
            P_insertionNoeudVoisin = rechercheVoisinAjoute(noeud ,noeudPrec,  P_insertionNoeudVoisin);
            if (!P_insertionNoeudVoisin){
               
            }
            noeudPrec = noeud;
            point = point->suiv;
            
        }
        
        // apres avoir parcouru tout les points de la chaine, on creer la comodité
        if (!(*P_commodites = creerCommodite(premier_noeud,noeud))){
            printf("Annulation de la reconstitution\n");
            libererReseau(&R);
            LibererArbreQuat(Aq);
            return NULL;
        }
        P_commodites= &(*P_commodites)->suiv;
        chaine = chaine->suiv;
    }
    
    
    LibererArbreQuat(Aq);// on a plus besoin de l'arbre donc on le libere
    if (chaine){// si chaine n'est pas NULL c'est que nbChaines est inférieur au nombre de  chaine de chaines existants, donc  on rapport l'erreur 
        fprintf(stderr,"Erreur :  Il y a plus de chaine que le nombre indique\n");
        printf("Annulation de la reconstitution\n");
        libererReseau(&R);
        return NULL;
    }
    return R;
}
