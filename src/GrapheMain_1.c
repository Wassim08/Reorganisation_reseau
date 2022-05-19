# include "../headers/Reseau.h"
# include "../headers/Hachage.h"
# include "../headers/OutilT.h"
# include "../headers/ArbreQuat.h"
# include "../headers/Graphe.h"

int main(){
	FILE* f1 = fopen("./docs/00014_burma.cha","r");
	FILE* f2 = fopen("./docs/05000_USA-road-d-NY.cha","r");
	FILE* f3 = fopen("./docs/07397_pla.cha","r");
	Chaines* C1 = lectureChaines(f1);
	Chaines* C2 = lectureChaines(f2);
	Chaines* C3 = lectureChaines(f3);
	
	
	Reseau* r1 = reconstitueReseauListe(C1);
	Reseau* r2 = reconstitueReseauListe(C2);
	Reseau* r3 = reconstitueReseauListe(C3);
	
	printf("%d %d %d \n",reorganiseReseau(r1),reorganiseReseau(r2),reorganiseReseau(r3));
	
	libererReseau(r1);
	libererReseau(r2);
	libererReseau(r3);
	fclose(f1);
	fclose(f2);
	fclose(f3);
	
}
