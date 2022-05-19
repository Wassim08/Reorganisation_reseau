# include "../headers/Chaine.h"
#include <dirent.h>
// Genere des fichiers contenant des chaines
// format d'appel >./bin/generationAleatoireFileMain2 'chemin_dossier_sauvegarde' 'nb_fichier_a_generer' 'xmax' 'ymax'  'nb_min_chaines' 'nb_max_chaines' 'nb_min_points' 'nb_max_points'
// si l'option '-f'  est mise en premiere arguments, le programme ecrasera les fichiers  ayant le meme nom que ceux generer ( a la fin on peut se retrouver avec moin de fichiers que 'nb_fichier_a_generer')
// si l'option '-f' n'est pas mise , si un fichier generer porte le meme nom qu'un fichier existant, un suffix lui seras ajouter exemple :(nom_fichier)_2
int main(int argc, char  *argv[])
{
    srand( time( NULL ) ); // on utilise rand() donc on doit initialiser le générateur de nombres aléatoires
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
    if (argc!=8 ){
        fprintf(stderr, "Erreur dans les arguments: l'appel doit etre effectue comme suit >./bin/generationAleatoireFileMain2 'chemin_dossier_sauvegarde' 'nb_fichier_a_generer' 'xmax' 'ymax'  'nb_min_chaines' 'nb_max_chaines' 'nb_min_points' 'nb_max_points'\n");
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
    int nb_fichier_a_generer = atoi(argv[1]);
    int xmax = atoi(argv[2]);
    int ymax = atoi(argv[3]);
    int nb_min_chaines = atoi(argv[4]);
    int nb_max_chaines = atoi(argv[5]);
    int nb_min_points = atoi(argv[6]);
    int nb_max_points = atoi(argv[7]);
    int nb_points;
    int nb_chaines;
    // on verifie les arguments
    if (nb_fichier_a_generer<0){
        fprintf(stderr, "Erreur dans les arguments: le nombre de fichier a generer doit etre positif\n"); // si il est egal a 0 rien ne se passe
        return 1;
    }
    if (xmax <0 || ymax <0){
        fprintf(stderr, "Erreur dans les arguments: xmax et ymax doivent etre positifs ou nul\n");
        return 1;
    }
    if (nb_min_chaines <=0 || nb_min_points <=0 ){
        fprintf(stderr, "Erreur dans les arguments: le nombre minimum de chaines et de points doivent etre strictement superieur a 0\n");
        return 1;
    }

    if (nb_max_chaines < nb_min_chaines || nb_max_points < nb_min_points){
        fprintf(stderr, "Erreur dans les arguments: le nombre maximum de chaines  doit etre superieur ou egal au nombre mimum de chaines et le nombre maximum de points  doit etre superieur ou egal au nombre mimum de points\n");
    }

    FILE * fic ;
    for (int i = 0; i<nb_fichier_a_generer ; i++){ 
        // On genere le nombre de points et de chaines

        nb_chaines = rand()%(nb_max_chaines - nb_min_chaines +1) + nb_min_chaines;
        nb_points =rand()%(nb_max_points - nb_min_points +1) + nb_min_points;
        // puis on creer le fichiers
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
