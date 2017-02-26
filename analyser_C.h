#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<assert.h>
#define NONPATH 0
#define TRUE 1
#define FALSE 0
#define MAXFILE 100
#define MAXPROFONDEUR 300
#define GMAXND 700

typedef unsigned int Shu; // le noeud n'est jamais négatif
typedef int node_t; // si nb noeud > 2^16, le type short ne marche plus


struct strand{
  Shu node; // le noeud courant
  node_t next; // le noeud suivant
  /// ajouté 
  int poid; // necessaire pour creer un graphe orienté parent prends le poid 0, l'enfant prends le poid 1
  /// ajouté
};
typedef struct strand strand;


struct strandgraph {
  Shu nbs; // max noeud possible
  Shu nbstr; // tableau de brin
  node_t * node; /* first strand*/
  strand * nxt;/* node and next strand*/
  /// ajouté
  Shu nbnd; // nbre de noeud
  Shu nbbrin; // nbre de noeud associe  
  node_t * declaration; 
  node_t * recursive; 
  char * listf[GMAXND]; // liste de fonction dans le graphe
  char * listfile[MAXFILE]; // liste de fichier a lire
  node_t * appel;
  /// ajouté 
};
typedef struct strandgraph strgr;


enum{
	MAT_MAX = 100,
};

struct graph_mat{
	int nbNode; // nb noeud
	int lineMax; // le max noeud
	int matrice[MAT_MAX * MAT_MAX]; // graph 
	char * listf[MAT_MAX]; // le nom de fonction
	int declaration[MAT_MAX]; // tableau de declaration
  	int appel[MAT_MAX]; // nombre d
};
typedef struct graph_mat graph_mat;


// mat_base.c
extern graph_mat Mat_createGraph();
extern int Mat_addGraph(graph_mat * gm,char * fname);
extern int Mat_insertNode(graph_mat * gm,int parent,int enfant);
extern int Mat_isInGraph(graph_mat gm,char * fname);
extern graph_mat MAT_parser(char * path,graph_mat gm);
extern void Mat_profondeur(graph_mat gm);
extern int Mat_graphInfo(graph_mat gm);
extern int Mat_connexe(graph_mat g);
extern void Mat_connexePoid(graph_mat g);
// mat_base.c



// parserUtils.c
extern char * getName(FILE * f,int prec);
extern int exception(char * name);
extern int isNameChar(int c);
extern void espace(int nb);
extern void espace2(int nb);
// parserUtils.c


// brin_base.c
extern void Brin_freeMemory(strgr * g);
extern strgr Brin_parser(char * path,strgr g);
extern strgr Brin_make(int maxN);
extern void Brin_insertNode(strgr * g,Shu parent,Shu enfant);
extern void add_sub(strgr *g,node_t path,Shu node);
extern int addGraphBrin(strgr * g,char * fname);
extern int isInGraphBrin(strgr g,char * fname);
extern void Brin_infoGraph(strgr g);
extern void Brin_affiche(strgr g);
extern int Brin_ispath(strgr g,Shu dep,Shu arr);
extern void Brin_findPath(strgr g,Shu dep,Shu arr);
extern int Brin_connexe(strgr g);
extern void Brin_afficheSucesseur(strgr g);
extern void Brin_printRoot(strgr g);
extern void print_sub(strgr g,Shu dep,int etage,node_t *  visite);

// brin_base.c



// brin_annalyse.c
extern void Brin_analyseError4(strgr g);
extern void Brin_analyseError3(strgr g);
extern void Brin_analyseError2(strgr g);
extern void Brin_analyseError1(strgr g);
// brin_annalyse.c
