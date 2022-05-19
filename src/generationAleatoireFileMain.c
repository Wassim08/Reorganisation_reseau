# include "../headers/Chaine.h"
#include <dirent.h>
// Genere des fichiers contenant des chaines
// format d'appel >./bin/generationAleatoireFileMain 'chemin_dossier_sauvegarde' 'xmax' 'ymax' 'nombre_chaines_fichier1' 'nombre_points_fichier1' 'nombre_chaines_fichier2' 'nombre_points_fichier2' ...
// si l'option '-f'  est mise en premiere arguments, le programme ecrasera les fichiers  ayant le meme nom que ceux generer
// si l'option '-f' n'est pas mise , si un fichier generer porte le meme nom qu'un fichier existant, un suffix lui seras ajouter exemple :(nom_fichier)_2
int main(int argc, char  *argv[])
{
    srand( time( NULL ) );
    DIR *dp;
    int mode_silencieux = 2;

    argv++;
    argc--;
    for (;argc;argc--){
        if (!strcmp(*argv,"-f"))
            mode_silencieux = 1;
        else break;

        argv++;

    }
    if (argc<5 || argc%2!=1){
        fprintf(stderr, "Erreur dans les arguments, l'appel doit etre effectue comme suit >./bin/generationAleatoireFileMain 'chemin_dossier_sauvegarde' 'nombre_chaines_fichier1' 'nombre_points_fichier1' 'nombre_chaines_fichier2' 'nombre_points_fichier2' ...\n");
        return 1;
    }
    dp = opendir(argv[0]);
    if (dp == NULL){// on verifie que le chemin est bon
        fprintf(stderr, "Impossible d'ouvrir le dossier'%s'%s\n", *argv, (errno == 2 ? ", veuillez verifier le nom du fichier ainsi que son chemein d'acces\n" : strerror(errno)));
        return 1;
    }
    closedir(dp);
    // on rajoute '/' a la fin du chemin d'acces
    if (!(argv[0] = strdup(argv[0]))) return 1;
    if (!(concatenate(argv,"/"))){
        free(*argv);
        return 1;
    }
    char nom_fichier[100];
    int xmax = atoi(argv[1]);
    int ymax = atoi(argv[2]);
    if (xmax <0 || ymax <0){
        fprintf(stderr, "Erreur dans les arguments, xmax et ymax doivent etre positifs ou nul\n");
        return 1;
    }
    int nb_chaines;
    int nb_points;
    FILE * fic ;
    for (int i = 3; i<argc ; i++){ // on recupere les differents nombres de chaines et nombre de points
        nb_chaines = atoi(argv[i]);
        nb_points = atoi(argv[++i]);
        if (nb_chaines<=0 || nb_points<=0) continue; // si nb_chaines ou nb_points est nulle on passe aux suivant (la fonction marcherait quand meme mais ce n'est pas interessant d'avoir 0 points ou 0 chaines)
        if (mode_silencieux==1){
            sprintf(nom_fichier,"%dchaines_%dpoints.cha",nb_chaines,nb_points);
            fic = generationFilealeatoire(nom_fichier,*argv,NULL,nb_chaines,nb_points,xmax,ymax,mode_silencieux);
        }
        else{
            sprintf(nom_fichier,"%dchaines_%dpoints",nb_chaines,nb_points);
            fic = generationFilealeatoire(nom_fichier,*argv,".cha",nb_chaines,nb_points,xmax,ymax,mode_silencieux);
        }
        if(fic)   
            fclose(fic);
    }

    free(*argv);
    return 0;
}
