# include "../headers/OutilT.h"

char * concatenate( char**  partie1, const char * partie2 ){
    /*  Rajoute a 'partie1' le contenu de 'partie' en fin de chaine
       'partie1' doit etre un pointeur vers une chaine de caractere allouer dynamiquement
        la taille de l'espace memoir reserve pour 'partie1' sera modifie a len('partie1') + len('partie2') +1
        Retourne un pointeur vers le premier char du resulatat de la concatenation si celle si reussi et NULL sinon */
    if (partie2==NULL) return *partie1;
    char * temp;
    if (!( temp = (char*) realloc(*partie1,(strlen(partie2)+strlen( *partie1)+1)*sizeof(char) ))){
            fprintf(stderr,"Erreur realloc concatenation de \"%s\" avec \"%s\"\n",*partie1,partie2);
            return NULL;
        }
    *partie1 = temp ;
    
    return strcat(*partie1,partie2);
    

}
char * r_concatenate( char**  partie2, const char * partie1 ){
    /*  Rajoute a 'partie2' le contenu de 'partie1' en debut de chaine
        'partie1' doit etre un pointeur vers une chaine de caractere allouer dynamiquement
        la taille de l'espace memoir reserve pour 'partie2' sera modifie a len('partie1') + len('partie2') +1
        Retourne un pointeur vers le premier char du resulatat de la concatenation si celle si reussi et NULL sinon */
    if (partie1==NULL) return *partie2;
    char * temp;
    int lenp1 =strlen( partie1);
    int lenp2 =strlen(*partie2);
    if (!( temp = (char*) malloc((lenp1+lenp2+1)*sizeof(char) ))){
            fprintf(stderr,"Erreur realloc concatenation de \"%s\" avec \"%s\"\n",partie1,*partie2);
            return NULL;
        }
    // on met le partie 1 dans le nouvelle espace memoire
    memcpy(temp,partie1,lenp1);
    // puis on met la partie 2 en fin
    memcpy(temp+lenp1,*partie2,lenp2+1);
    free(*partie2);
    *partie2 = temp ;
    
    return *partie2;
    

}
void toLower(char * src){
    //Met en minisucule la chaine de caractere (src) passe en parametre
    while( (*src = tolower( *src ) )){
        src++;
    }
}
char *lowerOf(char * src){
    //Retourne une copie  en minisucule la chaine de caractere (src) passe en parametre (src n'est pas modifie)
    // Attention a ne pas oublie de liberer la copie apres son utilisation
    char * res = (char *) malloc(strlen(src)+1);
    if (!res){
        fprintf(stderr,"Erreur malloc lowerOf \"%s\"\n",src);
        return NULL;
    }
    for (int i = 0 ; ( res[i] = tolower( src[i] ) ) ; i++);

    return res;
}
void  vide_tampon(){
    //permet de vider le tampon apres une saisie
    char c = getchar () ;
    while (c != '\n' && c != EOF ) {
        c = getchar () ;
    }
}
int getInt(void){
    /* Permet de recuperer un entier  depuis l'entree standard
    La fonction retourne la valeur 0 si l'entree ne contient pas une représentation de valeur numérique. Du coup, il n'est pas possible de distinguer l'entree "0" d'une chaîne ne contenant pas un nombre entier.*/
    char * entree= NULL;

    if (!(entree=recup_str()))  return 0;

    int c = atoi(entree);

    free (entree);
    return c;
    
}

