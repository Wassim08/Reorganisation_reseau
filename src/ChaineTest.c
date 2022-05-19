# include "../headers/Chaine.h"


void test_Lecture_ecriture_chaines(){
    clock_t temps_initial ;
    clock_t temps_final ;
    double temps_cpu  ;
    
    int  tab []= {5,10,50,100,500,1000,1500,2000};
    FILE* fic;
    Chaines * tab_EnsembleChaines[8];
    printf("\n\n#NbChaine\t temps_Lecture\n");
    for (int j=0;j<8;j++){
        fic = tmpfile();
        printf("%d",tab[j]);
        temps_cpu =0;
        
        fprintf(fic,"NbChain : %d\nGamma : %d\n\n",tab[j],4);
        for (int i=0; i<tab[j]; i++){
            double x1= ((double)rand()/(double)(RAND_MAX/200))+12 ; double y1= ((double)rand()/(double)(RAND_MAX/200))+12 ;
            double x2= ((double)rand()/(double)(RAND_MAX/200))+12 ; double y2= ((double)rand()/(double)(RAND_MAX/200))+12 ;
            double x3= ((double)rand()/(double)(RAND_MAX/200))+12 ; double y3= ((double)rand()/(double)(RAND_MAX/200))+12 ;
            double x4= ((double)rand()/(double)(RAND_MAX/200))+12 ; double y4= ((double)rand()/(double)(RAND_MAX/200))+12 ;
            double x5= ((double)rand()/(double)(RAND_MAX/200))+12 ; double y5= ((double)rand()/(double)(RAND_MAX/200))+12 ;
            fprintf(fic,"%d 5 %.9g %.9g %.9g %.9g %.9g %.9g %.9g %.9g %.9g %.9g\n",i,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5);

        }
        Chaines * EnsembleChaines;

        fflush(fic);
        rewind(fic);
        
        for(int i=0; i<750; i++){
            rewind(fic);
            temps_initial = clock () ;
            EnsembleChaines = lectureChaines(fic);
            temps_final = clock () ;
            temps_cpu += (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
            libereChaines(EnsembleChaines);

        }
        printf("\t%f\n",temps_cpu);
        rewind(fic);
        tab_EnsembleChaines[j]= lectureChaines(fic);
        fclose(fic);
    
    }
    printf("\n\n#NbChaine\t algo1\talgo2\n");
    for (int j=0;j<8;j++){
        printf("%d",tab[j]);
        temps_cpu =0;
        
        for(int i=0; i<750; i++){
            fic = tmpfile();
            temps_initial = clock () ;
            ecrireChaines(tab_EnsembleChaines[j],fic);
            temps_final = clock () ;
            temps_cpu += (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC ;
            fclose(fic);
        }
        printf("\t%f",temps_cpu);
        temps_cpu =0;
        for(int i=0; i<750; i++){
            fic = tmpfile();
            temps_initial = clock () ;
            ecrireChaines(tab_EnsembleChaines[j],fic);
            temps_final = clock () ;
            temps_cpu += (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC ;
            fclose(fic);
        }
        printf("\t%f\n",temps_cpu);


    }
    for (int j=0;j<8;j++)
        libereChaines(tab_EnsembleChaines[j]);
    



    
}

void test_Lecture_ecriture_points(){
    clock_t temps_initial ;
    clock_t temps_final ;
    double temps_cpu  ;
    int  tab []= {5,10,50,100,500,1000,1500,2000};
    FILE* fic;
    Chaines * tab_EnsembleChaines[8];
    printf("\n\n#NbPoints\t temps_Lecture\n");
    for (int j=0;j<8;j++){
        fic = tmpfile();
        printf("%d",tab[j]);
        temps_cpu =0;
        
        fprintf(fic,"NbChain : %d\nGamma : %d\n\n",5,4);
        for (int i=0; i<5; i++){
            fprintf(fic,"%d %d",i,tab[j]);
            for(int x = 0; x<tab[j];x++){
                double x= ((double)rand()/(double)(RAND_MAX/200))+12 ; double y= ((double)rand()/(double)(RAND_MAX/200))+12 ;
                fprintf(fic," %.9g %.9g",x,y);
            }
            fprintf(fic,"\n");
        }
        Chaines * EnsembleChaines;

        fflush(fic);
        rewind(fic);
        
        for(int i=0; i<750; i++){
            rewind(fic);
            temps_initial = clock () ;
            EnsembleChaines = lectureChaines(fic);
            temps_final = clock () ;
            temps_cpu += (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC  ;
            libereChaines(EnsembleChaines);

        }
        printf("\t%f\n",temps_cpu);
        rewind(fic);
        tab_EnsembleChaines[j]= lectureChaines(fic);
        fclose(fic);
    
    }
    printf("\n\n#NbPoints\t algo1\talgo2\n");
    for (int j=0;j<8;j++){
        printf("%d",tab[j]);
        temps_cpu =0;
        
        for(int i=0; i<750; i++){
            fic = tmpfile();
            temps_initial = clock () ;
            ecrireChaines(tab_EnsembleChaines[j],fic);
            temps_final = clock () ;
            temps_cpu += (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC ;
            fclose(fic);
        }
        printf("\t%f",temps_cpu);
        temps_cpu =0;
        for(int i=0; i<750; i++){
            fic = tmpfile();
            temps_initial = clock () ;
            ecrireChaines(tab_EnsembleChaines[j],fic);
            temps_final = clock () ;
            temps_cpu += (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC ;
            fclose(fic);
        }
        printf("\t%f\n",temps_cpu);


    }
    for (int j=0;j<8;j++)
        libereChaines(tab_EnsembleChaines[j]);
    



    
}

int main(int argc, char const *argv[])
{   
    srand( time( NULL ) );

    test_Lecture_ecriture_chaines();
    test_Lecture_ecriture_points();
    return 0;
}
