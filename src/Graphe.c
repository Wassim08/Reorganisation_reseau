# include "../headers/Graphe.h"


void libererC_arete(Cellule_arete* c_arete ){
    if (c_arete->a){
        if (c_arete->a->u<0)// si u est negatif , c'est que c'est le deuxieme passage donc on peut liberer l'espace
            free(c_arete->a);
        else c_arete->a->u = -1; // si c'est la premiere fois qu'on le voit, on notifie le passage en passant 'u' a -1
    }
    free( c_arete);
}

void libererSommet(Sommet* s ){
    // libere un Sommet 's' et toutes ses composantes
    if (!s) return;
    Cellule_arete* voisin = s->L_voisin;
    while(voisin){
        s->L_voisin = voisin->suiv;
        libererC_arete(voisin);
        voisin = s->L_voisin;
    }
    free(s);
}


void libererGraphe(Graphe* g){
    // libere un graphe 'g' et toute ses composantes
    if (!g) NULL;
    
    if (g->T_som){
        for(int i = 0 ; i<= g->nbsom; i++){
            libererSommet(g->T_som[i]);
        }
        free(g->T_som);
    }
    if(g->T_commod)
        free(g->T_commod);
    free(g);
}


int creerTabCommod(Reseau * R, Commod ** commodites){
    //creer un tableau de commodite a partir d'un reseau 'R' en les ajoutant a *'commodites'  et retourne le nombre de commodite
    // retourne -1 en cas d'erreur
    CellCommodite* commod;
    if (!(commod = R->commodites)) return 0;
    
    int cpt = 1; //permet de compter les comodite
    // on alloue l'espace pour le tableau
    Commod * T_commod = (Commod *) malloc(sizeof(Commod ));
    Commod * temp;
    if(!T_commod){
        fprintf(stderr,"Erreur: Impossible d'allouer l'espace nécéssaire pour un tableau de commodite\n");
        return -1;
    }
    // on inialise la premiere instance de Commod
    T_commod[0].e1 = commod->extrA->num;
    T_commod[0].e2 = commod->extrB->num;

    // Puis on fait une boucle pour ajouter les autres commodite
    while ((commod = commod->suiv))
    {
        // on agrandi l'espace du tableau
        temp = (Commod *) realloc(T_commod,++cpt* sizeof(Commod ));
        if (!temp){
            fprintf(stderr,"Erreur: Impossible d'allouer l'espace nécéssaire pour un tableau de commodite\n");
            free(T_commod);
            return -1;
        }
        T_commod = temp;

        //Puis on l'initialise
        T_commod[cpt -1].e1 = commod->extrA->num;
        T_commod[cpt -1].e2 = commod->extrB->num;

        
    }
    // une fois l'a boucle fini on insere le tableau dans le graphe et on rend le compteur
    *commodites =  T_commod;
    return cpt;



}



