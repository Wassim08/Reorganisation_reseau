# include "../headers/Chaine.h"
# include "../headers/SVGwriter.h"

void libereCellChaine( CellChaine* chaine){
    //desalloue l'espace de la CellChaine (chaine) donné en parametre ainsi que celui de chacun de ses points
    CellPoint * point;
    while ((point =chaine ->points) != NULL)
    {
        chaine ->points = point->suiv;
        free(point);
    }
    free (chaine);
    
}

void libereChaines(Chaines* Chaines){
    //desalloue l'espace de l'ensemble de chaines (Chaines) donné en parametre ainsi que celui de chacune de ses chaines
    if (!Chaines) return;

    CellChaine * chaine ;
    

    while ((chaine =Chaines ->chaines) != NULL){
        Chaines->chaines=chaine->suiv;
        libereCellChaine(chaine);
    }
    free(Chaines);
}


Chaines* lectureChaines(FILE *f){
    /* retourner un pointeur vers une instance de  Chaines allouer dynamiquement a partir du contenu d'un flux 'f' */
    Chaines * EnsembleChaines= (Chaines *) calloc( 1 ,sizeof(Chaines));
    if (!EnsembleChaines){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour 'Chaines'\n");
        printf("La lecture est abandonnee\n");
        return NULL;
    }
    // on recupere d'abord le nombre de chaine et gamma
    int nombre1 , nombre2;
    if (fscanf(f,"%*[^0-9] %d %*[^0-9] %d",&nombre1,&nombre2)!=2){
        fprintf(stderr,"Erreur lors de la lecture des 2 premieres lignes du fichier\n");
        printf("La lecture est abandonnee\n");
        free(EnsembleChaines);
        return NULL;
    }
    EnsembleChaines ->nbChaines = nombre1;
    EnsembleChaines ->gamma = nombre2;

    
    if (!EnsembleChaines ->nbChaines){ // si il n'y a pas de chaines, pas besoin d'aller plus loin
        return EnsembleChaines;
    }
    
    
    double x , y ; // permet de lire les points
    CellChaine** PP_chaine=&EnsembleChaines ->chaines; //pointeur pour savoir ou doit etre placer la prochaine chaines, permet de faire une insertion en queue
    
    CellPoint** PP_point;//pointeur pour savoir ou doit etre placer le prochain point, permet de faire une insertion en queue
    CellPoint* point;
    CellChaine* chaine;

    for (int i=0; i<EnsembleChaines->nbChaines;i++){// on va lire autant de chaine que nbChaines
        // Si il y a un probleme lors de la lecture d'une ligne, on abandonne la lecture et on libere tout l'ensembles de chaines car au vue du sujets enregistré un ensemble incomplet pourrait mener a un resulatat non concluant

        // on alloue l'espace pour une nouvelle chaine
        chaine= (CellChaine*) calloc(1,sizeof(CellChaine));
        if (!chaine){
            fprintf(stderr,"Erreur lors de l'allocation de l'espace pour la chaine %d\n",i);
            printf("La lecture est abandonnee\n");
            libereChaines(EnsembleChaines);
            return NULL;
        }
        // on place la chaine dans l'ensemble
        *PP_chaine = chaine;
        PP_point = & chaine->points;
        PP_chaine = & chaine->suiv;
    
        // on lit le numero de chaine et le nombre de points de la chaine
        if (fscanf(f,"%d %d",&nombre1,&nombre2)!=2){
            fprintf(stderr,"Erreur lors de la lecteur de la chaine %d\n",i);
            printf("La lecture est abandonnee\n");
            
            libereChaines(EnsembleChaines);
            return NULL;
        }
        chaine->numero = nombre1;

        // on fait un for sur 'nombre2' qui est le nombre de point de la chaine pour pouvoir recuperer tout les points et les ajouter a la chaine
        for (int j=0; j<nombre2;j++){

            // on alloue l'espace pour un point
            point= (CellPoint*) calloc( 1 ,sizeof(CellPoint));
            if (!point){
                fprintf(stderr,"Erreur lors de l'allocation de l'espace pour le point %d de la chaine %d\n",j,i);
                printf("La lecture est abandonnee\n");
                libereChaines(EnsembleChaines);
                return NULL;
            }
            // on fait une insertion en queue du point dans la liste des points de la chaine
            *PP_point = point;
            PP_point =&point ->suiv;

            // on recupere les coordonnees du point
            if (fscanf(f,"%lf %lf",&x,&y)!=2){
                fprintf(stderr,"Erreur lors de la lecture du point %d de la chaine %d\n",j,i);
                printf("La lecture est abandonnee\n");
                libereChaines(EnsembleChaines);
                return NULL;
            }

            // on initialise le point
            point->x=x ; 
            point->y=y ;

        }
        
    }
    
    return EnsembleChaines;

}