long getLong(int *etat){
    /* Permet de recuperer un entier  depuis l'entree standard
        La fonction met le pointeur 'etat' à
                -2 si l'entree n'a pas pu etre interpretée
                -1 si aucun entier n'a été trouvée dans l'entree avant un caractere qui n'est pas espace
                 0 si l'entree est composée uniquement d'un entier 
                 1 si l'entree contient d'abord un entier puis autre que des espaces
       'etat' n'est pas obligatoire, il peut etre mis a NULL */
    
    // d'abord on recupere l'entree de l'utilisateur
    char * entree= NULL;
    if (!(entree=recup_str())){
        if (etat)  
            *etat=-2;
        return 0;
    }

    // si etat n'est pas defini, on retourne directement la conversion de l'entree en long
    long res;
    if (!etat){
        res = strtol(entree,NULL,10);
        free(entree);
        return res;

    } 

    // on recupere l'entree de l'utilisateur
    char * end;
    res = strtol(entree,&end,10);
    // puis on analyse 'end' pour savoir de quel cas il s'agis

    if (end == entree){ //aucun entier n'a été trouvée dans l'entree avant un caractere qui n'est pas espace
        *etat=-1;
        free(entree);
        return 0;
    }
    if (*end=='\0'){ //l'entree est composée uniquement d'un entier
        *etat=0;
        free(entree);
        return res;
    }
    while (*end !='\0'){
        if (!isspace(*end)){//l'entree contient d'abord un entier puis autre que des espaces
            *etat=1;
            free(entree);
            return res;
        }
        end++;
    }
    *etat=0;//l'entree contient d'abord un entier des espaces
    free(entree);
    return res;
    
}

long getLongbeetween(int *etat, long valMin, long valMax, long* DefaultValue){
    /* Retourne un long entre par l'utilisateur entre 'valMin' et 'valMax' compris
        si 'DefaultValue' est non NULL et que l'entree n'est pas un entier dans ['valMin' ,'valMax' ] ou que l'entree a ete mal faite, 'DefaultValue' est retourne 
        La fonction met le pointeur 'etat' à
                -2 si l'entree n'a pas pu etre interpretée
                -1 si aucun entier n'a été trouvée dans l'entree avant un caractere qui n'est pas espace
                 0 si l'entree est composée uniquement d'un entier 
                 1 si l'entree contient d'abord un entier puis autre que des espaces
                 2 si l'entree n'est pas dans ['valMin' ,'valMax' ]
       'etat' n'est obligatoire, il  peut etre entree a  NULL */
    
    // on recupere l'entree
    long choix = getLong(etat);
    // on analyse la valeur de etat
    // si etat est NULL on retourne la valeur obtenue si elle est dans l'intervale ['valMin' ,'valMax' ] ou 'DefaultValue' si il est defini
    if (!etat) return (choix<valMin || choix >valMax) && DefaultValue ? *DefaultValue : choix;

    // si la valeur retourne est dans ['valMin' ,'valMax' ] et que l'entree a ete bien faite on retourne l'entree
    if (!*etat && choix>=valMin && choix <=valMax)
        return choix;

    // si l'entree a ete bien faite mais que la valeur n'est pas dans ['valMin' ,'valMax' ]  on met etat a 2
    if (!*etat) *etat=2;
    
    // on retourne 'DefaultValue' si il est defini et l'entree sinon
    return  DefaultValue ? *DefaultValue : choix;
}

long menu(int * essai, int essaiMax,long valMin, long valMax,long DefaultValue, char * affichage, int failIndication){
    /* Affiche 'affichage' a l'utilisateur et lui demande de rentre une valeur entre 'valMin' et 'valMax' compris
        l'utilisateur a 'essaiMax' - *'essai' restant pour rentrer une bonne valeur, si essai est NULL l'utilisateur n'a qu'un essai
        si au bout des essai l'entree de l'utilisateur n'est toujours pas bonne, 'DefaultValue' est retournee
        si 'failIndication' est vrai et que l'utisateur a fait une mauvaise entree, une indication sur l'erreur lui sera affiche
        affichage et essai peuvent etre NULL */

    // on affiche l'affichage
    if (affichage)
        printf("%s",affichage);
    int etat;
    long choix;
    // on recupere l'entree de l'utilisateur
    choix = getLongbeetween(&etat,valMin,valMax,&DefaultValue);
    if (!etat) // si etat est NULL c'est que l'entree est valide donc on peut la retournee
        return choix;
    
    if (!essai) // si l'entre n'est pas valide mais que essai est NULL, on retourne la valeur par default
        return DefaultValue;
    if ((++(*essai)) >= essaiMax) // on incremente le nombre d'essai effectue et on verifie si il en reste encore, si il n'en reste plus on retourne la valeur par default
        return DefaultValue;
    if (failIndication){ // si fail indication est vrai , on indique a l'utilisateur le probleme avec son entree
        if (etat<0){
            printf("L'entree n'a pas pu etre interpretee ");
        }
        else{
            printf("Votre entree doit etre un entier compris entre %ld et %ld selon votre choix. ",valMin,valMax);
        }
    }
    // et on rappel (menu) pour l'essai suivant
    printf("Veuillez reesayer.\n");
    return menu(essai,essaiMax,valMin,valMax,DefaultValue,affichage,failIndication);
        

}

