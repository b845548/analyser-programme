
#include"analyser_C.h"
#include"ANSIstandard.h"


extern void Brin_analyseError1(strgr g){
int i;
for(i=0;i<g.nbnd;i++)
if(g.node[i]==0)
printf("WARNING! Detect fonction non appelee : %s()\n",g.listf[i]);


}

/*
static void analyseError2_sub3(strgr g,Shu * visit,Shu dep,int etage){
	node_t middle=(g.nbstr>>1); // acceder au noeud courant
	node_t start,current;
	Shu suivant;
	if(visit[dep]!=0)return;
	visit[dep]++;
	
	espace(etage);
	printf("(FUNC %d) %s \n",dep,g.listf[dep]);
	start=current=g.node[dep];
	do{
	suivant=g.nxt[-current+middle].node;
	if(g.nxt[-current+middle].poid>NONPATH)
		  analyseError2_sub3(g,visit,suivant,etage+1);
	
	current=g.nxt[current+middle].next;
	}while(current!=start);	  

	return;// pas de chemin
}


static void analyseError2_sub2(strgr g,int nbid){
	Shu * visit;
	visit=(Shu *)malloc(sizeof(Shu)*g.nbnd); // alloué le tabeau de visite
	assert(visit); // verifier le memoire alloué
	memset(visit,0,sizeof(Shu)*g.nbnd);//initialiser par 0

	analyseError2_sub3(g,visit,nbid,1);
	//printf("nb connexe %d %d\n",res,g.nbnd);
	free(visit); // liberer le memoire
}
*/

static int analyseError2_sub(strgr g,Shu dep,int etage){
	node_t middle=(g.nbstr>>1); // acceder au noeud courant
	node_t start,current;
	Shu suivant;
	if(etage>MAXPROFONDEUR)return 1;
	start=current=g.node[dep];
	do{
	suivant=g.nxt[-current+middle].node;
	if(g.nxt[-current+middle].poid>NONPATH)
		 if(analyseError2_sub(g,suivant,etage+1)==1)
		 	return 1;
	
	current=g.nxt[current+middle].next;
	}while(current!=start);	  

	return 0;// pas de chemin
}


static int findStandardLibrary(char * fname){
int i,j;
char ** library[]={ANSI_ctype,ANSI_errono,ANSI_local,ANSI_math,ANSI_setjmp,
ANSI_signal,ANSI_stdarg,ANSI_time,ANSI_string,ANSI_stdio,ANSI_stdlib,ANSI_assert,NULL};
for(j=0;library[j]!=NULL;j++)
	for(i=0;library[j][i]!=NULL;i++)
		if(strcmp(fname,library[j][i])==0)return 1;
if(strcmp("main",fname)==0)return 1;
return 0;
}
extern void Brin_analyseError2(strgr g){
	int i;
	
	for(i=0;i<g.nbnd;i++)
		if(g.declaration[i]==0)
			if(findStandardLibrary(g.listf[i])==0)
			printf("WARNING! Detect fonction non declaree : %s()\n",g.listf[i]);

}


extern void Brin_analyseError3(strgr g){
	int i,res;
	node_t * visite;
	visite=(node_t *)malloc(sizeof(node_t)*g.nbs);
	assert(visite);

	for(i=0;i<g.nbnd;i++){
		res=analyseError2_sub(g,i,1);
		if(res==1){
			printf("WARNING! Detect fonction infinie : %s()\n",g.listf[i]);
			memset(visite,0,sizeof(visite)*g.nbs);
			print_sub(g,i,1,visite);

		 //analyseError2_sub2(g,i);
			printf("\n");
		}
	}

}

extern void Brin_analyseError4(strgr g){
	int i;
	for(i=0;i<g.nbnd;i++)
	if(g.recursive[i]==1)
	printf("WARNING! Detect fonction recursive : %s()\n",g.listf[i]);
	
}