void ecrireChaines(Chaines *C, FILE *f){
    //ecrit dans un flux 'f' le contenu d’une Chaines 'C'
    //on considere que les coordonnees des points ont au maximum 9 chiffres exemple (123.456789)
    if (!C){
        fprintf(stderr,"Erreur : L'Ensemble de chaines n'est pas defini\n");
        printf("L'ecriture est abandonne\n");
        return;
    }
    if (!f){
        fprintf(stderr,"Erreur : Le fichier de destination n'est pas defini\n");
        printf("L'ecriture est abandonne\n");
        return;
    }

    // on ecrit le nombre de chaines et gamma
    if (fprintf(f,"NbChain : %d\nGamma : %d\n\n",C->nbChaines,C->gamma)<23){
        fprintf(stderr,"Erreur lors de l'ecriture du nombre de chaines et de Gamma\n");
        printf("L'ecriture est abandonne\n");
        return;
    }
    
    
    CellChaine* chaine= C->chaines;
    int nb; // permet de savoir ou on doit ecrire dans le buffer
    int espace = 30*sizeof(char*); //espace allouer dans le malloc de bufffer
    
    char* buffer = (char *) malloc(espace); // le buffer va servir a contenir tout le contenu d'une chaine au lieu de la parcourir une premiere fois pour compter les points et une deuxieme fois pour les ecrire
    char * temp;
    if (! buffer){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour le buffer de chaines\n");
        printf("L'ecriture est abandonee\n");
        return;
    }
    // on parcourt les chaines
    for (int x = 0; x<C->nbChaines; x++){// on fait un for sur nbChaines au lieu de faire un while(chaine) pour s'assurer que le nombre de chaines indiqué correspond bien au nombre de chaine ecrite
        
        if (!chaine){// si nbChaines est superieur aux nombre de chaine dans chaines, on rapport l'erreur et on stop l'ecriture
            fprintf(stderr,"Erreur :  %d chaine(s) trouvee(s) au lieu de %d\n",x,C->nbChaines);
            printf("L'ecriture est abandonee\n");
            free(buffer);
            return;
        }
        nb=0; //nombre de carractere ecrit dans le buffer
        int nbPoint=0;
        int verifSprintf;// permet de verifier si le sprint a bien fonctionne
        CellPoint* point =chaine->points;
        // on parcourt les points de la chaine
        while(point){
            // on verifie que le buffer est assez grand
            if ((nb + 20 + 1)*sizeof(char)>espace){ // nb est le nombre de carractere deja ecrit, 20 et le nombre de caracatere maximum pour ecrire un point et 1 est la place pour le '\0' 
                espace +=30;
                if (!(temp = (char *) realloc(buffer,espace))){
                    fprintf(stderr,"Erreur lors de l'allocation de l'espace pour le buffer de la chaine %d\n",x);
                    printf("L'ecriture est abandonee\n");
                    free(buffer);
                    return;
                }
                buffer=temp;
            }
            // on ecrit le point dans le buffer
            verifSprintf = nb + 4; // permet de verifier si le sprint a bien fonctionne
            if ((nb += sprintf(buffer+nb," %.9g %.9g",point->x,point->y))<verifSprintf){
                fprintf(stderr,"Erreur lors de l'ecriture du point %d de la chaine %d\n",nbPoint,x);
                printf("L'ecriture est abandonee\n");
                free(buffer);
                return;
            }
            nbPoint++;
            point = point->suiv;
            
        }
        // un fois le parcourt de la chaine termine, on sait son nombre de point donc point, peut ecrire la chaine
        if (fprintf(f,"%d %d%s\n",chaine->numero,nbPoint,buffer)<(nb + 4)){
            fprintf(stderr,"Erreur lors de l'ecriture de la chaine %d\n",x);
            printf("L'ecriture est abandonee\n");
            free(buffer);
            return;
        }
        
        chaine = chaine ->suiv;

        
    }
    if (chaine)// si chaine n'est pas NULL c'est que nbChaines est inférieur au nombre de  chaine de chaines, donc  on rapport l'erreur 
        fprintf(stderr,"Erreur :  Il y a plus de chaine que le nombre indique\n");
    free(buffer);

}

