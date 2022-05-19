# include "../headers/Chaine.h"
# include "../headers/OutilT.h"

/*
Permet de charger des chaines, les sauvegarder dans le format normal, les sauvegarder au format svg, les afficher dans la sortie standart
    si l'option '-f' est mises en debut d'appel, les fichiers passer en commande ecraserons les fichier present si il y'a colision
    si l'option '-q' est mises en debut d'appel, des suffixes seront ajouté aux fichiers ayant les meme noms que des fichiers deja existant
    les option '-q' et '-f' empeche les interecations avec l'utilisateur lors de l'analyse donc en cas d'erreur avec une chaine ou un fichier qui ne peut pas s'ouvrir ect.. le fichier sera juste abandonne et le suivant sera analysé
    si l'option  '-q' et '-f' sont tout les 2 passés en debut de commandes ensembles, seul l'option '-f' sera retenu

    si l'option '-m' est mises en debut d'appel, le menu de sera pas ouvert apres l'analyse des arguments de l'appel donc le programme se terminera directement apres
    L'interpretation ce fait comme suit:
                        on met d'abord les option voulu
                        chaques nom de fichier contenant une instance de Chaines doit etre sepraré par 2 tirets '--'
                        on met d'abord le nom du ficher contenant les instances, puis les noms des fichiers ou sauvegarder les insances puis SVG pour indiqué qu'on veut passer a l'enregistrement au format svg puis les noms des fichiers ou sauvegarde au format SVG
                    

    exemple d'appel:

            > ./bin/ChaineMain -f 'source1' 'dest1'
                active l'option '-f', sauvegarde les chaines de source1 dans dest1 et le menu s'ouvrira a l'issus de l'interpretation de la commande avec les chaine de source1

            > ./bin/ChaineMain  -m 'source1' 'svg' 'dest1'
                active l'option '-m', sauvegarde les chaines de source1 dans dest1 au format svg et le programme se termine a l'issus de l'interpretation de l'appel

            > ./bin/ChaineMain 'source1' 'dest1' 'dest2' svg' 'dest3'
                    va sauvegarder les chaines de source1 dans dest1 et dest2 au meme format que source1 et va sauvegarder les chaines de source1 dans dest3 au format svg et le menu va s'ouvrir avec les chaines de source1

            >./bin/ChaineMain -m -q 'source1' 'svg' 'dest1' -- 'source2' 'stdout' -- 'source3' stdout
                    active les options '-m' et 'q', puis va sauvegarder les chaines de source1 dans dest1 au format svg , va afficher les chaines de source2 dans la sortie standard et va afficher les chaines de source3 dans la sortie standard et n'ouvrira pas le menu 
            
            > ./bin/ChaineMain 
                    ouvrir directement le menu */




void appel_ChargerChainesFile(Chaines ** P_chaines){
    // Appelle la fonction ChargerChainesFile et si elle reussi  'P_chaines' pointe vers la nouvelle liste de chaines charge
    Chaines * chaines;
    if (!(chaines = ChargerChainesFile(NULL,NULL,NULL,0))) return;
    libereChaines(*P_chaines);
    *P_chaines = chaines;
    
}
void appel_EcrituresChainesFile(Chaines ** P_chaines){
    // Fait l'appelle EcrituresChainesFile(NULL,*P_chaines,0)
    EcrituresChainesFile(NULL,*P_chaines,0);
}

void appel_EcritureChainesSVG(Chaines ** P_chaines){
    // Fait l'appelle EcritureChainesSVG(NULL,* P_chaines,0);
    EcritureChainesSVG(NULL,* P_chaines,0);
}

void appel_nbPoints (Chaines ** P_chaines){
    // Affiche dans la sortie standard le nombre de points de total de la liste de chaine pointe par P_chaines
    printf("Il y a %d points\n",comptePointsTotal(*P_chaines));
}