char * recup_str(){
    // Retourne une chaine de caractere entrer depuis l'entree standard
    // Retourne NULL en cas d'erreur
    // la chaine retourne doit etre liberer apres son utilisation

    // on alloue l'espace pour recuper la chaine
    char * str = (char*) malloc(20*sizeof(char));
    if (!str){
        fprintf(stderr,"Erreur malloc recup_str\n");
        return NULL;
    }
    char c ;
    int cpt=0;
    int nb_malloc = 1;
    char * temp;
    // on evalue les caratere entree un par un
    while ((c= getchar ()  ) != '\n' && c != EOF ){
        str[cpt]=c;
        cpt++;
        // on verifie si il y'a encore assez d'espace allouer sinon on augment l'espace de 20 unités
        if(cpt%20==0){
            nb_malloc++;
            temp =(char*) realloc(str,nb_malloc*20*sizeof(char));
            if (!str){
                fprintf(stderr,"Erreur realloc recup_str\n");
                free(str);
                printf("\n");
                return NULL;
            }
            str = temp;
        }

    }
    // on place le caractere final et on retourne la chaine
    str[cpt]='\0';
    printf("\n");
    return str;
    

}
int yes_no(){
    // Fonction permettant de recuperer la reponse d'une question posé a l'utilisateur ou il doit repondre par y ou n
    // les reponses oui, o, yes, non, n, no, sont aussi interpreté par la fonction peu importe la casse
    // les reponses posives y,yes,o,oui rendent 1
    // les reponses negatives n,no,non rendent -1 
    // si apres 5 tentatives l'utilisateur ne donne pas une reponse interpretable la fonction rend 0
    // en cas d'erreur du fgets on rend 2
    char buffer [5];
    
    for (int i=0;i<5; i++){// si au bout de 5 essais l'utilisateur ne saisi pas correctement la reponse on arrete le programme
        buffer[4]='a';
        if (i>0)
            printf("\nImpossible d'interpreter cette entree, Veuillez repondre par 'y' ou 'n'\n");
        if (!fgets(buffer,5,stdin)){
            printf ("Impossible de lire l'entree\n");
            printf("\n");
            return 2 ;
        }
        if (buffer[4]=='\0' && buffer[3]!='\n'){// si on entre dans ce if c'est que trop de caracteres ont ete entree
            vide_tampon();
            printf("\n");
            continue;
        }   
        // on remplace les '\n' par des '\0' si il y'en a
        else if (buffer[1]=='\n')
            buffer[1]='\0';
        else if (buffer[2]=='\n')
            buffer[2]='\0';
        else if (buffer[3]=='\n')
            buffer[3]='\0';
        
        else printf("\n");
        toLower(buffer);// met le buffer en minuscule pour permettre de faire une verification peu importe la casse
        // on analyse ce qu'il y a dans le buffer
        if (buffer[1]=='\0'){// si l'entre est seulement une lettre, on la compare aux reponses possibles
            if (buffer[0]=='y' || buffer[0]=='o')
                return 1;

            if (buffer[0]=='n')
                return -1;
        }
        else if(buffer[3]=='\0'){// si l'entre est composer de plusieurs caractere, on compare aux reponses possibles
            if (! (strcmp(buffer,"yes") && strcmp(buffer,"oui")) )
                return 1;
            if (! (strcmp(buffer,"no") && strcmp(buffer,"non")) )
                return -1;
        }

    }// on arrive au bout des 5 essai donc on retourne 0
    printf ("Impossible d'interpreter les entrees obtenues\n");
    return 0;
}



