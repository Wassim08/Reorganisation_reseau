# include "../headers/Reseau.h"
# include "../headers/Hachage.h"
# include "../headers/OutilT.h"
# include "../headers/ArbreQuat.h"
# define fonction_defaut 1 // 0 pour annulation , 1 Liste , 2Table de hachage ,3 Arbre quaternaire

/*
Permet de charger des chaines, reconstituer le reseau de ces chaines avec 3 fonction differentes reconstitueReseauListe, reconstitueReseauHachage , reconstitueReseauArbre, sauvegarder le reseau dans le format standard et sauvegarder le reseau au format svg
    si l'option '-f' est mises en debut d'appel, les fichiers passer en commande ecraserons les fichier present si il y'a colision
    si l'option '-q' est mises en debut d'appel, des suffixes seront ajouté aux fichiers ayant les meme noms que des fichiers deja existant
    les option '-q' et '-f' empeche les interecations avec l'utilisateur lors de l'analyse donc en cas d'erreur avec une chaine ou un fichier qui ne peut pas s'ouvrir ect.. le fichier sera juste abandonne et le suivant sera analysé
    si l'option  '-q' et '-f' sont tout les 2 passés en debut de commandes ensembles, seul l'option '-f' sera retenu

    si l'option '-m' est mises en debut d'appel, le menu de sera pas ouvert apres l'analyse des arguments de l'appel donc le programme se terminera directement apres
    L'interpretation ce fait comme suit:
                        on met d'abord les option voulu
                        chaques nom de fichier contenant une instance de Chaines doit etre sepraré par 2 tirets '--'
                        on met d'abord le nom du ficher contenant les instances, puis le chiffre correspondant a la fonction de reconstitution souhaité 1 reconstitueReseauListe,2 reconstitueReseauHachage ,3 reconstitueReseauArbre
                        si la fonction 2 est choisi, il faut choisir la taille du tableau
                        puis puis les noms des fichiers ou sauvegarder les insances puis SVG pour indiqué qu'on veut passer a l'enregistrement au format svg puis les noms des fichiers ou sauvegarde au format SVG
    
    exemple d'appel:

            > ./bin/ReconstitueReseau >./bin/ChaineMain -m -q 'source1' 3 'svg' 'dest1' -- 'source2' 2 100 'stdout' -- 'source3' 3 stdout
                active les options '-m' et 'q', puis va interpreter les chaines de souce1 avec un arbre puis sauvegarder le reseau issu de source1 dans dest1 au format svg , va interpreter les chaines de source2 avec une table de taille 100  et va  afficher le reseau issu de source2 dans la sortie standard puis va interpeter les chaines de source 3 avec un arbre et va afficher le reseau de source3 dans la sortie standard et n'ouvrira pas le menu 

    
    */



// tableau des differenctes fonctions de reconstitutions
Reseau* (*tabReconsReseau[])() = {reconstitueReseauListe,reconstitueReseauHachage,reconstitueReseauArbre};      
// chaine de demande du choix de la taille de la table
char taille_H [98];


int choix_struct(){
    // Demande a l'utilisateur quel structure il veut utiliser 
    // Retourne le choix de l'utilisateur
    char affichage []= "Quelle structure voulez vous utiliser pour reconstituer le Reseau?\n\n0-Annuler\n1-Liste\n2-Table de hachage\n3-Arbre quaternaire\n\n";
    int essai=0;
    long DefaultValue=fonction_defaut;
    return (int) menu(&essai,5,0,3,DefaultValue,affichage,1);
}

