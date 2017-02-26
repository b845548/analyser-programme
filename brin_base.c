
#include"analyser_C.h"


static int ACCOLADE=0;
static int PARENT=0;
static int COMMENTAIRE_T1=0;
static int COMMENTAIRE_T2=0;

extern int isInGraphBrin(strgr g,char * fname){
  int i;
  for(i=0;i<g.nbs;i++) // parcours tous les noeud 
  	if(g.listf[i]!=NULL) // le nom de fonction existe
 	   if(strcmp(fname,g.listf[i])==0)return i; // si on trouve le nom de fonction dans la liste
  return -1; // si on ne trouve pas 
}

extern int addGraphBrin(strgr * g,char * fname){
	g->listf[g->nbnd]=fname; // ajouter le nom de fonction à une nouvelle place
	return g->nbnd++; //nbre de noeud ++
}


extern void add_sub(strgr *g,node_t path,Shu node){
	node_t parent;
	if(g->node[node]==0){ // créer le noeud
		g->node[node]=(node_t)path; //une nouvelle place 
		g->nxt[path+(g->nbstr>>1)].node=(Shu)node; // identifiant de noeud
		g->nxt[path+(g->nbstr>>1)].next=(node_t)path; // pointe vers lui-même 
	}else{ // inserer le noeud 	
		parent=(node_t)g->nxt[g->node[node]+(g->nbstr>>1)].next; // sauvegarder le chemin vers le suivant 
		g->nxt[g->node[node]+(g->nbstr>>1)].next=(node_t)path; // le suivant du noeud parent prends le chemin du nouveau noeud 
		g->nxt[path+(g->nbstr>>1)].node=(Shu)node;	 // identifiant de noeud 
		g->nxt[path+(g->nbstr>>1)].next=(node_t)parent;// on colle l'ancien suivant du noeud parent au suivant du nouveau noeud  
	}
	
}

void Brin_insertNode(strgr * g,Shu parent,Shu enfant){
//	printf("relation %d : %s -> %s\n",g->nbbrin,g->listf[parent],g->listf[enfant]);
	g->nbbrin++;// nbre de noeud associé ++
//	if(parent != enfant){
	g->nxt[-g->nbbrin+(g->nbstr>>1)].poid=NONPATH; // NonPath pour parent
	g->nxt[g->nbbrin+(g->nbstr>>1)].poid=1; // 1 pour enfant
	if(parent==enfant)g->recursive[parent]=1;
	/*
	}else{
	g->nxt[-g->nbbrin+(g->nbstr>>1)].poid=0; // si c'est une fonction recursive
	g->nxt[g->nbbrin+(g->nbstr>>1)].poid=0; // enfant vaut 0 parent vaut 0
	}
	*/
	add_sub(g,-g->nbbrin,parent);// ajouter le parent
	add_sub(g,g->nbbrin,enfant);// ajouter l'enfant
	
}





extern strgr Brin_make(int maxN){
	int i;
	strgr g;
	g.nbs=maxN; // max noeud possible
	g.nbbrin=0; // nbre de noeud inseré
	g.nbnd=0; // nbre de noeud inseré
	g.nbstr=((maxN*4)+1) ; // nbre de tableau de brin
	g.node=(node_t *)malloc(sizeof(node_t)*g.nbs); // allouer le tableau de premier chemin 
	assert(g.node); // si c'est bien alloué
	g.declaration=(node_t *)malloc(sizeof(node_t)*g.nbs); // allouer le tableau de declaration
	assert(g.declaration); // si c'est bien alloué
	g.recursive=(node_t *)malloc(sizeof(node_t)*g.nbs); // allouer le tableau de declaration
	assert(g.recursive); // si c'est bien alloué
	g.nxt=(strand *)malloc(sizeof(strand)*g.nbstr); // allouer le tableau de brin
	assert(g.nxt); // si c'est bien alloué
	memset(g.node,0,sizeof(node_t)*g.nbs); // initialiser par 0
	g.appel=(node_t *)malloc(sizeof(node_t)*g.nbs); // allouer le tableau de premier chemin 
	assert(g.appel); // si c'est bien alloué
	
	for(i=0;i<g.nbs;i++){	 // initialiser par 0
		g.declaration[i]=0;
		g.recursive[i]=0;
	}
	for(i=0;i<g.nbstr;i++){		 // initialiser par 0
		g.nxt[i].node=0; 
		g.nxt[i].next=0; 
		g.nxt[i].poid=NONPATH;
	}
	
	return g; // retourner le graphe
}