FILE * Open_read_only(char ** P_nomfichier,char* prefix,char* suffix ,int redef_chemin , int mode_silencieux){
    /* Retourne un flux de lecture correspondant a 'prefix'+ *'P_nomfichier' + 'suffix'
       Retourn NULL si aucun flux n'a pu etre ouvert
       si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez redefinir le nom
       si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne
       si 'P_nomfichier' pointe vers un NULL il vous sera demande d'entrer le nom du fichier (meme si 'mode_silencieux' est vrai)
       si 'P_nomfichier' ne pointe pas vers un NULL, Il doit obligatoirement pointer vers un espace memoire allouer dynamiquement
       'prefix' et 'suffix' peuvent etre NULL
       si 'redef_chemin' est vrai  on considerera que une tentative d'ouverture a deja ete effectue et on demande directement a l'utilisateur de donner un nouveau nom de fichier
       'redef_chemin' vrai annule le mode_silencieux
       Il faudra liberer la valeurs pointer par P_nomfichier apres la fin de son utilisation  */

    if (!P_nomfichier) return NULL;


    FILE* fic = NULL;
    if (!redef_chemin){
        if (!*P_nomfichier){ // si le nom du fichier n'a pas encore ete defini, on le demande
            printf("Entrez le chemin d'acces au fichier source : ");
            if (!(*P_nomfichier = recup_str())){
                fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier source\n");
                return NULL;
            }
        }
        if (prefix){ // si prefix est defini, on l'ajoute au nom du fichier
            if (!r_concatenate(P_nomfichier, prefix)){
                fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier source\n");
                free(*P_nomfichier) ; P_nomfichier =NULL;
                return NULL;
            }
        }
        if (suffix){ // si suffix est defini, on l'ajoute au nom du fichier 
            if (!concatenate(P_nomfichier, suffix)){
                fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier source\n");
                free(*P_nomfichier) ; P_nomfichier =NULL;
                return NULL;
            }
        }
        // on tente d'ouvrir le fichier
        fic = fopen ( *P_nomfichier , "r" );
        if (mode_silencieux){ // si le mode silencieux est active , ne s'occupe pas de la reussite ou non de l'ouverture
            if (!fic) fprintf(stderr,"Probleme lors de l'ouverture du ficher '%s'%s\n",*P_nomfichier,(errno==2 ? ", veuillez verifier le nom du fichier ainsi que son chemein d'acces\n":strerror(errno)));
            return fic;
        }
    }
    
    // on fait un boucle jusqu'a qu'on obtienne un flux
    while(!fic){
        if(!redef_chemin)
            fprintf(stderr,"Probleme lors de l'ouverture du ficher '%s'%s\n",*P_nomfichier,(errno==2 ? ", veuillez verifier le nom du fichier ainsi que son chemein d'acces\n":strerror(errno)));
        redef_chemin = 0;
        // on rappele le chemin precedement entrer et on demande si l'utilisateur veut definir un nouveau chemin
        printf("Vous avez specifie le chemin d'acces suivant :'%s' Voulez vous en specifier un nouveau? [Y/N]\n",*P_nomfichier);
    
        free(*P_nomfichier);
        *P_nomfichier =NULL;

        switch (yes_no()){

            case 1:// si oui il un nouvelle entree est demande
                printf("Entrez le chemin d'acces au fichier source : ");
                if (!(*P_nomfichier = recup_str())){
                    fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier source\n");
                    return NULL;
                }
                if (prefix){ // on ajoute le prefix
                    if (!r_concatenate(P_nomfichier, prefix)){
                        fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier source\n");
                        free(*P_nomfichier) ; P_nomfichier =NULL;
                        return NULL;
                    }
                }
                if (suffix){ // on ajoute le suffix
                    if (!concatenate(P_nomfichier, suffix)){
                        fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier source\n");
                        free(*P_nomfichier) ; P_nomfichier =NULL;
                        return NULL;
                    }
                }
                errno=0;// on retente une ouverture
                fic = fopen ( *P_nomfichier , "r" );
                break;

            default: //l'utilisateur ne veut pas redefinir de chemin ou bien sa reponse n'est pas interpretable
                return NULL;
                break;
        }
    }
    return fic;
}
    