void ecrireChaines_non_eff(Chaines *C, FILE *f){
    //ecrit dans un flux 'f' le contenu d’une Chaines 'C'
    //on considere que les coordonnees des points ont au maximum 9 chiffres exemple (123.456789)
    // cette fonction a la difference de (ecrireChaines) parcours 2 fois chaque chaine , 1 fois pour compter le nombre de point et une deuxieme fois pour ecrire la chaine
    if (!C){
        fprintf(stderr,"Erreur : L'Ensemble de chaines n'est pas defini\n");
        printf("L'ecriture est abandonne\n");
        return;
    }
    if (!f){
        fprintf(stderr,"Erreur : Le fichier de destination n'est pas defini\n");
        printf("L'ecriture est abandonne\n");
        return;
    }
    // on ecrit le nombre de chaines et gamma
    if (fprintf(f,"NbChain : %d\nGamma : %d\n\n",C->nbChaines,C->gamma)<23){
        fprintf(stderr,"Erreur lors de l'ecriture du nombre de chaines et de Gamma\nL'ecriture est arretee\n");
        return;
    }
    
    // on parcourt les chaines
    CellChaine* chaine= C->chaines;
    for (int x = 0; x<C->nbChaines; x++){// on fait un for sur nbChaines au lieu de faire un while(chaine) pour s'assurer que le nombre de chaines indiqué correspond bien au nombre de chaine ecrite
        
        if (!chaine){
            fprintf(stderr,"Erreur :  %d chaine(s) trouvee(s) au lieu de %d\nL'ecriture est arretee\n",x,C->nbChaines);
            return;
        }
        int nbPoint=0;
        CellPoint* point =chaine->points;
        // on parcourt une premiere fois les points de la chaine pour les compter
        while(point){
            nbPoint++;
            point = point->suiv;
        }
        // on n'ecrit le numero de la chaine et le nombre de points de la chaine
        fprintf(f,"%d %d",chaine->numero,nbPoint);
        point =chaine->points;     
        // on parcourt une deuxieme fois la chaine pour ecrire les points   
        while(point){
            if (fprintf(f," %.9g %.9g",point->x,point->y)<4){
                fprintf(stderr,"Erreur lors de l'ecriture du point %d de la chaine %d\nL'ecriture est arretee\n",nbPoint,x);
                return;
            }
            point = point->suiv;
        }
        fprintf(f,"\n");
        
        chaine = chaine ->suiv;

        
    }
    

}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    //creer un fichier SVG en html de nom 'nomInstance' pour visualiser des chaines 'C'
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

double distance(CellPoint* p1, CellPoint* p2){
    // rentre la distance physique entre 2 CellPoint 'p1' et 'p2'
    return sqrt( pow(p1->x - p2->x ,2) + pow(p1->y - p2->y ,2)         );
}
double longueurChaine(CellChaine *c){
    // retourne la longueure physique entre de la  chaine 'c'
    if (!c) return 0;
    CellPoint* p1 = c->points ;
    if (!p1) return 0;
    CellPoint* p2;
    double res = 0;
    
    while ( ( p2 = p1->suiv) )
    {
        res += distance(p1,p2);

        p1 = p2;
        p2 = p2->suiv;
    }
    return res;
    
}

double longueurTotale(Chaines *C){
    //  retrourne la longueur physique totale des chaines de 'C'
    if (!C) return 0;
    CellChaine * c = C->chaines;
    double res = 0;
    while ( c) // tant que la chaine n'est pas nulle on execute longueurChaine(c)
    {
        res += longueurChaine(c);
        c = c->suiv;
    }
    
    return res;
}

int comptePointsChaine(CellChaine *chaine){
    // Retourne le nombre  de points dans la chaine 'chaine'
    if (!chaine) return 0;
    int nbPoint=0;
    CellPoint* point =chaine->points;
    while(point){
        nbPoint++;
        point = point->suiv;
    }
    return nbPoint;

}
int comptePointsTotal( Chaines *C){
    // Retourne le nombre total de points dans les chaines de 'C'
    if (!C) return 0;
    CellChaine *chaine = C->chaines;
    int nbPoint=0;
    
    while( chaine){
        nbPoint += comptePointsChaine(chaine);
        chaine = chaine->suiv;
    }

    return nbPoint;
}