extern strgr Brin_parser(char * path,strgr g){

  int c=0,prec=0,precNonSpace=0; //c == character actuel ,prec == caracter suivant
  FILE * f; // fichier source
  char * Nomfonction; // le nom de fonction
  int enfant; // l'indice de noeud enfant
  f=fopen(path,"r+"); // ouvrir le code source
  assert(f); // check erreur
  while((c=fgetc(f))!=EOF){ // parcourir jusqu'à la fin de fichier
    if(COMMENTAIRE_T1==FALSE && prec=='/'&& c=='*') // SI char precedent == '/' actuel == '*',ouverture de commentaire 
      COMMENTAIRE_T1=TRUE; // on active le commenatire
    if(COMMENTAIRE_T1==TRUE && prec=='*'&& c=='/')// SI char precedent == '*' actuel == '/', fermeture de commentaire
      COMMENTAIRE_T1=FALSE; // on désactive le commenatire

    
    if(COMMENTAIRE_T2==FALSE && c=='/'&&prec=='/')// SI char precedent == '/' actuel == '/', ouverture de commentair
      COMMENTAIRE_T2=TRUE;// on active le commenatire
    if(COMMENTAIRE_T2==TRUE && c=='\n')// SI char actuel ='\n', fermeture de commentaire
      COMMENTAIRE_T2=FALSE;// on désactive le commenatire
//printf("%d %c\n",c,c);

    
    if(COMMENTAIRE_T1==FALSE&&COMMENTAIRE_T2==FALSE){// S'il n'y a pas le commentaire
      if(c=='(') // la fin de fonction se détermine par une parenthése ouvrante sauf le cast
		if(isNameChar(precNonSpace/*getLastCharWithoutSpace(f)*/)==1){ // si c'est une cast ou non
		
			Nomfonction=getName(f,precNonSpace); // prendre le nom de fonction
			if(!exception(Nomfonction)){// verifier si le nom de fonction est un mot clé 
	   			if((enfant=isInGraphBrin(g,Nomfonction))==-1)//fonction graph
	   				enfant = addGraphBrin(&g,Nomfonction); //fonction graph
	   	   
				if(!ACCOLADE){ // si accolade n'existe pas on est le niveau de parent
	    		
				if(g.declaration[PARENT]==0)g.declaration[PARENT]=1;
					PARENT=enfant; // s'il y a pas d'accolade, on n'est pas dans la definition de fonction, donc c'est un parent
	    		}else{
						Brin_insertNode(&g,PARENT,enfant); //inserer l'enfant dans la definition de fonction du parent
	    			g.appel[enfant]++;
	    		}
	  		}
		}
    
    	if(c=='{')// si on trouve une accolade ouvrante dans la fonction
			ACCOLADE++; // accolade ++
    	if(c=='}')// si on trouve une accolade fermente dans la fonction
			ACCOLADE--; // accolade --
    }
    //////////////// S'il n'y a pas le commentaire
    if(c!=' '&&c!='\n'&&c!='\t')precNonSpace=c;
    prec=c; // precedent = char actuel
 //   printf("prec non space %c\n",precNonSpace);
  }
  
  fclose(f); // liberer le code source
  return g; // retourner la structure
}


extern void Brin_affiche(strgr g){
	int i;
	printf("premier brin\n");
	for(i=0;i<g.nbs;i++)
	printf("%2d ",g.node[i]);
	printf("\ntableau brin");
	printf("\n     ");
	for(i=0;i<g.nbstr/2;i++)
	printf("%2d ",-((g.nbstr/2)-i));		
	
	for(i=0;i<g.nbstr/2+1;i++)
	printf("%2d ",i);		
	printf("\nnode :");
	for(i=0;i<g.nbstr;i++)
	printf("%2d ",g.nxt[i].node);
	printf("\nnext :");
	for(i=0;i<g.nbstr;i++)
	printf("%2d ",g.nxt[i].next);
	
	printf("\n");
}



static int path_sub(strgr g,Shu * visit,Shu dep,Shu arr){
	node_t start,current,middle=(g.nbstr>>1); // acceder au noeud courant
	Shu suivant;

	if(dep==arr)// si on a trouvé le chemin
	  return 1;// on retourne le nombre de visite
		
	visit[dep]=1;// on visite
	current=g.node[dep];
	start=current;
	do{
	if(g.nxt[-current+middle].poid>0)return 0;
	suivant=g.nxt[-current+middle].node;
	if(visit[suivant]==0)
	  if(path_sub(g,visit,suivant,arr)==1){
	     	return 1;
		}
	     current=g.nxt[current+middle].next;
	}while(current!=start);	  

	return 0;// pas de chemin
}