FILE * Open_write_only(char ** P_nomfichier,char *prefix,char * suffix,int mode_silencieux){
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


    if (mode_silencieux<0){
        fprintf(stderr,"'mode_silencieux' doit etre superieur ou egal a 0\n");
        return NULL;
    }
    if (!P_nomfichier) return NULL;
    FILE* fic = NULL;
    char buffer[11];// permettra si le mode silencieux 2 ou plus est choisi d'ajouter un suffix si le fichier existe deja
    int len;
    if (!*P_nomfichier){ // si le nom du fichier n'a pas encore est ete defini, on le demande
        printf("Entrez le chemin d'acces au fichier de destination (ou 'stdout' pour la sortie standard): ");
        if (!(*P_nomfichier = recup_str())){
            fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier de destination\n");
            return NULL;
        }
    }
    char * low = lowerOf(*P_nomfichier);
    if(!low){ 
        free(*P_nomfichier); P_nomfichier =NULL;
        return NULL;    
    }
    if (!strcmp(low,"stdout")){// si on veut que ce soit envoye plutot vers la sortie standart
        free(low);
        return stdout;
    }
    free(low);
    if (prefix){ // on ajoute le suffix si suffix est non NULL
        if (!r_concatenate(P_nomfichier, prefix)){
            fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier de destination\n");
            free(*P_nomfichier) ; P_nomfichier =NULL;
            return NULL;
        }
    }
    //
    if (mode_silencieux>1) len = strlen(*P_nomfichier); // si mode_silencieux est a 2 ou plus on retient la taille de 'prefix'+'*P_nomfichier'  au cas ou il faudra ajouter un suffix si le nom est deja utilise exemple : 'prefix''*P_nomfichier'"_2"'suffix'
    if (suffix){// puis on ajoute le suffix si il est defini
        if (!concatenate(P_nomfichier, suffix)){
            fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier de destination\n");
            free(*P_nomfichier) ; P_nomfichier =NULL;
            return NULL;
        }
    }

    struct stat sb;
    while (!fic )
    {
        if( mode_silencieux==1 ||stat(*P_nomfichier,&sb)==-1){  // mode silencieux 1 ou Le fichier n'existe pas
            errno=0;
            if (!(fic =fopen(*P_nomfichier, "w"))){ // on ouvre le fichier en lecture
                fprintf(stderr,"Probleme lors de la creation du ficher '%s'%s\n",*P_nomfichier,(errno==2 ? ", veuillez verifier le nom du fichier ainsi que son chemein d'acces\n":strerror(errno)));
                if (mode_silencieux)   break; // si on est en mode silencieux 1 on ne regarde pas si le fichier a ete ouvert  ou non, on sort directement de la boucle

            }

            else  break;
            
        }
        else{   // Le fichier existe deja
            if (mode_silencieux){ // si le fichier existe deja et que mode_silencieux est superieur ou egal a 2, on ajoute _'chiffre' entre le nom du fichier et son suffix puis on remet son suffix a la fin et on retente de verifier si il y a pas deja de fichier a ce nom
                sprintf(buffer,"_%d",mode_silencieux);
                if (suffix){
                    (*P_nomfichier)[len]='\0';
                }
                if (!concatenate(P_nomfichier,buffer) || !concatenate(P_nomfichier,suffix)){// ajoue de _'chiffre' puis du suffix
                    fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier de destination\n");
                    free(*P_nomfichier) ; P_nomfichier =NULL;
                    return NULL;
        
                }

                mode_silencieux++;
                // et on l'envoi verfier si le nom n'est pas deja pris
                continue;
                
            }
            printf("Le fichier '%s' existe deja%s, voulez vous le remplacer? [Y/N]\n",*P_nomfichier,(sb.st_size==0 ? " mais il est vide":"")); // si on est pas en mode silencieux, on demande a l'utilisateur ce qu'il veut faire
            switch (yes_no()){

                case 1: //oui
                    if (!(fic =fopen(*P_nomfichier, "w"))){
                        fprintf(stderr,"Probleme lors de l'ecrasement du fichier '%s'\n",*P_nomfichier);
                    }
                    break;
                
                case -1:// non
                    break;

                default://reponse non interpretable
                    free(*P_nomfichier); 
                    *P_nomfichier = NULL;                    
                    return NULL;

            }
            if (fic)
                break;
        }

        printf("Voulez vous specifier un nouveau chemin d'accees? [Y/N]\n");
        free(*P_nomfichier);
        *P_nomfichier = NULL;
        switch (yes_no()){

            case 1:// si oui l'utilisateur entre un nouveau nom de fichier
                printf("Entrez le chemin d'acces au fichier de destination (ou 'stdout' pour la sortie standard): ");
                if (!(*P_nomfichier = recup_str())){
                    fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier de destination\n");
                    return NULL;
                }
                // on verifie si il a choisi stdout
                low = lowerOf(*P_nomfichier);
                if(!low){ 
                    free(*P_nomfichier); P_nomfichier =NULL;
                    return NULL;    
                }
                if (!strcmp(low,"stdout")){
                    free(low);
                    fic=stdout;
                    break;
                }
                free(low);
                if (prefix){// on  ajoute le prefix
                    if (!r_concatenate(P_nomfichier, prefix)){
                        fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier de destination\n");
                        free(*P_nomfichier) ; P_nomfichier =NULL;
                        return NULL;
                    }
                }
                if (suffix){// on  ajoute le suffix
                    if (!concatenate(P_nomfichier, suffix)){
                        fprintf(stderr,"Impossible de specifier le chemin d'acces au fichier de destination\n");
                        free(*P_nomfichier) ; P_nomfichier =NULL;
                        return NULL;
                    }
                }

                break;

            default: // non ou reponse non interpretable
                return NULL;
                break;
        }
    }

    return fic;


}