Chaines * ChargerChainesFile(char * nomfichier,char * prefix, char * suffix, int mode_silencieux){
    /* Retourne les chaines  contenu dans le fichier  'prefix'+'nomfichier'+'suffix' et retourne NULL en cas d'erreur
    si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier ou avec les chaines qu'il contient il vous seras demander si vous voulez redefinir le nom
    si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon ou que les chaines du fichier ne sont pas bonnes, l'ecriture seras abandonne
    si 'nomfichier' est NULL il vous sera demande d'entre le nom du fichier (meme si 'mode_silencieux' est vrai)
    'prefix' et 'suffix' peuvent etre NULL */

    FILE* fic = NULL;
    char** P_nomfichier = &nomfichier;

    // si 'nomfichier' est defini on le reecrit dans un espace memoire allouer dynamiquement pour pouvoir le passé a la fonction (Open_read_only)
    if (nomfichier){
        char * copy_nomfichier;
        P_nomfichier = & copy_nomfichier;
        if (!(*P_nomfichier = strdup(nomfichier))){
            fprintf(stderr,"Erreur strdup\nImpossible de copier le chemin d'acces au fichier source %s\n",nomfichier);
            printf("Chargement des chaines abandonee\n");
            return NULL;
        }
        nomfichier = copy_nomfichier;
        
    }
    // on utilise (Open_read_only) pour avoir le flux correspondant au nom de fichier
    fic = Open_read_only(P_nomfichier,prefix,suffix,0 , mode_silencieux);
    if (!fic){
        free(*P_nomfichier);
        printf("Chargement des chaines abandonee\n");
        return NULL;
    }
    Chaines* chaines=NULL;
    // on essaye de lire les chaines du fichier
    while(!chaines){
        // si la lecture fonctionne  on arrete la boucle
        if ((chaines = lectureChaines(fic)) || mode_silencieux)
            break;

        if (fclose(fic))
            printf("Attention le fichier '%s' ne s'est pas referme correctement\n",*P_nomfichier);
        

        // si la lecture de la chaine n'a pas fonctionne et qu'on est en mode silencieux on arrete la fonction
        // si la lecture de la chaine n'a pas fonctionne et qu'on est pas en mode silencieux, on demande a l'utilisateur si il veut definir un autre nom de fichier
        if ( mode_silencieux  || !(fic = Open_read_only(P_nomfichier,prefix,suffix,1 , mode_silencieux))){
            free(*P_nomfichier);
            printf("Chargement des chaines abandonee\n");
            return NULL;
        }
        // si l'utilisiteur a defini un autre nom de fichier et que le flux a pu etre ouvert, on reesaye de lire les chaines
        

    }
    
    if (fclose(fic))
            printf("Attention le fichier '%s' ne s'est pas referme correctement\n",*P_nomfichier);
    free(*P_nomfichier);
    printf("Ensemble de %d chaines chargees\n",chaines->nbChaines);
    return chaines;
}

void EcrituresChainesFile(char * nomfichier, Chaines* chaines , int mode_silencieux){
    //Ecrit des chaines 'chaines' dans le fichier 'nomfichier'
    //si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
    //si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
    //si 'mode_silencieux' est à 2, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
    //si 'nomfichier' est NULL il vous sera demande d'entre le nom du fichier (meme si 'mode_silencieux' est vrai)
    EcrituresFile(nomfichier,ecrireChaines,chaines,mode_silencieux);
}

void EcritureChainesSVG(char * nomfichier, Chaines *chaines , int mode_silencieux){
    //Creer un fichier SVG en html de nom 'nomfichier' pour visualiser les chaines 'chaines'
    //si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
    //si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
    //si 'mode_silencieux' est à 2, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2.html
    //si 'nomfichier' est NULL il vous sera demande d'entre le nom du fichier (meme si 'mode_silencieux' est vrai)
    EcritureSVG(nomfichier,afficheChainesSVG,chaines,mode_silencieux);
}

