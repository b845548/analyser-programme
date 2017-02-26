
#include"analyser_C.h"
#include<time.h>
static int ACCOLADE=0;
static int PARENT=0;
static int COMMENTAIRE_T1=0;
static int COMMENTAIRE_T2=0;

extern graph_mat Mat_createGraph(){
	graph_mat gm;
	int i,size= MAT_MAX * MAT_MAX; // taille de matrice
	gm.nbNode=0;// init nb noeud = 0
	gm.lineMax=MAT_MAX; // le max noeud possible
	for(i=0;i<size;i++){
		if(i<MAT_MAX){
			gm.declaration[i]=0;// chaque case represente la declaration de chaque fonction	
			gm.appel[i]=0;

		}
		gm.matrice[i]=0;// tous les arretes = 0 (non connecté)	
	}
	return gm;
}
extern int Mat_addGraph(graph_mat * gm,char * fname){
	gm->listf[gm->nbNode]=fname; // ajouter le nom de fonction
	gm->nbNode++; // le nombre de noeud += 1
	return gm->nbNode - 1; // renvoyer son indice dans le graphe
}


extern int Mat_insertNode(graph_mat * gm,int parent,int enfant){
	gm->matrice[gm->lineMax*parent+enfant]=1;//associer du parent a l'enfant
	return enfant;
}

extern int Mat_isInGraph(graph_mat gm,char * fname){
	int i;
 	for(i=0;i<gm.nbNode;i++) // parcours tous les noeud 
  		if(gm.listf[i]!=NULL) // le nom de fonction existe
 	   		if(strcmp(fname,gm.listf[i])==0)
				return i; // si on trouve le nom de fonction dans la liste
	return -1; // si on ne trouve pas 
	
}