int choixM(long DefaultValue){
    // demande a l'utilisateur la taille de table voulue
    // Retourne le choix de l'utilisateur
    int essai=0;
    return (int) menu(&essai,3,1,MAX_M,DefaultValue,taille_H,1);

}
int liste_chaines(int argc, char *argv[],int debut,Reseau ** P_R,Chaines** P_chaines ,int mode_silencieux){
    /* Interprete la ligne de commande de l'appelle a ReconstitueReseau et retourne l'etape a laquelle s'est arrete la fonction
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
                        on met d'abord le nom du ficher contenant les instances, puis le choix de la structure pour reconstitué le Reseau
                        1 : Liste , 2 : table hachage , 3 : Arbre
                        Si 2 est chosi , il faut faire suivre la taille de table voulu
                        puis les noms des fichiers ou sauvegarder les insances puis SVG pour indiqué qu'on veut passer a l'enregistrement au format svg puis les noms des fichiers ou sauvegarde au format SVG
                        exemple:
                                'source1'  '1'  'dest1'
                                va interpreter les chaines de source1 avec  reconstitueReseauListe et sauvegarder le reseau dans dest1 et va garder en memoire le reseau isssu de source1

                                'source1' '2' '100' 'svg' 'dest1'
                                va interpreter les chaines de source1 avec reconstitueReseauHachage  avec une  table de taille 100 puis sauvegarder le reseau dans dans dest1 au format svg et va garder en memoire le reseau isssu de source1

                                'source1' '3' 'dest1' svg' 'dest2'
                                va interpreter les chaines de source1 avec reconstitueReseauArbre sauvegarder le reseau dans dest1 et va sauvegarder le reseau  de source1 dans dest2 au format svg et va garder en memoire le reseau isssu de source1

                                'source1' 3 'stdout' -- 'source2'
                                va interpreter les chaines de source1 avec reconstitueReseauArbre  puis va afficher le reseau de source1 dans la sortie standard et les chaines de source2 seront gardé en memoire

                                'source1' 1 'svg' 'dest1' -- 'source2' '2' '10' 'stdout' -- 'source3' 
                                va interpreter les chaines de source1 avec  reconstitueReseauListe ,  sauvegarder le reseau de source1 dans dest1 au format svg , va interpreter les chaines de source2 avec reconstitueReseauHachage avec une table de taille 10 puis va afficher le reseau dans la sortie standard et va garder en memoire les chaines de source 3

        */
        
    char *low;
    int M;
    int i = debut;
    int etape = 0;
    Chaines * chaines2;
    int choix;
    for (; i<argc;i++){
        if (!strcmp(argv[i],"--")){// si on a un double tiret c'est qu'on passe a une nouvelle chaine
            etape = 0;
            continue;
        }
        switch (etape)
        {
        case 0: // on cherche le fichier source et on charge les chaines
            if((chaines2 = ChargerChainesFile(argv [i],NULL,NULL,mode_silencieux))){
                libereChaines(*P_chaines);
                libererReseau(P_R);
                *P_chaines = chaines2;
                etape = 1;
                continue;
            }
            etape=6;
            continue;
        case 1: // on choix de la structure
            if ((choix = atoi(argv [i]))<=0 || choix >3){ // si le choix de structure n'est pas entre 1 et 3  on le redefini
                if (mode_silencieux) choix = fonction_defaut;
                else choix = choix_struct();
            }
            if (!choix) etape = 6;
            else if (choix == 2) etape = 3;

            else {
                etape = 2;
                i--;
            }
            continue;
        
        case 2: //utilisation de la structure pour reconstituer le reseau
            if((*P_R =  tabReconsReseau[choix-1](*P_chaines,M ))){
                etape = 4;
            }

            else{
                etape = 6;
            }
            libereChaines(*P_chaines);
            *P_chaines = NULL;
            continue;

        case 3: // Si on veut utiliser une table de hashchage, il faut choisir M
            M = atoi(argv [i]);
            if (!(M>=1 && M<=MAX_M)){
                if (mode_silencieux) M = (*P_chaines)->nbChaines*5;
                
                else M = choixM((int)((*P_chaines)->nbChaines*5));
            }
            etape = 2;
            i--;
            continue;

        case 4: // soit ecriture du Reseau dans un fichier ou debut nom de fichiers pour enregistrement svg
            if(!(low = lowerOf(argv[i]))){
                libererReseau(P_R);
                *P_R = NULL;
                return 0;
            }
            if (!strcmp(low,"svg")){// si on veut enregistrer une representation graphique des chaines au format SVG
                etape = 5;
                free(low);
                continue;
            }
            free(low);
            EcrituresReseauFile(argv[i],*P_R,mode_silencieux);
            continue;

        case 5:// enregistrement des fichier svg
            EcritureReseauSVG(argv[i],*P_R , mode_silencieux);
            continue;
        
        case 6: // Si il y a eu une erreur, on zappe jusqu'au prochain fichier source
            continue;
        }
        
    }
    return etape;

}

        




            
int main(int argc, char  *argv[])
{
    srand( time( NULL ) ); // la fonction d'affichage svg utilise rand() donc on doit initialiser le générateur de nombres aléatoires
    Chaines* chaines =NULL;
    Chaines** P_chaines =  &chaines;
    Reseau * R = NULL;
    Reseau ** P_R = &R;
    int M = 0; 
    sprintf(taille_H,"Veuillez specifier la taille souhaite pour la table de hashage (entre 1 et %d inclus) :\n",MAX_M);
    int ouvrir_menu = 1;
    int mode_silencieux = 0;
    int choix=0;
    if (argc==1) chaines = ChargerChainesFile(NULL,NULL,NULL,0);// si il n'y a pas d'argument on demande directement a l'utilisateur de donner un fichier contenant des chaines
    
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
        choix = liste_chaines( argc, argv ,x,P_R,P_chaines,mode_silencieux);

        if (!ouvrir_menu){
            libereChaines(chaines);
            libererReseau(&R);
            printf("Fin programme\n");
            return 0;
        }
        if (choix == 3){
            M = choixM((int)(M_defaut));
            R = reconstitueReseauHachage(chaines,M);
            libereChaines(chaines);
            chaines = NULL;
        }

    }
    
    

    if (!chaines && ! R){
        printf("Fin programme\n");
        return 0;
    }
    char affichage1[]={"\nAction disponible:\n\n0-sortie du programme\n1-Charger un nouvelle ensemble de chaines\n2-Reconstituer le Reseau \n\nQue voulez vous faire?\n"};
    char affichage2[]={"\nAction disponible:\n\n0-sortie du programme\n1-Charger un nouvelle ensemble de chaines\n2-Ecrire le Reseau dans un fichier\n3-Enregistrer une representation graphique du Reseau au format SVG\n\nQue voulez vous faire?\n"};
    int essai;
    void (*tabEnregistrement[])(char * nomfichier, Reseau *R , int mode_silencieux) = {EcrituresReseauFile,EcritureReseauSVG};
    int menu1 = 0;
    while (1)
    {
        essai=0;
        if (chaines){
            if (menu1){
                choix = (int)menu(&essai,5,0,2,0,affichage1,1) ;
                if (choix == 0){
                    libereChaines(chaines);
                    break;
                }
                if(choix ==1){
                    libereChaines(chaines);
                    chaines = NULL;
                    continue;
                }
                menu1 = 0;
            }
            if (!(choix = choix_struct())){
                menu1 = 1;
                continue;
            }
            R = tabReconsReseau[choix-1](chaines,(choix==2 ? choixM((int)(M_defaut)):0 ));
            if (!R){
                 continue;
            }
            libereChaines(chaines);
            chaines = NULL;
        }
        essai=0;
        if (R){
            while ((choix = (int)menu(&essai,5,0,3,0,affichage2,1) - 2)>=0){
            tabEnregistrement[choix](NULL,R,0);
            }
            libererReseau(&R);
            if (choix==-2) break;
            
            }
        if (!(chaines = ChargerChainesFile(NULL,NULL,NULL,0))) break;
    }
    printf("Fin programme\n");
    return 0;
}
