
#include"analyser_C.h"

extern void espace(int nb){
	int i;
	for(i=0;i<nb;i++){
	if(i==nb-1)printf("|_______");
	else printf("|         ");
	}
}


extern void espace2(int nb){
	int i;
	for(i=0;i<nb;i++)
	printf("        ");
	
}
extern int isNameChar(int c){
  if(isdigit(c)!=0)return 1;
  if(isalpha(c)!=0)return 1;
  if(c=='_')return 1;
  return 0;
}

extern char * getName(FILE * f,int prec){
  int i,c,nbc=0;
  char * name;
 while(c!=prec){
    fseek(f,-2,SEEK_CUR); // reculer le curseur -2 == verifier le char precedent
    c=fgetc(f); // prendre le char precedent
 }

do{
  	
    nbc++;
    fseek(f,-2,SEEK_CUR);// reculer le curseur -2 == verifier le char precedent
    c=fgetc(f); // prendre le char precedent
  }while(isNameChar(c)==1);  ///////////// reculer au debut du nom de fonction
	 
	
  name=(char *)malloc(sizeof(char)*nbc+1); // nbc + 1 pour la place de 0 à la fin     
  for(i=0;'('!=(c=fgetc(f));i++){//parcourir du debut du nom de fonction à la fin du nom de fonction 
 	name[i]=c; // affecte chaque charactere
  }
  name[nbc]=0; // fin de chaine

  return name; // retourner un pointer qui contient le nom de fonction

}

extern int exception(char * name){
 int i;
 char * listexpt[7]={"return","sizeof","for","if","while",NULL}; //liste d'exception possible ?for, if, while aussi consière comme l'exception?
 for(i=0;listexpt[i]!=NULL;i++){ //parcourir jusqu'a la fin de listexpt
   if(strcmp(listexpt[i],name)==0)// si on trouve 
     return 1;// retourner 1 true
 }
 return 0; // retourner 0 false
}

