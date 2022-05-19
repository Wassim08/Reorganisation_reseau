# include "../headers/Reseau.h"
# include "../headers/Hachage.h"
# include "../headers/OutilT.h"
# include "../headers/ArbreQuat.h"
# include <dirent.h>


void performance(FILE* dest ,int nbRepetition,char * chemin_dossier, char *nomfichier){
    Chaines* chaines= ChargerChainesFile(nomfichier,chemin_dossier,NULL,1);
    if(!chaines) return;
    Reseau* R = NULL;
    clock_t temps_initial ;
    clock_t temps_final ;
    double temps_cpu_Liste = 0;
    double temps_cpu_TableHachage  = 0;
    double temps_cpu_Arbre  = 0;
    
    int M;
    for(int i=0; i<nbRepetition; i++){
        libererReseau(&R);
        // on calcule le temps pour reconstitueReseauListe
        temps_initial = clock () ;
        R = reconstitueReseauListe(chaines);
        temps_final = clock () ;
        temps_cpu_Liste += (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
        libererReseau(&R);
        // on calcule le temps pour reconstitueReseauHachage
        M = M_defaut;
        temps_initial = clock () ;
        R = reconstitueReseauHachage(chaines,M);
        temps_final = clock () ;
        temps_cpu_TableHachage += (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
        libererReseau(&R);
        // on calcule le temps pour reconstitueReseauArbre
        temps_initial = clock () ;
        R = reconstitueReseauArbre(chaines);
        temps_final = clock () ;
        temps_cpu_Arbre += (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
        
    }
    // on report les infos dans le fichier de destination
    
    fprintf(dest,"%d\t\t%f\t%f\t%f\n",comptePointsTotal(chaines),temps_cpu_Liste,temps_cpu_TableHachage,temps_cpu_Arbre);
    libererReseau(&R);
    libereChaines(chaines);

    

}

int main(int argc, char *argv[])
{
    srand( time( NULL ) ); // la fonction d'affichage svg utilise rand() donc on doit initialiser le générateur de nombres aléatoires
    if (argc < 4)
    {
        fprintf(stderr, "Erreur : Plus d'argument sont attendue comme suit >./bin/PerformanceReconstitution 'nom_ficher_destination' 'nombre_de_repetition' 'chemin_dossier_source_1' 'chemin_dossier_source_2'...\n");
        return 1;
    }
    int nbRepetition = atoi(argv[2]);
    if(nbRepetition<=0){
        fprintf(stderr, "Erreur : Le nombre de repetition doit etre strictement superieur a 0");
        return 1;
    }
    // on met le nom du fichier de destination du rapport de reconstitueReseauListe dans un espace allouer dynamiquement pour pouvoir utiliser la fonction Open_write_only
    if (!(argv[1] = strdup(argv[1]))){
        fprintf(stderr, "Erreur strdup\n");
        printf("Analyse perfomance abandonnee\n");
        return 1;
    }
    FILE *fic = Open_write_only(argv + 1, NULL, NULL, 0);
    if (!fic){
        free(argv[1]);
        return 1;
    }
    fprintf(fic,"# nb= nombre de points total des chaines\n");
    fprintf(fic,"# L=temps_cpu de reconstitueReseauListe en seconde\n");
    fprintf(fic,"# H=temps_cpu de reconstitueReseauHachage en seconde\n");
    fprintf(fic,"# A=temps_cpu de reconstitueReseauArbre en seconde\n");
    fprintf(fic,"# La taille  de chaque table de Hachage vaut : " xstr(M_defaut) "\n");
    fprintf(fic,"# Les temps sont obtenus apres %d repetitions des fonctions de reconstitutions\n\n",nbRepetition);
    fprintf(fic,"# nb\t\t    L\t            H\t            A\n");
    DIR *dp;
    struct dirent *dir;
    char *copy_chemin_dossier;
    for (int i = 3; i < argc; i++)
    {
        if (!(copy_chemin_dossier = strdup(argv[i]))) continue;
        if (!(concatenate(&copy_chemin_dossier,"/"))){
            free(copy_chemin_dossier);
            continue;
        }
        dp = opendir(argv[i]);
        if (dp != NULL)
        {
            while ((dir = readdir(dp)) != NULL)
            {
                if (dir->d_type == DT_REG)
                {
                    performance(fic,nbRepetition,copy_chemin_dossier,dir->d_name);
                }
            }
            closedir(dp);
        }
        else
            fprintf(stderr, "Impossible d'ouvrir le dossier'%s'%s\n", argv[i], (errno == 2 ? ", veuillez verifier le nom du fichier ainsi que son chemein d'acces\n" : strerror(errno)));
        free(copy_chemin_dossier);
    }
    fclose(fic);
    free(argv[1]);

    return 0;
}