FILE* generationFilealeatoire(char* nomfichier ,char *prefix,char*suffix,int nbChaines,int nbPointsChaine,int xmax,int ymax,int mode_silencieux){
    /* Retourne le flux d'un fichier generer avec 'nbChaines' chaines, 'nbPointsChaine' points et des coordonnees de point qui oscillent entre ( 0,xmax) pour x et  (0,ymax) pour y
       Retourne NULL en cas d'erreur
       'prefix' et 'suffix' peuvent etre NULL
       si 'nomfichier' est non NULL 
           le nom du fichier sauvegarde sera 'prefix'+'nomfichier'+'suffix'
           si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
           si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
           si 'mode_silencieux' est à 2, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
       si 'nomfichier' est NULL un fichier temporaire seras generer (qui sera supprimé une fois tout ses flux fermé) */
       

    FILE *fic = NULL;
    // si 'nomfichier' est defini on le reecrit dans un espace memoire allouer dynamiquement pour pouvoir le passé a la fonction (Open_read_only)
    if (nomfichier){
        if (!(nomfichier = strdup(nomfichier))){
            fprintf(stderr,"Erreur strdup\nImpossible de copier le chemin d'acces au fichier de destination %s\n",nomfichier);
            return NULL;
        }
        fic = Open_write_only(&nomfichier,prefix,suffix,mode_silencieux);
        free(nomfichier);
    }
    // si 'nomfichier' n'est pas defini on ouvre un fichier temporaire
    else fic = tmpfile();
    if (!fic){
        return NULL;
    }
    // on ecrit le nombre de chaines et gamma
    fprintf(fic,"NbChain : %d\nGamma : %d\n\n",nbChaines,4);

    // on ecrit 'nbChaines' chaines
    for (int i=0; i<nbChaines; i++){
        // on ecrit numero de chaine et le nombre de points de la chaine
        fprintf(fic,"%d %d",i,nbPointsChaine);
        // on ecrit les points de la chaine
        for(int x = 0; x<nbPointsChaine;x++){
            double x= (double)rand()*xmax/(RAND_MAX) ; double y= (double)rand()*ymax/RAND_MAX ;
            fprintf(fic," %.9g %.9g",x,y);
        }
        fprintf(fic,"\n");
    }
    return fic;
}

Chaines* generationAleatoire(int nbChaines,int nbPointsChaine,int xmax,int ymax){
    /* Retournes un ensemble de chaines genere aleatoirement avec 'nbChaines' chaines, qui ont chacune 'nbPointsChaine' points de coordonnees oscillant entre ( 0,xmax) pour x et  (0,ymax) pour y
       Retourne NULL en cas d'erreur */

    if ( nbChaines < 0|| nbPointsChaine < 0|| xmax < 0|| ymax < 0){
        fprintf(stderr,"Tout les arguments doivent etre positif\n");
        printf("Generation abandonnee\n");
        return NULL;
    }
    Chaines* EnsembleChaines =  (Chaines *) calloc( 1 ,sizeof(Chaines)); // on alloue l'espace nécessaire
    if (!EnsembleChaines){
        fprintf(stderr,"Erreur lors de l'allocation de l'espace pour 'Chaines'\n");
        printf("Generation abandonnee\n");
        return NULL;
    }
    // on declare les differement element de la struture dont on va avoir besoin
    CellPoint *point;
    CellChaine *chaine;
    // On initialise la stucture
    EnsembleChaines->nbChaines = nbChaines;
    EnsembleChaines->gamma = 3; // gamma est choisi arbitrairement
    
    // on fait un boucle sur Nbchaines pour creer les chaines
    for( int i = 0; i< nbChaines;i++){
        chaine= (CellChaine*) calloc(1,sizeof(CellChaine));
        if (!chaine){
            fprintf(stderr,"Erreur lors de l'allocation de l'espace pour une chaine\n");
            printf("Generation abandonnee\n");
            libereChaines(EnsembleChaines);
            return NULL;
        }
        
        chaine->numero = i; // on initalise l'attribut numero de CellChaine
        // on fait un insertion en tete de chaine
        chaine->suiv = EnsembleChaines->chaines;
        EnsembleChaines->chaines = chaine;
        
        // on fait un boucle sur nbPointsChaine pour creer les points
        for (int j=0; j<nbPointsChaine;j++){

            point= (CellPoint*) calloc( 1 ,sizeof(CellPoint));
            if (!point){
            fprintf(stderr,"Erreur lors de l'allocation de l'espace pour un point\n");
            printf("Generation abandonnee\n");
            libereChaines(EnsembleChaines);
            return NULL;
            }
            // On initialise aleatoirement les coordonnees du point
            point->x  = (double)rand()*xmax/(RAND_MAX);
            point->y  = (double)rand()*ymax/RAND_MAX;

            // on fait un insertion en tete de point
            point->suiv = chaine->points;
            chaine->points = point;
        
        }
        

    }
    return EnsembleChaines;


}
