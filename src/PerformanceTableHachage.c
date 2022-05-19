# include "../headers/Reseau.h"
# include "../headers/Hachage.h"
# include "../headers/OutilT.h"
// Test la performance de reconstitueReseauHachage en fonction de sa taille
// Genere un fichier rapport de l'analyse
// format d'appel >./bin/PerformanceTableHachage 'nom_fichier_destination_rapport' 'nbPointsChaine' 'xmax' 'ymax' 'min_nb_chaines' 'max_nb_chaines' 'pas'
int main(int argc, char  *argv[])

{   

    if (argc != 8){
        fprintf(stderr, "Erreur dans les arguments, l'appel doit etre effectue comme suit >./bin/PerformanceTableHachage 'nom_fichier_destination_rapport' 'nbPointsChaine' 'xmax' 'ymax' 'min_nb_chaines' 'max_nb_chaines' 'pas'\n");
        return 1;
    }
    srand( time( NULL ) ); // on utilise rand() donc on doit initialiser le générateur de nombres aléatoires
    Chaines * chaines;
    argc --;
    argv ++;
    // on met le nom du fichier de destination du rapport
    if (!(*argv = strdup(*argv))){
        fprintf(stderr, "Erreur strdup\n");
        printf("Analyse perfomance abandonnee\n");
        return 1;
    }
    // on creer le fichier pour le rapport 
    FILE * fic = Open_write_only(argv,NULL,NULL,0);
    free(*argv); // on libere le nom du fichier car on en a plus besoin
    
    // on recupere les arguments
    int nbPointsChaine = atoi(argv[1]);
    int xmax =atoi(argv[2]);
    int ymax = atoi(argv[3]);
    int min_nb_chaines = atoi(argv[4]);
    int max_nb_chaines = atoi(argv[5]);
    int pas = atoi(argv[6]);

    // on verifie la validite des arguments
    if (nbPointsChaine <=0 || min_nb_chaines <=0 || max_nb_chaines <=0 || pas <=0){
        fprintf(stderr, "Erreur dans les arguments, l'appel doit etre effectue comme suit >./bin/PerformanceTableHachage 'nom_fichier_destination_rapport' 'nbPointsChaine' 'xmax' 'ymax' 'min_nb_chaines' 'max_nb_chaines' 'pas'\n");
        printf("Analyse perfomance abandonnee\n");
        fclose(fic);
        return 1;
    }

    if (xmax<0 || ymax <0){
        fprintf(stderr, "Erreur dans les arguments: xmax et ymax doivent etre positifs\n");
        printf("Analyse perfomance abandonnee\n");
        fclose(fic);
        return 1;
    }

    if (max_nb_chaines < min_nb_chaines){
        fprintf(stderr,  "Erreur dans les arguments, l'appel doit etre effectue comme suit >./bin/PerformanceTableHachage 'nom_fichier_destination_rapport' 'nbPointsChaine' 'xmax' 'ymax' 'min_nb_chaines' 'max_nb_chaines' 'pas'\n");
        printf("Analyse perfomance abandonnee\n");        
        fclose(fic);
        return 1;
    }
    // on ecrit les commentaire du rappport
    fprintf(fic,"# nb= nombre de points total des chaines\n");
    fprintf(fic,"# C = colonne , M = taille table\n");
    fprintf(fic,"# C1 M=500,C2 M2=chaines->nbChaines , C3 M2=chaines->nbChaines*5, C4 M =comptePointsTotal(chaines)/2 , C5 M = comptePointsTotal(chaines)\n\n");
    fprintf(fic,"# nb\t\t    C1\t            C2\t            C3\t            C4\t            C5\n");



    // on declare ce dont on aura besoin pour calculer les temps de calcul
    clock_t temps_initial ;
    clock_t temps_final ;
    double temps_cpu_C1;
    double temps_cpu_C2;
    double temps_cpu_C3;
    double temps_cpu_C4;
    double temps_cpu_C5;


    int Nb_points_totale;
    Reseau* R;

    // on fait une  boucle pour analyser les fonctions
    for (int x =min_nb_chaines; x<=max_nb_chaines; x+=pas){
        // on creer la chaines
        chaines = generationAleatoire(x,nbPointsChaine,xmax,ymax);
        Nb_points_totale = nbPointsChaine * x;
        if (!chaines){
            printf("Analyse perfomance abandonnee\n");
            fclose(fic);
            return 1;
        }
        // on calcule le temps de C1
        temps_initial = clock () ;
        R = reconstitueReseauHachage(chaines,500);
        temps_final = clock () ;
        temps_cpu_C1 = (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
        libererReseau(&R);
        // on calcule le temps de C2
        temps_initial = clock () ;
        R = reconstitueReseauHachage(chaines,x);
        temps_final = clock () ;
        temps_cpu_C2 = (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
        libererReseau(&R);
        // on calcule le temps de C3
        temps_initial = clock () ;
        R = reconstitueReseauHachage(chaines,x*5);
        temps_final = clock () ;
        temps_cpu_C3 = (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
        libererReseau(&R);
        // on calcule le temps de C4
        temps_initial = clock () ;
        R = reconstitueReseauHachage(chaines,Nb_points_totale/2);
        temps_final = clock () ;
        temps_cpu_C4 = (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
        libererReseau(&R);
        // on calcule le temps de C1
        temps_initial = clock () ;
        R = reconstitueReseauHachage(chaines,Nb_points_totale);
        temps_final = clock () ;
        temps_cpu_C5 = (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
        libererReseau(&R);
        // puis on ecrit dans le  fichier
        fprintf(fic,"%d\t\t%f\t%f\t%f\t%f\t%f\n",Nb_points_totale,temps_cpu_C1,temps_cpu_C2,temps_cpu_C3,temps_cpu_C4,temps_cpu_C5);
        
        // et on libere les chaines 
        libereChaines(chaines);

    }
    if (fclose(fic))
        printf("Attention le fichier du rapport\n");

    
    return 0;
}