Graphe* creerGraphe(Reseau* r){
    // Cree un graphe a partir d’un reseau 'r'

    if (!r){
        fprintf(stderr,"Erreur: Le reseau n'est pas defini\n");
        printf("Creation du graphe abandonnee\n");
        return NULL;
    }
    // on alloue d'abord l'espace nécéssaire pour un graphe
    Graphe* g = (Graphe*) calloc(1,sizeof(Graphe));
    if (!g){
        fprintf(stderr,"Erreur: Impossible d'allouer l'espace nécéssaire pour un graphe\n");
        printf("Creation du graphe abandonnee\n");
        return NULL;
    }

    //  on initalise les attributs simples
    g->gamma = r->gamma;
    g->nbsom = r->nbNoeuds;

    if(!g->nbsom) return g; //si il y'a pas de noeud , il y a pas non plus de commodite, donc on rend le graphe vide


    // on declare les variable dont on aura besoin pour la creation du graphe
    Sommet* sommet;
    Cellule_arete* c_arete;
    Arete* arete;
    CellNoeud * c_noeud;
    CellNoeud * c_noeud2;
    Noeud * noeud;
    Noeud * noeud2;
    int cpt;

    Sommet ** T_som = (Sommet **) calloc((g->nbsom +1),sizeof(Sommet*));
    if (!T_som){
        fprintf(stderr,"Erreur: Impossible d'allouer l'espace nécéssaire pour le tableau de sommets\n");
        printf("Creation du graphe abandonnee\n");
        libererGraphe(g);
        return NULL;
    }
    // on met T_som dans le grape
    g->T_som = T_som;
    // on recupere le premier point
    c_noeud = r->noeuds ;
    // on fait un for sur g->nbsom pour ajouter les sommets au graphe
    cpt =  g->nbsom;
    for (int i = 0; i<cpt ; i++){
        if (!c_noeud){// on verifie qu'il y a bien un noeud
            fprintf(stderr,"Erreur: 'r->nbNoeuds' indique %d noeuds, hors il y en a %d dans le Reseau\n",r->nbNoeuds,i);
            printf("Creation du graphe abandonnee\n");
            libererGraphe(g);
            return NULL;

        }
        // on verifie que le noeud est conforme 
        noeud = c_noeud ->nd;
        if(!noeud){
            fprintf(stderr,"Erreur un des Cellnoeud du reseau ne contient pas de noeud\n");
            printf("Creation du graphe abandonnee\n");
            libererGraphe(g);
            return NULL;

        }
        if (noeud->num<1|| noeud->num>cpt){
            fprintf(stderr,"Erreur dans la numerotation des noeuds du reseau, un noeud porte le numero %d alors que il devrait etre entre 1 et %d\n",noeud->num,cpt);
            printf("Creation du graphe abandonnee\n");
            libererGraphe(g);
            return NULL;
            
        }
        if (T_som[noeud->num]){
            fprintf(stderr,"Erreur: 2 noeuds du reseau porte le numero %d\n",noeud->num);
            printf("Creation du graphe abandonnee\n");
            libererGraphe(g);
            return NULL;

        }
        // on alloue l'espace pour un sommet
        sommet = (Sommet*) malloc (sizeof(Sommet));
        if (!sommet){
            fprintf(stderr,"Erreur: Impossible d'allouer l'espace nécéssaire pour le sommet %d\n",noeud->num);
            printf("Creation du graphe abandonnee\n");
            libererGraphe(g);
            return NULL;
        }
        // on l'initialise
        sommet->num = noeud->num ;
        sommet->x = noeud->x;
        sommet->y = noeud->y;
        sommet->L_voisin = NULL;

        // et on place le sommet dans le graphe

        g->T_som[sommet->num] = sommet;
        
        // on cherche les voisins
        c_noeud2 = noeud->voisins;
        while (c_noeud2)
        {
            if(!(noeud2 = c_noeud2->nd)){
                fprintf(stderr,"Erreur un des Cellnoeud du reseau ne contient pas de noeud\n");
                printf("Creation du graphe abandonnee\n");
                libererGraphe(g);
                return NULL;
            }

            // on fait une verification sur le voisin
            if(noeud2->num<1||noeud2->num>cpt){
                fprintf(stderr,"Erreur dans la numerotation des noeuds du reseau, un noeud voisin de %d porte le numero %d alors que il devrait etre entre 1 et %d\n",sommet->num,noeud2->num,cpt);
                printf("Creation du graphe abandonnee\n");
                libererGraphe(g);
                return NULL;
            }
            // si le voisin a deja été ajouté a T_som, on ajoute l'arret ( ca permet d'eviter les doublons)
            if (g->T_som[noeud2->num]){
                // on alloue l'espace pour Cellule_arete
                c_arete =(Cellule_arete*) calloc (1,sizeof(Cellule_arete));
                if (!c_arete){
                    fprintf(stderr,"Erreur: Impossible d'allouer l'espace pour une Cellule_arete\n");
                    printf("Creation du graphe abandonnee\n");
                    libererGraphe(g);
                    return NULL;
                }

                // on accroche  la Cellule_arete aux voisin de sommet
                // insertion en tete
                c_arete->suiv = sommet->L_voisin;
                sommet->L_voisin = c_arete;

                // on alloue l'espace pour une arete 
                arete = (Arete*) malloc(sizeof(Arete));
                if (!arete){
                    fprintf(stderr,"Erreur: Impossible d'allouer l'espace pour une arete\n");
                    printf("Creation du graphe abandonnee\n");
                    libererGraphe(g);
                    return NULL;
                }

                // on initialise arete
                arete->u = sommet->num;
                arete->v = noeud2->num;

                // on insere arete dans sa Cellule_arete
                c_arete->a = arete;

                // on alloue l'espace pour la Cellule_arete du deuxieme sommet
                c_arete =(Cellule_arete*) calloc (1,sizeof(Cellule_arete));
                if (!c_arete){
                    fprintf(stderr,"Erreur: Impossible d'allouer l'espace pour une Cellule_arete\n");
                    printf("Creation du graphe abandonnee\n");
                    libererGraphe(g);
                    return NULL;
                }

                // on accroche  la Cellule_arete aux voisin de  g->T_som[noeud2->num-1]
                // insertion en tete
                c_arete->suiv = g->T_som[arete->v]->L_voisin;
                g->T_som[arete->v]->L_voisin = c_arete;

                // on insere arete dans sa  deuxieme Cellule_arete
                c_arete->a = arete;


            }
            c_noeud2 = c_noeud2->suiv;

        }
        c_noeud = c_noeud->suiv;
        

    }
    // puis on met les commodité
    if ((g->nbcommod = creerTabCommod (r,&g->T_commod))<0){
        printf("Creation du graphe abandonnee\n");
        libererGraphe(g);
        return NULL;
    }

    // on a plus qu'a retourner le graphe
    return g;

        


}