extern graph_mat MAT_parser(char * path,graph_mat gm){
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
//	printf("%d %c\n",c,c);

    
    if(COMMENTAIRE_T1==FALSE&&COMMENTAIRE_T2==FALSE){// S'il n'y a pas le commentaire
      if(c=='(') // la fin de fonction se détermine par une parenthése ouvrante sauf le cast
		if(isNameChar(precNonSpace/*getLastCharWithoutSpace(f)*/)==1){ // si c'est une cast ou non
		
			Nomfonction=getName(f,precNonSpace); // prendre le nom de fonction
//	   	   	printf("%s\n",Nomfonction);

			if(!exception(Nomfonction)){// verifier si le nom de fonction est un mot clé 
	   			if((enfant=Mat_isInGraph(gm,Nomfonction))==-1)//fonction graph
	   				enfant =Mat_addGraph(&gm,Nomfonction); //fonction graph
				if(!ACCOLADE){ // si accolade n'existe pas on est le niveau de parent
	    		
				if(gm.declaration[PARENT]==0)gm.declaration[PARENT]=1;
					PARENT=enfant; // s'il y a pas d'accolade, on n'est pas dans la definition de fonction, donc c'est un parent
	    		}else{
					Mat_insertNode(&gm,PARENT,enfant); //inserer l'enfant dans la definition de fonction du parent
	    			gm.appel[enfant]++;
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
  return gm; // retourner la structure
}
extern int Mat_graphInfo(graph_mat gm){
	int i;
	printf("NB node : %d\n",gm.nbNode);
	printf("MAX node : %d\n",gm.lineMax);
	for(i=0;i<gm.nbNode;i++)
	printf("FUNC %d : %s\n",i,gm.listf[i]);
return 0;
}


static void Mat_subAffiche(graph_mat gm,int courant,int *visite,int prof){
	int suc;
	if(visite[courant]==gm.appel[courant]+1)return;// deja visité ou non?
	visite[courant]++;// marquer la visite du noeud courant
	espace(prof);// afficher le profondeur
	prof++;// profondeur += 1
	printf("(FUNC %d) %s\n",courant,gm.listf[courant]);// affiche le nom de fonction
	for(suc=0;suc<gm.nbNode;suc++) // parcourir tous les successeurs
		if(gm.matrice[gm.lineMax*courant+suc]==1)// s'il y a un arret?
			Mat_subAffiche(gm,suc,visite,prof);// visiter le successeur
		
}

extern void Mat_affiche(graph_mat gm,int node){
	int * visite;
	visite=(int *)malloc(sizeof(int)*gm.nbNode); // reserver la memoire pour le tableau de visite
	assert(visite); // verifier l'allocation du memoire
	memset(visite,0,sizeof(int)*gm.nbNode); // init a 0
	Mat_subAffiche(gm,node,visite,0); // appeller sous fonction 
	free(visite); // liberer la memoire
	return;
}


static int Mat_subProfondeur(graph_mat gm,int courant,int *visite,int prof){
	int suc;
	int cmpProf,maxProf=prof+1;
	if(visite[courant]==1)return prof;
	visite[courant]=1;
	prof++;
	for(suc=0;suc<gm.nbNode;suc++)
		if(gm.matrice[gm.lineMax*courant+suc]==1)
			if(visite[suc]==0){
				cmpProf=Mat_subProfondeur(gm,suc,visite,prof); 
			if(cmpProf>maxProf)maxProf=cmpProf;// comparer la maximum profondeur que le successeur a envoyé
		}
	return maxProf;// max profondeur
}

extern void Mat_profondeur(graph_mat gm){
	int i,max=0,cmp,maxi=-1;
	int * visite;
	visite=(int *)malloc(sizeof(int)*gm.nbNode);
	assert(visite);
	for(i=0;i<gm.nbNode;i++){
	memset(visite,0,sizeof(int)*gm.nbNode);
	cmp=Mat_subProfondeur(gm,i,visite,0);// si le 'i'eme noeud a le max profondeur ou non?
		if(cmp>max){
		maxi=i;// l'indice de max profondeur
		max=cmp;// le nombre de max profondeur
		}
	}
	Mat_affiche(gm,maxi);// le max profondeur 'i' == la fonction 'main'
	free(visite);
	return;
}




static int Mat_connexe_r(graph_mat g,int dep,int * visite,int deja){
	int enfant;	
	int res=0;
	if(visite[dep]!=0){
		if(visite[dep]<deja)// si le parent visite l'enfant qu'on avait déjà compté
			return 1;
		else if(visite[dep]==deja)
			return 0; // le parcour dans la même connexe
	}

		*(visite+dep)=deja;
			
	for(enfant=0;enfant<g.nbNode;enfant++)
		if(*(g.matrice+dep*g.lineMax+enfant))
			res+=Mat_connexe_r(g,enfant,visite,deja);
		
	
	for(enfant=0;enfant<g.nbNode;enfant++)
		if(*(g.matrice+enfant*g.lineMax+dep))
			res+=Mat_connexe_r(g,enfant,visite,deja);

	return res ? 1 : 0;

}


extern int Mat_connexe(graph_mat g){
	int * visit,i,res;
	visit=(int *)malloc(sizeof(int)*g.nbNode);
	assert(visit);
	memset(visit,0,g.nbNode*sizeof(int));
	
	for(i=0,res=1;i<g.nbNode;i++)
		if(!Mat_connexe_r(g,i,visit,res))
			res++;
	//for(i=0;i<g.nbNode;i++)
	//printf("%d : %d\n",i,visit[i]);	
//	free(visit);

	return res;
}






static int Mat_connexePoid_r(graph_mat g,int dep,int profondeur,float * coef,int * connexeTab){
	int aleatoire,debut,direction;
	if(profondeur>40)
		return 0;
	profondeur++;
	coef[dep]+=0.001;
	aleatoire = debut= rand()%g.nbNode;
	direction=rand()%2;
	while(connexeTab[aleatoire]!=connexeTab[dep]&&(g.matrice[aleatoire+g.lineMax*dep]||g.matrice[aleatoire*g.lineMax+dep])){
		if(direction){
		aleatoire++;
		aleatoire%=g.nbNode;
		}else{
		aleatoire--;
		if(aleatoire<0)aleatoire=g.nbNode-1;
		}
		if(aleatoire==debut)return 0;
	}

		if(!Mat_connexePoid_r(g,aleatoire,profondeur,coef,connexeTab))
			return 0;
	return  0;

}


extern void Mat_connexePoid(graph_mat g){
	int j,i,res,arret;	
	int * connexeTab;
	int * nbNodeInConnexe;
	float coef[MAT_MAX]={0.,};

	srand(time(NULL));

	connexeTab=(int *)malloc(sizeof(int)*g.nbNode);
	assert(connexeTab);
	memset(connexeTab,0,g.nbNode*sizeof(int));
	
	for(i=0,res=1;i<g.nbNode;i++)
		if(!Mat_connexe_r(g,i,connexeTab,res))
			res++;
	
	nbNodeInConnexe=(int *)malloc(sizeof(int)*res);
	assert(nbNodeInConnexe);
	memset(nbNodeInConnexe,0,res*sizeof(int));
	


	for(i=0;i<g.nbNode;i++)
		Mat_connexePoid_r(g,i,0,coef,connexeTab);
//	for(j=0;j<;j++)
	for(j=1;j<res;j++){
		printf("liste de %d er(eme) connexe :\n",j);
		for(i=0;i<g.nbNode;i++)
		if(j==connexeTab[i]){
			nbNodeInConnexe[j-1]++;
			printf("\t%s (poid) %f \n",g.listf[i],coef[i]);	
		}
	}


  	printf("######## INFO GRAPH ACTUEL #######\n");
	for(i=1;i<res;i++)
		printf("number of nodes in %d er(eme) connexe = %d \n",i,nbNodeInConnexe[i-1]);
	for(j=0,arret=0;j<g.nbNode;j++)
		for(i=0;i<g.nbNode;i++)
			if(g.matrice[g.lineMax*j+i]==1)arret++;
	

	printf("MAX Node %d NB node %d\n",g.lineMax,g.nbNode);
	printf("NB arret %d\n",arret);
	printf("Taux de graphe %.3f%%\n",(arret/(float)(g.nbNode*g.nbNode))*100);
  	printf("######## INFO GRAPH ACTUEL #######\n");


}