extern int Brin_ispath(strgr g,Shu dep,Shu arr){
	Shu * visit;
	int res;
	if(g.node[dep]==0){
		printf("node : '%s()' contient aucune relation avec autre noued\n",g.listf[dep]);	
	return 0;
	}
	visit=(Shu *)malloc(sizeof(Shu)*g.nbnd); // alloué le tabeau de visite
	assert(visit); // verifier le memoire alloué
	memset(visit,0,sizeof(Shu)*g.nbnd);//initialiser par 0


//	printf("chemin? '%s()' -> '%s()'\n",g.listf[dep],g.listf[arr]);
	res=path_sub(g,visit,dep,arr); // appeler sous fonction
//        printf("%s\n",res? "path":"non path");
	return res?1:0;
	free(visit); // liberer le memoire
	
}


static int findPath_sub(strgr g,Shu * visit,Shu * path,Shu dep,Shu arr,int etage){
	node_t middle=(g.nbstr>>1); // acceder au noeud courant
	node_t start,current;
	Shu suivant;
	int res;
	
	if(dep==arr)// si on a trouvé le chemin
	  return etage;// on retourne le nombre de visite
		
	visit[dep]=etage++;// on visite


	start=current=g.node[dep];
	do{

	suivant=g.nxt[-current+middle].node;

	if(g.nxt[-current+middle].poid>NONPATH)
		if(visit[suivant]==0)
		  if((res=findPath_sub(g,visit,path,suivant,arr,etage))){
			path[etage-1]=suivant;	
		     return res;
	}
	current=g.nxt[current+middle].next;
	}while(current!=start);	  

	return 0;// pas de chemin
}




extern void Brin_findPath(strgr g,Shu dep,Shu arr){
	Shu * visit,* path;
	int res,i;
	if(g.node[dep]==0){
		printf("node : '%s()' contient aucune relation avec autre noued\n",g.listf[dep]);	
	return;
	}

	visit=(Shu *)malloc(sizeof(Shu)*g.nbnd); // alloué le tabeau de visite
	assert(visit); // verifier le memoire alloué
	memset(visit,0,sizeof(Shu)*g.nbnd);//initialiser par 0

	path=(Shu *)malloc(sizeof(Shu)*g.nbnd); // alloué le tabeau de visite
	assert(path); // verifier le memoire alloué
	memset(path,0,sizeof(Shu)*g.nbnd);//initialiser par 0

	printf("chemin? '%s()' -> '%s()'\n",g.listf[dep],g.listf[arr]);
	res=findPath_sub(g,visit,path,dep,arr,1); // appeler sous fonction

	if(res){
	printf("le chemin existe\n");
	path[0]=dep;
		for(i=0;i<res;i++)
	printf("%d '%s' ",path[i],g.listf[path[i]]);	
	printf("\n");
	}else
	printf("non path\n");

	free(path); // liberer le memoire
	free(visit); // liberer le memoire
	
}

static int connexe_sub(strgr g,Shu * visit,Shu parcour,Shu mark){
	node_t start,current,middle=(g.nbstr>>1); // acceder au noeud courant
	Shu suivant;
	int res=0;
	if(g.node[parcour]==0)return 0;
	if(visit[parcour]!=0){
		if(visit[parcour]<mark){// si le parent visite l'enfant qu'on avait déjà compté
			return 1; 
		}
		else if(visit[parcour]==mark)
			return 0; // le parcour dans la même connexe
	}

	visit[parcour]=mark;// on visite
	current=g.node[parcour];
	start=current;
	do{
	suivant=g.nxt[-current+middle].node;
	if(visit[suivant]==0){
	  res+=connexe_sub(g,visit,suivant,mark);
	}
	     current=g.nxt[current+middle].next;
	}while(current!=start);	  

	return res? 1: 0;
}




extern int Brin_connexe(strgr g){
	Shu * visit;
	int res,i;
	visit=(Shu *)malloc(sizeof(Shu)*g.nbnd); // alloué le tabeau de visite
	assert(visit); // verifier le memoire alloué
	memset(visit,0,sizeof(Shu)*g.nbnd);//initialiser par 0

	for(i=0,res=0;i<g.nbnd;i++)	
	if(connexe_sub(g,visit,i,i+1)==0)
		res++;
/*
for(i=0;i<g.nbnd;i++)
printf("%d ",g.node[i]);
for(i=0;i<g.nbnd;i++)
printf("%d ",visit[i]);
*/
	//printf("nb connexe %d %d\n",res,g.nbnd);
	free(visit); // liberer le memoire
	return res;
}





