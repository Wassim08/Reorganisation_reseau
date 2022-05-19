# include "../headers/Reseau.h"
# include "../headers/Hachage.h"
# include "../headers/OutilT.h"
# include "../headers/ArbreQuat.h"
// Test la performance des structures des fonctions reconstitueReseauListe reconstitueReseauHachage reconstitueReseauArbre
// Genere un fichier rapport de l'analyse
// format d'appel >./bin/PerformanceReconstitution2 'nom_fichier_destination_rapport_liste' 'nom_fichier_destination_rapport_hachage_abre' 'nbPointsChaine' 'xmax' 'ymax' 'min_nb_chaines' 'max_nb_chaines' 'pas'
int main(int argc, char  *argv[])

{   

    if (argc != 9){
        fprintf(stderr, "Erreur dans les arguments, l'appel doit etre effectue comme suit >./bin/PerformanceReconstitution2 'nom_fichier_destination_rapport_liste' 'nom_fichier_destination_rapport_hachage_abre' 'nbPointsChaine' 'xmax' 'ymax' 'min_nb_chaines' 'max_nb_chaines' 'pas'\n");
        return 1;
    }
    srand( time( NULL ) ); // on utilise rand() donc on doit initialiser le générateur de nombres aléatoires
    Chaines * chaines;
    argc --;
    argv ++;
    // on met le nom du fichier de destination du rapport de reconstitueReseauListe dans un espace allouer dynamiquement pour pouvoir utiliser la fonction Open_write_only
    if (!(*argv = strdup(*argv))){
        fprintf(stderr, "Erreur strdup\n");
        printf("Analyse perfomance abandonnee\n");
        return 1;
    }
    // on creer le fichier pour le rapport de reconstitueReseauListe
    FILE * L_fic = Open_write_only(argv,NULL,NULL,0);
    free(*argv); // on libere le nom du fichier car on en a plus besoin
    if (!L_fic) {
        printf("Analyse perfomance annulee\n");
        return 0;// on ne met pas return 1 car si il n'y pas de fic c'est que l'utilisateur l'a choisi (Open_write_only laisse le choix de redefinir le nom du fichier si il n'est pas trouvé) donc c'est pas une erreur
    }
    argc --;
    argv ++;
    // maintenant on fait de meme pour le fichier du rapport de reconstitueReseauHachage et reconstitueReseauArbre
    if (!(*argv = strdup(*argv))){
        fprintf(stderr, "Erreur strdup\n");
        fclose(L_fic);
        printf("Analyse perfomance abandonnee\n");
        return 1;
    }
    // on creer le fichier pour le rapport de reconstitueReseauHachage et reconstitueReseauArbre
    FILE * H_A_fic = Open_write_only(argv,NULL,NULL,0);
    free(*argv); // on libere le nom du fichier car on en a plus besoin
    if (!H_A_fic) {
        printf("Analyse perfomance annulee\n");
        fclose(L_fic);
        return 0;// on ne met pas return 1 car si il n'y pas de fic c'est que l'utilisateur l'a choisi (Open_write_only laisse le choix de redefinir le nom du fichier si il n'est pas trouvé) donc c'est pas une erreur
    }
    // on recupere les arguments
    int nbPointsChaine = atoi(argv[1]);
    int xmax =atoi(argv[2]);
    int ymax = atoi(argv[3]);
    int min_nb_chaines = atoi(argv[4]);
    int max_nb_chaines = atoi(argv[5]);
    int pas = atoi(argv[6]);

    // on verifie la validite des arguments
    if (nbPointsChaine <=0 || min_nb_chaines <=0 || max_nb_chaines <=0 || pas <=0){
        fprintf(stderr, "Erreur dans les arguments: nbPointsChaine , min_nb_chaines , max_nb_chaines et pas doivent etre strictement superieur a 0\n");
        printf("Analyse perfomance abandonnee\n");
        fclose(L_fic);
        fclose(H_A_fic);
        return 1;
    }

    if (xmax<0 || ymax <0){
        fprintf(stderr, "Erreur dans les arguments: xmax et ymax doivent etre positifs\n");
        printf("Analyse perfomance abandonnee\n");
        fclose(L_fic);
        fclose(H_A_fic);
        return 1;
    }

    if (max_nb_chaines < min_nb_chaines){
        fprintf(stderr, "Erreur dans les arguments: max_nb_chaines doit etre superieur ou egal a min_nb_chaines\n");
        printf("Analyse perfomance abandonnee\n");        
        fclose(L_fic);
        fclose(H_A_fic);
        return 1;
    }
    // on ecrit les commentaire du rappport de reconstitueReseauListe
    fprintf(L_fic,"# nb= nombre de points total des chaines\n");
    fprintf(L_fic,"# L=temps_cpu de reconstitueReseauListe en seconde\n\n");
    fprintf(L_fic,"# nb\t\t    L\t\n");

    // on ecrit les commentaire du rappport de reconstitueReseauHachage et reconstitueReseauArbre
    fprintf(H_A_fic,"# nb= nombre de points total des chaines\n");
    fprintf(H_A_fic,"# H=temps_cpu de reconstitueReseauHachage en seconde\n");
    fprintf(H_A_fic,"# A=temps_cpu de reconstitueReseauArbre en seconde\n");
    fprintf(H_A_fic,"# La taille  de chaque table de Hachage vaut : " xstr(M_defaut) "\n\n");
    fprintf(H_A_fic,"# nb            H\t            A\n");


    // on declare ce dont on aura besoin pour calculer les temps de calcul
    clock_t temps_initial ;
    clock_t temps_final ;
    double temps_cpu_Liste;
    double temps_cpu_TableHachage;
    double temps_cpu_Arbre;

    int M; // taille de la table de hachage
    Reseau* R;
    int nbPointTotale;

    // on fait une  boucle pour analyser les fonctions
    for (int x =min_nb_chaines; x<=max_nb_chaines; x+=pas){
        // on creer la chaines
        chaines = generationAleatoire(x,nbPointsChaine,xmax,ymax);
        nbPointTotale = comptePointsTotal(chaines);
        EcrituresChainesFile("stdout",chaines,1);//suppr
        if (!chaines){
            printf("Analyse perfomance abandonnee\n");
            fclose(L_fic);
            fclose(H_A_fic);
            return 1;
        }
        // on calcule le temps de reconstitueReseauListe
        temps_initial = clock () ;
        R = reconstitueReseauListe(chaines);
        temps_final = clock () ;
        temps_cpu_Liste = (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
        
        // puis on ecrit dans le  fichier
        fprintf(L_fic,"%d\t\t%f\n",nbPointTotale,temps_cpu_Liste);
        libererReseau(&R);

        // on calcule le temps pour reconstitueReseauHachage
        M = M_defaut;
        temps_initial = clock () ;
        R = reconstitueReseauHachage(chaines,M);
        temps_final = clock () ;
        temps_cpu_TableHachage = (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
        libererReseau(&R);
        // on calcule le temps pour reconstitueReseauArbre
        temps_initial = clock () ;
        R = reconstitueReseauArbre(chaines);
        temps_final = clock () ;
        temps_cpu_Arbre = (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;

        // puis on ecrit dans le  fichier
        fprintf(H_A_fic,"%d\t\t%f\t%f\n",nbPointTotale,temps_cpu_TableHachage,temps_cpu_Arbre);
        libererReseau(&R);

        // et on libere les chaines 
        libereChaines(chaines);

    }
    // on en a fini avec les fichiers, on peut donc les fermer
    if (fclose(L_fic))
        printf("Attention le fichier du rapport de reconstitueReseauHachage et reconstitueReseauArbre ne s'est pas referme correctement\n");

    if (fclose(H_A_fic))
        printf("Attention le fichier du rapport de reconstitueReseauListe ne s'est pas referme correctement\n");
   
    
    return 0;
}
