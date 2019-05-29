// Programme of NaSch
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//  DECLARATION DES PARAMETRES
#define L 100   //  La taille de la route
#define L1 100   //  La taille de la route

#define MCMAX  500 // temps maximum mote carlo
#define MCALC 100  // temps de calcul
#define Moy 1     // la moyenne
//Moy=1 pour les diag spasio temporelle et Moy=100 pour les diagramme fondamentale
#define Vmax 5 // vitesse maximum (varie avec les question )

#define pb 0. // probabilite de freinage
#define pb1 0.
#define den 0.2 // la densite est fixe seulement pour les diagramme spasio temporelle
#define den1 1. 
//donc il faut l'eliminer pour tracer les diag fondamentales et il faut le varier dans une boucle

// fonction maximum entre deux valeurs
#define prob_desord 0.8 //// probabilite de desordre


int max ( int a,int b)
{
if(a>=b)
return a;
else
return b;
}
// fonction minimum entre deux valeurs
int min ( int a,int b)
{
if(a>=b)
return b;
else
return a;
}
// initialisation du tableau des positions
int initiale (int T[],int P[])
{  int is=0;
	for(int i=1;i<=L;i++)
	{
		if(T[i]!=0)
		{
			is=is+1;
			P[is]=i;
		}
	}
	P[is+1]=P[1];

	
	return	is; // is est le nombre de vehicules dans le system
}

	int initiale1 (int T1[],int P1[])
{  int is=0;
		for(int i=1;i<=L1;i++)
	{
		if(T1[i]!=0)
		{
			is=is+1;
			P1[is]=i;
		}
	}
	P1[is+1]=P1[1];
return	is; // is est le nombre de vehicules dans le system
}


// deplacement des vehicules
int deplacement(int T[],int P[],int V[],int npp)
{ //	srand(time(NULL));

	int gap,Kf,K,COU;
  	COU=0;
	float r;
	
	

for(int i=1;i<=npp;i++)	{
	 K=P[i];
 if(i!=npp)
 {
 	gap=P[i+1]-P[i]-1; // calcul de la distance entre les vehicules
 }
 else
 { gap=(P[1]+L)-P[i]-1; // calcul de la distance entre les vehicules
 }
 //***********************codition tres important**********************************




V[K]=min(V[K]+1,Vmax);
V[K]=min(V[K],gap);


r=float(rand())/float(RAND_MAX);

		if(r<pb){//prob de frenage
	V[K]=max(V[K]-1,0);  }

//******************************************************************************
if(V[K]>0){
 Kf=K+V[K];
 if(Kf>L){
Kf=Kf-L;
 }
 V[Kf]=V[K];
 T[Kf]=T[K];
 V[K]=0;
 T[K]=0;

 COU=COU+V[Kf];


}
}

return COU;
}


int deplacement1(int T1[],int P1[],int V1[],int npp1)
{ //	srand(time(NULL));

	int gap,Kf,K,COU1;
  	COU1=0;
	float r;
	
	

for(int i=1;i<=npp1;i++)	{
	 K=P1[i];
 if(i!=npp1)
 {
 	gap=P1[i+1]-P1[i]-1; // calcul de la distance entre les vehicules
 }
 else
 { gap=(P1[1]+L1)-P1[i]-1; // calcul de la distance entre les vehicules
 }
 //***********************codition tres important**********************************




V1[K]=min(V1[K]+1,Vmax);
V1[K]=min(V1[K],gap);


r=float(rand())/float(RAND_MAX);

		if(r<pb1){//prob de frenage
	V1[K]=max(V1[K]-1,0);  }

//******************************************************************************
if(V1[K]>0){
 Kf=K+V1[K];
 if(Kf>L1){
Kf=Kf-L1;
 }
 V1[Kf]=V1[K];
 T1[Kf]=T1[K];
 V1[K]=0;
 T1[K]=0;

 COU1=COU1+V1[Kf];


}
}

return COU1;
}





// fonction principale

int main()