int plusCourtChemin(Graphe * g, int u, int v){
    //retourne le plus petit nombre d’aretes d’une chaıne entre deux sommets 'u' et 'v' d’un graphe 'g'
    // retourne 0 si u==v , -1 si aucun parcourt n'existe et -2 en cas d'erreur 

    if (v<0 || v>g->nbsom || u<0 || u>g->nbsom || !g->T_som[u] || !g->T_som[v]){
        fprintf(stderr,"Erreur: u et v doivent etre dans le graphe 'g'\n");
        printf("%d\t,%d\t,%d\t,%d\t,%d\t,%d\t\n",v<0 , v>g->nbsom , u<0 , u>g->nbsom , !g->T_som[u] , !!g->T_som[v]);//suppr
        return -2;
    }
    if (u==v) return 0;

    // on alloue un tableau pour repertorier les elements deja rencontre
    char* rencontre= calloc(g->nbsom+1,sizeof(char));
    if (!rencontre){
        fprintf(stderr,"Erreur: Impossible d'allouer l'espace nécéssaire pour un tableau de char\n");
        return -2;
    }
    // on met v dedans
    rencontre[v] = 'o';
    // on declare la file pour le parcours en largeu
    S_file  file;
    // on initialise la file
    Init_file(&file);

    // puis on demare la recherche

    
    // on part depuis v
    Cellule_arete * voisin = g->T_som[v]->L_voisin;

    int dernier_voisin;// permetra plus tard de savoir quand augmenter cpt
    

    // on ajoute les voisins de v a la file et pour chaque voisin de v on creer une liste
    while (voisin)
    {
        if ((dernier_voisin = voisin->a->u)!= v){
            enfile(&file,dernier_voisin);
        }

        else{
            dernier_voisin = voisin->a->v;
            enfile(&file,dernier_voisin);
        }
        // on verifie si on a trouvé u
        if (dernier_voisin == u){
            liberer_file(&file);
            free(rencontre);
            return 1;
        }
        
        // on place le voisin dans le tableau de rencontre
        rencontre[dernier_voisin] = 'o';

        voisin = voisin->suiv;



    }
    if (estFileVide(&file)){
        liberer_file(&file);
        free(rencontre);
        return -1;
    }
    // maintenant on peut demarer une boucle pour chercher le reste du chemin
    int num;
    int num2;
    int cpt = 2; // compteur d'aretes , on a deja passé les voisins directes de v , donc on inialise cpt à 2
    while(!estFileVide(&file)){
        num = defile(&file);
        
        voisin = g->T_som[num]->L_voisin;

        while (voisin)
        {
            
            if ( !rencontre [voisin->a->u]){
                num2 = voisin->a->u;
                enfile(&file,num2);
            }
            else if (!rencontre [voisin->a->v] ){
                num2 = voisin->a->v;
                enfile(&file,num2);
            }
            else {
                voisin = voisin->suiv;
                continue;
            }
            // on verifie si on a trouvé u
            if (num2 == u){
                liberer_file(&file);
                free(rencontre);
                return cpt;
            }
            // puis on ajoute le voisin a le tableau des rencontre
            rencontre[num2] = 'o';

            voisin = voisin->suiv;
        }
        if (num == dernier_voisin){
            cpt ++;
            dernier_voisin = num2;
        }

    }
    // si on arrive la c'est que aucun chemin n'a ete trouve
    liberer_file(&file);
    free(rencontre);
    return -1;   

}