void EcrituresFile(char * nomfichier,void (*fonc_decriture)() ,  void* objet,  int mode_silencieux){
    /* Ecrit dans le fichier 'nomfichier' une instance de 'objet' avec l'aide de la fonction d'ecriture 'fonc_decriture'
        si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
        si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
        si 'mode_silencieux' est à 2 ou plus, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
        si 'nomfichier' est NULL il vous sera demande d'entrer le nom du fichier (meme si 'mode_silencieux' est vrai)
        si 'nomfichier'  a "stdout" (peut importe la casse) l'ecriture sera faite dans la sortie standart */
    
    if (mode_silencieux<0){
        fprintf(stderr,"'mode_silencieux' doit etre superieur ou egal a 0\n");
        return;
    }
    FILE* fic = NULL;
    char** P_nomfichier = &nomfichier;
    if (nomfichier){// si il est defini, on met le nom du fichier dans un espace allouer dynamiquement pour pouvoir utiliser la fonction Open_write_only
        char * copy_nomfichier;
        P_nomfichier = & copy_nomfichier; 
        if (!(*P_nomfichier = strdup(nomfichier))){
            fprintf(stderr,"Erreur strdup\nImpossible de copier le chemin d'acces au fichier de destination %s\n",nomfichier);
            printf("Ecriture abandonee\n");
            return;
        }
        nomfichier = copy_nomfichier;
        
    }
    // on ouvre le fichier
    fic =Open_write_only(P_nomfichier,NULL ,NULL, mode_silencieux);
    if (!fic){
        free(*P_nomfichier);
        printf("Ecriture abandonee\n");
        return;
    }
    // on utilise la fonction d'ecriture pour ecrire dans le fichier
    fonc_decriture(objet,fic);
    // puis on ferme le flux si ce n'est pas la sortie standard
    if (fic!=stdout){
        printf("Fichier %s enregistre\n",*P_nomfichier);
        

        if (fclose(fic)){
            fprintf(stderr,"Attention le fichier '%s' ne s'est pas referme correctement\n",nomfichier);
        }
    }
    free(*P_nomfichier);
}