{

   int T[L],i,jj,P[L],V[L];
   int T1[L1],P1[L1],V1[L1];
   int COUR,COUR1;
   float r,Velo,Courant,aa,ppp ;
   float Velo1,Courant1,aa1,ppp1 ;
    FILE *f;
    f = fopen("output.dat", "w+");
     FILE *f1;
    f1 = fopen("output1.dat", "w+");




  	 Velo=0;
   Courant=0;
   	 Velo1=0;
   Courant1=0;




   /*for( den=0;den<=1;den=den+0.01){//cette boucle pour tracer les diagramme fondamentale
   mais il faut eliminer "#define den 0.2"  et declarer den a l'interieur de main */

  	for(int mo=1;mo<=Moy;mo++) // debut de la boucle du moyenne
  {
   // vider les tableaux
  for(i=1;i<=L;i++){

  	T[i]=0;
  	P[i]=0;
  	V[i]=0;

  }

  for(i=1;i<=L1;i++){

  	T1[i]=0;
  	P1[i]=0;
  	V1[i]=0;

  }
 int N=int(den*L);
int N1=int(den1*L1);
  

  jj=0;
  // initialisation du tableau de vehicules
  while(jj<N)

  {
  	r=rand();
  	 int ip=int((r/RAND_MAX)*L)+1;
  	 if(T[ip]==0){
  	 	T[ip]=1;
  	 jj++;
  	 }

  }
  

  jj=0;
  // initialisation du tableau de vehicules
  while(jj<N1)

  {
  	r=rand();
  	 int ip=int((r/RAND_MAX)*L1)+1;
  	 if(T1[ip]==0){
  	 	T1[ip]=1;
  	 jj++;
  	 }

  }
 ppp=0;
  ppp1=0;
 // debut de la fonction du monte carlo
  for(int ii=0;ii<MCMAX;ii++)
  {
  	int npp=0;
  	int npp1=0;
  	// initialisation du tableau de position
  	npp=initiale(T,P);
  	 
  	npp1=initiale1(T1,P1);
  	 //printf("%d   \n",npp1);// velo=vitesse moyen
  	COUR=0;
  	COUR1=0;
  	// appel de la fonction deplacement
    COUR=deplacement(T,P,V,npp);
    COUR1=deplacement1(T1,P1,V1,npp1);

    printf("npp %d COUR %d\n", npp, COUR);
    

    // debut du calcul des parametres
    if(ii>MCALC)
      {
       aa=0;
       if(npp!=0)  {
        aa=float(COUR)/float(npp);} // vitesse
       Velo=Velo+aa;
       aa=float(COUR)/float(L); // courant
       ppp=ppp+1;
       Courant=Courant+aa;
    	
       aa1=0;
       if(npp1!=0)  {
        aa1=float(COUR1)/float(npp1);} // vitesse
       Velo1=Velo1+aa1;
       aa1=float(COUR1)/float(L1); // courant
       ppp1=ppp1+1;
       Courant1=Courant1+aa1;
       //**********************important**************************
       // affichage de tableau de vehicule seulement pour le diag spasio temporelle
       //	for(i=1;i<=N;i++)
       //	{fprintf(f,"%d \t",1-T[i]);}
       //	fprintf(f,"\n");
    	//affichage a eliminer pour le diagramme fondamentale

    }
}


   } //fermeture de la boucle de la densité
   
   float ss;
ss=float(Moy*(MCMAX-MCALC-1));
 	Velo=Velo/ss;
    	Courant=Courant/ss;
    	float ss1;
ss1=float(Moy*(MCMAX-MCALC-1));
 	Velo1=Velo1/ss1;
    	Courant1=Courant1/ss1;
    	/*/les deux affichage suivant sont a ajouter pour le diagramme fondamentale .*/
    	printf("%f   %f  %f \n",den,Courant,Velo);// velo=vitesse moyen
    	 fprintf(f, "%f %f %f \n",den,Courant,Velo); 
    	     	/*/les deux affichage suivant sont a ajouter pour le diagramme fondamentale .*/
    	printf("%f   %f  %f \n",den1,Courant1,Velo1);// velo=vitesse moyen
    	 fprintf(f1, "%f %f %f \n",den1,Courant1,Velo1); 

fclose(f);
fclose(f1);
return 0;

}