ListeEntier plusCourtChemin2(Graphe * g, int u, int v){
    //retourne la plus petite chaine issue de u allant a v

    if (v<0 || v>g->nbsom || u<0 || u>g->nbsom || !g->T_som[u] || !g->T_som[v]){
        fprintf(stderr,"Erreur: u et v doivent etre dans le graphe g\n");
        return NULL;
    }
    if (u==v) return NULL;

    // on alloue l'espace pour un tableau de liste
    ListeEntier * tab_Liste = (ListeEntier *) calloc(g->nbsom+1,sizeof(ListeEntier ));
    if (!tab_Liste){
        fprintf(stderr,"Erreur: Impossible d'allouer l'espace nécéssaire pour un tableau de liste\n");
        return NULL;
    }
    // on declare la liste qui servira a enregistrer les chemins
    ListeEntier  liste = NULL;
    // on declare la liste qui nous permetra de savoir quelles listes desallouer
    ListeEntier Liste_a_effacer = NULL;
    // on ajoute v dans les 2 listes
    ajoute_en_tete(&Liste_a_effacer,v);
    ajoute_en_tete(&liste,v);

    // on place la liste dans le tableau
    tab_Liste[v] =  liste;

    // on declare la file pour le parcours en largeu
    S_file file;
    // on initialise la file
    Init_file(&file);

    
    // on part depuis v
    Cellule_arete * voisin = g->T_som[v]->L_voisin;

    int num2;
    

    // on ajoute les voisins de v a la file et pour chaque voisin de v on creer une liste
    while (voisin)
    {
        if ((num2 = voisin->a->u)!= v){
            enfile(&file,num2);
        }

        else{
            num2 = voisin->a->v;
            enfile(&file,num2);
        }




        // on verifie si on a trouvé u
        if (num2 == u){
            liberer_file(&file);
            desalloue(&Liste_a_effacer);
            tab_Liste[v] = NULL; //on met  tab_Liste[v] a NULL pour qu'il ne soit pas liberer par desalloue_tab_liste
            desalloue_tab_liste(tab_Liste,g->nbsom+1);
            return ajoute_en_tete2(liste,num2);// on retourne la liste (v,num2)
        }

        // on met num2 dans dans une liste qui est accrouché a 'liste' et on la place dans le tableau de liste
        tab_Liste[num2] = ajoute_en_tete2(liste,num2);
        // on place num2 dans la liste des elements a effacer
        ajoute_en_tete(&Liste_a_effacer,num2);
        
        voisin = voisin->suiv;



    }
    if (estFileVide(&file)){
        liberer_file(&file);
        desalloue(&Liste_a_effacer);
        desalloue_tab_liste(tab_Liste,g->nbsom+1);
        return NULL;
    }
    // maintenant on peut demarer une boucle pour chercher le reste du chemin
    int num;
    
    while(!estFileVide(&file)){
        num = defile(&file);
        liste = tab_Liste[num];
        
        voisin = g->T_som[num]->L_voisin;

        while (voisin)
        {
            
            if ( !tab_Liste [voisin->a->u]){
                num2 = voisin->a->u;
                enfile(&file,num2);
            }
            else if (!tab_Liste [voisin->a->v] ){
                num2 = voisin->a->v;
                enfile(&file,num2);
            }
            else {
                voisin = voisin->suiv;
                continue;
            }


            // on verifie si on a trouvé u
            if (num2 == u){
                liberer_file(&file);
                printf("u = %d, v = %d\n",u,v);
                desalloue_tab_liste2(tab_Liste,Liste_a_effacer,liste);
                return ajoute_en_tete2(liste,num2);
            }
            // puis on ajoute le voisin a la liste et on le met dans le tableau
            tab_Liste[num2] = ajoute_en_tete2(liste,num2);

            // on place num2 dans la liste des elements a effacer
            ajoute_en_tete(&Liste_a_effacer,num2);
            voisin = voisin->suiv;
        }

    }
    // si on arrive la c'est que aucun chemin n'a ete trouve
    liberer_file(&file);
    desalloue(&Liste_a_effacer);
    desalloue_tab_liste(tab_Liste,g->nbsom+1);
    return NULL;   

}







int maj_matrice_s_s(int * matrice, ListeEntier chaine, int nbcolonne,int gamma){
    /* met a jour la matrice sommet-sommet 'matrice' de 'nbcolonne' avec les aretes composant la chaine 'chaine'
    si une des valeurs de la matrices est superieur a 'gamma' la fonction s'arrete et rend 0, la fonction rend 1 sinon */


    int num1,num2;
    int index;
    int index2;

    while(chaine){
        num1 = chaine->u;
        if (!(chaine = chaine->suiv)) return 1; // si chaine->suiv est NULL c'est qu'on est arrive au bout de la chaine 
        num2 = chaine->u;
        index = getIndex(num1,num2,nbcolonne);
        index2 = getIndex(num2,num1,nbcolonne);
        if ((++ matrice [index])>gamma || (++ matrice [index2])>gamma){ // 
            printf("Il y a plus de %d chaines qui passent par l'arete ( %d , %d ) ",gamma,num1,num2);
            return 0;
        }
    }
    return 1;
}