void appel_longueurTotal(Chaines ** P_chaines){
    // Affiche dans la sortie standard la longueur totale la liste de chaine pointe par P_chaines
    printf("La longueur total est : %lf\n",longueurTotale(*P_chaines));
}
Chaines * liste_chaines(int argc, char *argv[], int debut,int mode_silencieux ){
    /* Interprete la ligne de commande de l'appelle a ChaineMain et Retourne la liste de chaines lu
        'argc' est la nombre d'element de 'argv' 
        'debut' est a quel case de 'argv' on doit commencer  l'analyse
        si 'mode_silencieux' est à 0
                        lors de la lecture: en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
                        lors de la l'ecriture: en cas de de probleme avec le nom de fichier il vous seras demander si vous voulez modifier le nom, et si un fichier existe deja avec le meme nom, il vous sera demandé si vous voulez l'ecraser
        si 'mode_silencieux' est à 1
                        lors de la lecture: si le nom du fichier n'est pas bon ou que les chaines du fichier ne sont pas bonnes, l'ecriture seras abandonne
                        lors de la l'ecriture: si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , il sera ecrasé

        si 'mode_silencieux' est à 2 ou plus
                        lors de la lecture: pareil que pour 1
                        lors de la l'ecriture: si le nom du fichier n'est pas bon, l'ecriture seras abandonne, et si un fichier porte deja le meme nom que 'nomfichier' , un suffix seras ajouter au nom pour eviter d'ecraser le fichier existant, exemple 'nomfichier'_2
        
        L'interpretation ce fait comme suit:
                        chaques nom de fichier contenant une instance de Chaines doit etre sepraré par 2 tirets '--'
                        on met d'abord le nom du ficher contenant les instances, puis les noms des fichiers ou sauvegarder les insances puis SVG pour indiqué qu'on veut passer a l'enregistrement au format svg puis les noms des fichiers ou sauvegarde au format SVG
                        exemple:
                                'source1' 'dest1'
                                va sauvegarder les chaines de source1 dans dest1 et les chaines de source1 seront rendus

                                'source1' 'svg' 'dest1'
                                va sauvegarder les chaines de source1 dans dest1 au format svg et les chaines de source1 seront rendus

                                'source1' 'dest1' svg' 'dest2'
                                va sauvegarder les chaines de source1 dans dest1 au meme format que source1 et va sauvegarder les chaines de source1 dans dest2 au format svg et les chaines de source1 seront rendus

                                'source1' 'stdout' -- 'source2'
                                va afficher les chaines de source1 dans la sortie standard et les chaines de source2 seront rendus

                                'source1' 'svg' 'dest1' -- 'source2' 'stdout' -- 'source3' stdout
                                va sauvegarder les chaines de source1 dans dest1 au format svg , va afficher les chaines de source2 dans la sortie standard et va afficher les chaines de source3 dans la sortie standard , les chaines de source3 seront rendus

        */
    char *low;
    Chaines* chaines = NULL;
    int i = debut;
    int etape = 0;
    Chaines * chaines2;
    for (; i<argc;i++){
        if (!strcmp(argv[i],"--")){ // si on a un double tiret c'est qu'on passe a une nouvelle chaine
            etape = 0;
            continue;
        }
        switch (etape)
        {
        case 0: // on cherche le fichier source et on charge les chaines
            if((chaines2 = ChargerChainesFile(argv [i],NULL,NULL,mode_silencieux))){
                libereChaines(chaines);
                chaines = chaines2;
                etape = 1;
                continue;
            }
            etape=3;
            continue;
        case 1:// soit ecriture des chaines dans un fichier ou debut des noms de fichiers pour enregistrement svg
            if(!(low = lowerOf(argv[i]))){
                libereChaines(chaines);
                return NULL;
            }
            if (!strcmp(low,"svg")){// si on veut enregistrer une representation graphique des chaines au format SVG
                etape = 2;
                free(low);
                continue;
            }
            free(low);
            EcrituresChainesFile(argv[i], chaines , mode_silencieux);
            continue;

        case 2:// enregistrement des fichier svg
            EcritureChainesSVG(argv[i],chaines , mode_silencieux);
            continue;

        
        case 3:// Si il y a eu une erreur, on zappe jusqu'au prochain fichier source
            continue;
        }
    }
    return chaines;
    
}

int main(int argc, char *argv[]){
    srand( time( NULL ) ); // la fonction d'affichage svg utilise rand() donc on doit initialiser le générateur de nombres aléatoires
    Chaines* chaines = NULL;
    int ouvrir_menu = 1;
    int mode_silencieux = 0;
    if (argc==1) chaines = ChargerChainesFile(NULL,NULL,NULL,0); // si il n'y a pas d'argument on demande directement a l'utilisateur de donner un fichier contenant des chaines
    else {// si il y a des arguments, on regarde d'abord si il y a des options
        int x = 1;
        for (;x<argc;x++){
            if (!strcmp(argv[x],"-f")){
                mode_silencieux = 1;
            }
            else if (!strcmp(argv[x],"-q")){ 
                if(!mode_silencieux) mode_silencieux=2;
            }
            else if(!strcmp(argv[x],"-m")){
                ouvrir_menu = 0;
            }
            else break;
        }// puis on analyse le reste des arguments
        chaines = liste_chaines ( argc, argv ,x,mode_silencieux);
    }
    if (!chaines || !ouvrir_menu){// si aucune chaine n'est charge ou que l'option -m a ete mise, arrete l'execution la
        libereChaines(chaines);
        printf("Fin programme\n");
        return 0;
    }
    
    
    
    int choix;
    // tableau des fonction disponible dans le menu
    void (*tabFunc[])(Chaines **) = {appel_ChargerChainesFile,appel_EcrituresChainesFile,appel_EcritureChainesSVG,appel_longueurTotal,appel_nbPoints};

    // affichage du menu
    char affichage[]={"\nAction disponible:\n\n0-sortie du programme\n1-Charger un nouvelle ensemble de chaines\n2-Ecrire les chaines au meme format que la source\n3-Enregistrer une representation graphique des chaines au format SVG\n4-Afficher la longueur Totale des chaines\n5-Afficher le nombre total de points dans les chaines\n\nQue voulez vous faire?\n"};
    int essai=0;
    // on lance le menu tant que l'utilisateur de choisi pas 0 qui correspond a la sortie du programme
    while (( choix = menu(&essai,5,0,5,0,affichage,1) )){
        tabFunc[ choix - 1 ](&chaines);
        essai = 0; 
    }
    
    libereChaines(chaines);
    printf("\nFin programme\n");
    return 0;
    
    
}

