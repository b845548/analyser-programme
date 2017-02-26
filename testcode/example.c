/*

commentaire
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#define MAXCHAR 100
#define MAXDIC 2048
#define MAXCODE 100000 

int _dicNB=0;// nombre de mot
char * _dic[MAXDIC];// tableau de mot

int _sortieNB=0;// nombre de resultat code
int _sortie[MAXCODE];// tableau de resultat code

int appelNullpart1(){
return 1;
}

int gest(){
	
play1();
}


int play1(){
play2();
}


int play2(){
gest();
}

int fib(int n){
if (n<2)
return 1;
return fib(n-2)+fib1(n-1);
}


int main2(){
fib(5);
}




int chercherMot(char * data,int taille){
	int i;
	for(i=0;i<_dicNB;i++)// parcourir tous les mots de dic avec le mot qu'on cherche
		if(strncmp(_dic[i],data,taille)==0)
			return 1;	
	return 0;
}
void ajouterDic(char * mot,int taille){
	assert(_dicNB<MAXDIC);
	_dic[_dicNB]=(char *)malloc(taille*sizeof(char));
	assert(_dic[_dicNB]!=NULL);
	strncpy(_dic[_dicNB],mot,taille);// copier le mot dans le dic
	_dicNB++;	// avancer l'indice
}
void ecrireCode(char * data,int taille){
	int i;
	for(i=0;i<_dicNB;i++)
		if(strncmp(data,_dic[i],taille)==0){
			printf("%d ",i);// afficher le resultat dans le terminal
			_sortie[_sortieNB++]=i;// ajouter le code dans le tableau de sortie
			break;	
		}
}

void makeDicFile(char * fileName){
	int i;
	char buf[100];
	FILE * f;
	
	strcpy(buf,fileName);
	strcat(buf,"(dictionary)");
	f=fopen(buf,"w");
	assert(f);
	for(i=256;i<_dicNB;i++){
	fprintf(f,"%s\n",_dic[i]);// ecrire les mots dans le fichjer *.dic ou (dictionary)
	printf("NO %d : %s\n",i,_dic[i]); // afficher le resultat dans le terminale
	}
	fclose(f);
}

void freeDic(){
int i;
for(i=0;i<_dicNB;i++)
free(_dic[i]);	// liberer la memoire de '_dic'
}


char makeBit(int neme){



}

void setBit (char tab[],int neme){
	tab[neme/8]|=makeBit(neme%8); // met a 1 le n eme bit de tab
}
void clrBit(char tab[],int neme){
	tab[neme/8]&= ~makeBit(neme%8); // met a 0  le n eme bit de tab
}

void compression(char * fileName,int compressBit){
	int i,j,indice,*tmp;
	char * encode, buf[100];
	FILE * f;
	strcpy(buf,fileName);
	strcat(buf,"(encoded)");
	f=fopen(buf,"w"); 
	assert(f);
	
	tmp=(int *)malloc(_sortieNB*sizeof(int));
	encode=(char *)malloc ((_sortieNB*compressBit/8+1)*sizeof(char));
		
	for(i=0;i<_sortieNB;i++)
	tmp[i]=_sortie[i];// recuperer les codes a ecrire

// conversion 'int *' 9~32 bits -> 'char *' 8bits
	for(j=0,indice=0;j<_sortieNB;j++)
		for(i=0;i<compressBit;i++,indice++,tmp[j]>>=1)
		if(tmp[j]&1)setBit(encode,indice);
		else clrBit(encode,indice);

// ecrire resultat 'char *' dans le fichier
for(i=0;i<_sortieNB*compressBit/8+1;i++)
	fprintf(f,"%c",encode[i]);

fclose(f);
free(tmp);
free(encode);
}
void HBB_encoding(char *data,char * outFileName){
	int i,j,taille;
	char mot[MAXCHAR];
	char nvchar;
		
	//// initialiser le dic entre 0 et 255(ascii)
	for(i=0;i<256;i++){
	_dic[i]=(char *)malloc(sizeof(char)); // une seule caractere
	assert(_dic[i]);
	_dic[i][0]=i;	// affecter la valeur de ascii code
	}
	_dicNB=i;
	
	
	printf("(start)");
	for(i=0,taille=1;data[i+taille]!=EOF;){
		for(;;){// la condition d'arret de ce boulce for est en bas
		assert(MAXCHAR>=taille);
		for(j=0;j<taille;j++)
			mot[j]=data[i+j];// copier le mot dans le buffer
		nvchar=data[i+taille];
		mot[taille++]=nvchar;// ajouter nouveau char
		if(chercherMot(mot,taille)==0){// si on ne trouve pas dans le dic
			ajouterDic(mot,taille); // ajouter la nouvelle chaine de charactere
			ecrireCode(mot,taille-1);// ecrire le code sauf le dernier caractere
			i+=taille-1;// passer la position suivante
			taille=1;// recommencer
			break;
			}
		if(data[i+taille]==EOF){// si on est la fin
			ecrireCode(mot,taille);// ecrire le code avec le dernier caractere
			break;	
			}
		}
	}
	printf("(end)\n");
	int compressBits,n;
	
	for(compressBits=9,n=512;n<=_dicNB;compressBits++,n*=2);
// calculer le bit de compression necessaire 
// si le nombre de mots dans le dic est superieure egale a 512, compBit=10
// si c'etait 1024, compBit=11, et compBit=12 pour 2048
	compression(outFileName,compressBits);
	makeDicFile(outFileName);
	freeDic();
	printf("ce fichier est compresse par %d bits\n",compressBits);
}
char * HBB_openFile(char * fileName){
	FILE * f;
	long size;
	char * buf;
	char bufName[100];
	
	strcpy(bufName,fileName);
	strcat(bufName,"(original)");
	f=fopen(bufName,"r");
	assert(f);
	fseek(f,0,SEEK_END);
	size=ftell(f);
	assert(size);
	rewind(f);
	buf=(char *)malloc(1+size*sizeof(char));
	memset(buf,0,1+size*sizeof(char));
	fread(buf,1,size+1,f);
	fclose(f);
	buf[size]=EOF;// marquer la fin
	return buf;

}

int appelNullpart2(){

}

int main(int args,char **argv){
	char fileName[]={"CompressionLZW"};//le nom de ficher
	char * data;
	/*if(args<2){
	fprintf(stderr,"Entrez le nom de fichier\n");
	return -1;
	}
	data=openFile(file,argv[1]);
	encoding(data,argv[1]);
	*/
	/*
	closefile();
	*/
	data=HBB_openFile(fileName);
	HBB_encoding(data,fileName);
	free(data);
	return 0;
}
