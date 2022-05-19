# include "../headers/Reseau.h"
# include "../headers/Graphe.h"
int main(int argc, char  *argv[])
{
    Chaines * c = ChargerChainesFile("./docs/00014_burma.cha",NULL,NULL,0);
    // c->gamma=2;
    Reseau* r =reconstitueReseau(c,3,0,1);
    libererReseau(&r);
    
    libereChaines(c);
    return 0;
}