int reorganiseReseau(Reseau* r){
    /* retourne vrai si pour toute arete du graphe, le nombre de chaines qui passe par cette
    arete est inferieur a γ, et faux sinon.
    Attention, la fonction retourne vrai si le 'r' est NULL*/
    
    if (!r) return 1;

    Graphe * g = creerGraphe(r);
    if(!g){
        printf("Reorganisation abandonne\n");
        return 0;
    }
    // on fait une matrice sommet - sommet qui permetra de compter les arretes
    int * matrice = calloc (sizeof(int),(g->nbsom)*(g->nbsom));
    if (!matrice){
        fprintf(stderr,"Impossible d'allouer l'espace pour la matrice sommet - sommet de taille %d x %d\n",g->nbsom,g->nbsom);
        printf("Reorganisation abandonnee\n");
        libererGraphe(g);
        return 0;
    }
    // les numeros des sommet comencent a 1 donc on decale 'matrice' pour que l'appel a getIndice nous mene au bon espace memoire
    matrice-=g->nbsom + 1;
    Commod Commodite;
    ListeEntier chaine = NULL;
    // on fait une boucle sur g->nbcommod pour acceder a toute les commodité de du tableau g->T_commod
    for (int i = 0; i< g->nbcommod ; i++){
        Commodite = g->T_commod[i];
        
        chaine = plusCourtChemin2(g,Commodite.e1,Commodite.e2);

        printf("plus court de %d a %d\n",Commodite.e1,Commodite.e2);
        afficheListe(chaine);//suppr
        printf ("nb arete %d\n\n",plusCourtChemin(g,Commodite.e1,Commodite.e2));//suppr
        if (!chaine){
            fprintf(stderr,"Impossible de trouver le chemin du sommet %d ( %.9g , %.9g ) au sommet %d ( %.9g , %.9g )\n",Commodite.e1,g->T_som[Commodite.e1]->x,g->T_som[Commodite.e1]->y,Commodite.e2  ,g->T_som[Commodite.e2]->x,g->T_som[Commodite.e2]->y);
            printf("Reorganisation abandonnee\n");
            free(matrice + g->nbsom + 1);
            libererGraphe(g);
            return 0;
        }

        if (!maj_matrice_s_s(matrice, chaine,g->nbsom,g->gamma)){
            printf("Reorganisation abandonnee\n");
            desalloue(&chaine);
            free(matrice + g->nbsom + 1);
            libererGraphe(g);
            return 0;
        }

        desalloue(&chaine);

    }
    // Si on arrive là c'est qu'il y a pas eu de problemes donc on peut retourner vrai (1)
    desalloue(&chaine);
    free(matrice + g->nbsom + 1);
    printf("nb comod %d\n",g->nbcommod);
    libererGraphe(g);
    return 1;
}

Reseau* reconstitueReseau(Chaines *C,int structure ,int M, int verification){
    /* Retourne Un reseau constitué a partir des chaines 'C' en utilisant la structure 'structure', ou NULL en cas d'erreur
    structure 1: liste, structure 2: table de hachage, structure 3 Arbre quaternaire
    si la structure 2 est choisi, la valeur de 'M' sera utilisé pour choisir la taille de la table de hachage
    Si 'verification' est vrai , la coherence des aretes avec la valeur de gamma seras verifié et le reseau ne sera retourné que si c'est cohérent*/

    Reseau * reseau;
    
    if (structure == 1)     reseau = reconstitueReseauListe(C);

    else if (structure == 2) reseau = reconstitueReseauHachage(C,M);

    else if (structure == 3) reseau = reconstitueReseauArbre(C);

    else{ // cas ou la valeur de structure n'est pas bonne
        fprintf(stderr,"La valeur de structure choisi doit etre comprise entre 1 et 3 inclus\n");
        printf("Reconstitution abandonnee\n");
        return NULL;
    }
    printf("%d suppr\n",reseau ==NULL);
    if (verification){
        if (!reorganiseReseau(reseau)){
            printf("La valeur de gamma n'est pas coherente avec les aretes\n");
            ecrireReseau(reseau,stdout);
            libererReseau(&reseau);
        }
    }
    printf("finih\n");
    return reseau;
}