extern void Brin_afficheSucesseur(strgr g){
	node_t x,y;
	Shu z;
	int i;
	for(i=0;i<g.nbnd;i++){
		if(g.listf[i]!=NULL){
			x=g.node[i];
			y=x;
			printf("(PNT) : %s()\n",g.listf[i]);
			do{
			z=g.nxt[-x+(g.nbstr>>1)].node;
			if(g.nxt[-x+(g.nbstr>>1)].poid>0)
				printf("|______(ENF) : %s()\n",g.listf[z]);
			x=g.nxt[x+(g.nbstr>>1)].next;
			}while(x!=y);
			printf("\n");
		}
	}
}




static int findRoot_sub(strgr g,Shu * visit,Shu dep,int etage,int max){
	node_t middle=(g.nbstr>>1); // acceder au noeud courant
	node_t start,current;
	Shu suivant;
	int res;
	visit[dep]=etage++;// on visite

	start=current=g.node[dep];
	do{

	suivant=g.nxt[-current+middle].node;

	if(g.nxt[-current+middle].poid>NONPATH)
		if(visit[suivant]==0)
		  if((res=findRoot_sub(g,visit,suivant,etage,max)))	
			if(res>max)max=res;

	
	current=g.nxt[current+middle].next;
	}while(current!=start);	  

	return etage > max? etage: max;// pas de chemin
}




static int Brin_findRoot(strgr g){
	Shu * visit;
	int profondeur,i,max,maxi;

	visit=(Shu *)malloc(sizeof(Shu)*g.nbnd); // alloué le tabeau de visite
	assert(visit); // verifier le memoire alloué
	memset(visit,0,sizeof(Shu)*g.nbnd);//initialiser par 0

	for(i=0,max=0,maxi=0;i<g.nbnd;i++){
	profondeur=findRoot_sub(g,visit,i,1,0); // appeler sous fonction
	memset(visit,0,sizeof(Shu)*g.nbnd);//initialiser par 0
	if(profondeur>max){
		maxi=i;
		max=profondeur;
		}
	}
	//printf("%s() : est la fonction principale\n",g.listf[maxi]);
	free(visit); // liberer le memoire
	return maxi;
}


extern void print_sub(strgr g,Shu dep,int etage,node_t *  visite){
	node_t middle=(g.nbstr>>1); // acceder au noeud courant
	node_t start,current;
	Shu suivant;

	if(visite[dep]==g.appel[dep]+1)return;
	visite[dep]++;
	espace(etage);
	if(etage==1)
	  printf("(ROOT %d) %s \n",dep,g.listf[dep]);
	else
	  printf("(FUNC %d) %s \n",dep,g.listf[dep]);
	start=current=g.node[dep];
	do{
	suivant=g.nxt[-current+middle].node;
	if(g.nxt[-current+middle].poid>NONPATH)
		  print_sub(g,suivant,etage+1,visite);
	
	current=g.nxt[current+middle].next;
	}while(current!=start);	  

	return;// pas de chemin
}




extern void Brin_printRoot(strgr g){
node_t * visite;
visite=(node_t *)malloc(sizeof(node_t)*g.nbs);
assert(visite);
memset(visite,0,sizeof(visite)*g.nbs);


	print_sub(g,Brin_findRoot(g),1,visite);
}



extern void Brin_infoGraph(strgr g){
	int i;
  	for(i=0;i<g.nbs;i++)
  		if(g.listf[i]!=NULL)
  	printf("ID : %d NAME : %s()\n",i,g.listf[i]);
  	printf("######## INFO GRAPH ACTUEL #######\n");
	printf("Max brin %d, MAX node %d\n",g.nbstr,g.nbs);
	printf("NB brin %d, NB noeud %d\n",g.nbbrin*2,g.nbnd);
	printf("Espace : %dN \nProportion du noued et du brin :%f%%\n",g.nbstr/g.nbs,(g.nbbrin/(float)g.nbnd)*100);
  	printf("Taux de graphe %f\n",(g.nbbrin/(float)(g.nbnd*g.nbnd))*100);
  	
  	printf("######## INFO GRAPH ACTUEL #######\n");
}



extern void Brin_freeMemory(strgr * g){
	int i;
	for(i=0;i<g->nbs;i++) //parcourir tous les noms de fonctions
		if(g->listf[i]!=NULL) // si ca exisite
	free(g->listf[i]);// liberer le mémoire
	free(g->node);// liberer le mémoire
	free(g->nxt);// liberer le mémoire
}




