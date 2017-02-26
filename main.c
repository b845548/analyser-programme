/*
# File : main.c
# Programme : l'analyseur du code C avec le graph brin
# Auteur : HOBEAN BAE
# Date : 6/DEC/2016
*/

#include"analyser_C.h"

int main(int args,char ** argv){
	int i;
	strgr brinG;
	graph_mat matG;
	char * path[MAXFILE]={
		"testcode/example.c",
		/*
		"testcode/window.c",
		"testcode/HBB_fractal.c",
		"testcode/HBB_tron.c",
		"testcode/HBB_graph.c",
		"testcode/HBB_soleil.c",
		"testcode/HBB_voiture.c",
		"testcode/HBB_spiral.c",
		"testcode/HBB_credit.c",
		"testcode/animations.c",
		"testcode/HBB_battleship.c",
		"testcode/HBB_objet.c",
		"testcode/audioHelper.c",*/
		NULL
		};
 
brinG=Brin_make(GMAXND);
for(i=0;path[i]!=NULL;i++){
  brinG.listfile[i]=path[i];
  brinG=Brin_parser(brinG.listfile[i],brinG);
  
}

printf("\n\n##################  GRAPHE BIRN  #################\n\n");
Brin_printRoot(brinG); // s'il y a une fonction recursive, il ne marche pas
Brin_analyseError1(brinG); // detect fonction jamais appelée
Brin_analyseError2(brinG); //detect fonction non declaré
Brin_analyseError3(brinG); // detect fonction infinie  ex) a() -> b() -> c()
Brin_analyseError4(brinG); // detect fonction recursive ex) fib() -> fib
Brin_infoGraph(brinG);
Brin_freeMemory(&brinG);


matG=Mat_createGraph();
for(i=0;path[i]!=NULL;i++)
	matG=MAT_parser(path[i],matG);

printf("\n\n##################  GRAPHE MAT  #################\n\n");
Mat_profondeur(matG);
//Mat_graphInfo(gm);
Mat_connexePoid(matG);
//printf("%s %d\n",matG.nameFunc[23],matG.appel[23]);
} 