void EcritureSVG(char * nomfichier, void (*fonc_decriture)() ,void *objet , int mode_silencieux){
    /* Ecrit dans le fichier 'nomfichier' une instance de 'objet' au format SVG avec l'aide de la fonction d'ecriture 'fonc_decriture'
        si 'mode_silencieux' est à 0, en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
        si 'mode_silencieux' est à 1, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé
        si 'mode_silencieux' est à 2 ou plus, si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
        si 'nomfichier' est NULL il vous sera demande d'entrer le nom du fichier (meme si 'mode_silencieux' est vrai) */

    
    if (mode_silencieux<0){
        fprintf(stderr,"'mode_silencieux' doit etre superieur ou egal a 0\n");
        return;
    }
    FILE* fic = NULL;
    char** P_nomfichier = &nomfichier;
    if (nomfichier){// si il est defini, on met le nom du fichier dans un espace allouer dynamiquement pour pouvoir utiliser la fonction Open_write_only
        char * copy_nomfichier;
        P_nomfichier = & copy_nomfichier; 
        if (!(*P_nomfichier = strdup(nomfichier))){
            fprintf(stderr,"Erreur strdup\nImpossible de copier le chemin d'acces au fichier de destination %s\n",nomfichier);
            printf("Ecriture abandonee\n");
            return;
        }
        nomfichier = copy_nomfichier;
        
    }
    // on ouvre le fichier juste pour verifier que 'nomfichier' est bon
    fic = Open_write_only(&nomfichier,NULL,".html" , mode_silencieux);
    
    if (!fic) {
        printf("Ecriture abandonee\n");
        return;
    }
    if (fic == stdout){
        fprintf(stderr,"La representation graphique ne peut pas etre effectuee dans la sortie standart\n");
        free(nomfichier);
        return;
    }
    // on referme le fichier car les fonctions d'ecritutre au format svg ne prennent pas le flux en parametre
    if (fclose(fic)){
            fprintf(stderr,"Attention le fichier '%s' ne s'est pas referme correctement\n",nomfichier);
        }
    // dans (Open_write_only) on a ajouter .html au fichier , mais les fonctions d'ecritutre au format svg le rajoute aussi donc on doit le retirer
    nomfichier [strlen(nomfichier)-5 ]='\0';
    // on utilise la fonction d'ecriture pour ecrire dans le fichier
    fonc_decriture(objet,nomfichier);
    printf("Fichier %s.html enregistre\n",nomfichier);
    free(nomfichier);

}

/*  Retourne l'indice dans un tableau a 1 dimension que aurait [ligne][colonne] dans un tableau a un dimension*/
int getIndex(int ligne, int colonne , int nb_colonne) { return ligne*nb_colonne+colonne; }

/* Retourne le maximum entre 'num1' et 'num2'*/
int max(int num1 , int num2){ return (num1 > num2 ? num1:num2);}

/* Retourne le minimum entre 'num1' et 'num2'*/
int min(int num1 , int num2){ return (num1 < num2 ? num1:num2);}