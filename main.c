#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Declaration Les TYPES Necessaire

//Fille dattente emprenteur
typedef struct fileAtt
{
       char emprenteur;
       struct fileAtt *suivant;
       struct fileAtt *precedent;
} fileAtt;

//Ptr La File des emprenteurs
typedef struct
{
      fileAtt *tete;
      fileAtt *queue;
} File;

//La Liste des emprenteurs
typedef struct empt
{
       char emprenteur;
       struct empt *suivant;
} empt;
typedef empt *Liste_Empt ;

//La Liste BD des Livres
typedef struct Livre
{
       struct Livre *suivant;
       struct Livre *precedent;
       char  *Auteur;
       char  *Titre;
       int NbTot;
       int NbDisp;
       empt  *Liste_Empt;
       File  File_Emp;
} Livre;

//Ptr de Livre
typedef struct
{
      Livre *tete;
      Livre *queue;
} ListeLivre;
//Fin des Types






//les fonctions Besoin

//initialisation de la table des caracteres
void init_tab (ListeLivre *Tab)
{
       int i;
       for(i=0;i<26;i++)
       {
        //initialisation des pointeur Tete et Queue sur NULL
           Tab[i].tete =NULL;
           Tab[i].queue =NULL;

       }
}

//Fonction qui Affiche la Table principale 4 par 4
void Affiche_Liste_Livre(ListeLivre Tab[26])
{
    Livre *k;
    int i;
    int affi ;
    int relire ;
    i=0 ;
	affi = 4 ;
	relire = 1 ;

    printf(" *******Votre Liste des Livres******* \n\n");
    while (relire==1)
    {
    	while (i<affi)
    	{
    		k=Tab[i].tete;
    		if (k==NULL)
    		{
    			printf ("il ne y'a pas de livre avec la lettre num %d \n\n", i+1 );
			}
			else
			{
            while(k!=NULL)
              {
                afficher_livre(k);
                k=k->suivant;
              }
           }
           i++;
        }
        if (affi<26)
        {
        	do
        	{
        		printf("Voulez vous continuer a Afficher les Quatre prochains caracteres (taper 1 si oui sinon taper 0): ");
                   scanf("%d",&relire);
            }while(relire<0 || relire>1);

			if (relire)
			{
				if (affi<24)
				{
					affi=affi+4;
				}
				else
				{
					affi=affi+2;
				}
			}

		}
		else
		{
			relire = 0 ;
		}
    }
    printf("\n");
}

//fonction qui crée un Noeud pour la création de la liste chainée Bidirectionnelle Livre
Livre *creerNoeud_Livre()
{
    Livre *p;
    p=(Livre *)malloc(sizeof(Livre));
    if (p==NULL)
    {
        exit(EXIT_FAILURE);
    }
    return p;
}

//Fonction qui Affiche les info de livre
void afficher_livre(Livre *p)
 {

        printf("Le Titre Du Livre : %s  \n ",p->Titre);
        printf("L auteur de ce Livre : %s   \n ",p->Auteur);
        printf("Le Nombre Total des livres : %d   \n ",p->NbTot);
        printf(" \n\n ***************************** \n\n");
 }

//Fonction qui return l'indice de caractere
int  indice_car(char titre[])
{
   int i;
   if (titre[0]  >= 97)
   {
        i=titre[0] -'a';
   }
  else
  {
       i=titre[0] -'A';
  }

   return i;
}
//Fonction qui insert un livre on gardant le trie de notre liste livre
void  sortedInsert(ListeLivre *Tab , Livre *p ,int i)
{

    Livre *precd , *pt;
// Si il ya pas de livre , on l'ajoure ...
    if (Tab[i].tete == NULL)
    {
        p->suivant = NULL;
        p->precedent = NULL;
        Tab[i].tete = p;
        Tab[i].queue = p;
    }
// s'il ya des livre , il faut d'abord chercher la position ou on l'ajoute
    else if (strcmp(Tab[i].tete->Titre , p->Titre)>=0  )
    {
        p->suivant = Tab[i].tete;
        p->precedent = NULL;
        Tab[i].tete->precedent = p;
        Tab[i].tete = p;
    }
    else
    {
        precd = Tab[i].tete;
        pt = Tab[i].tete->suivant;
        while (pt!=NULL && (strcmp(pt->Titre , p->Titre)<0))
        {
            precd = pt;
            pt = pt->suivant;
        }
        p->suivant = precd->suivant;
        p->precedent = precd;
        precd->suivant = p;
        if (p->suivant == NULL)   // Si P est le dernier element
            Tab[i].queue = p;
        else
            p->suivant->precedent = p;
    }

}
//fonction qui concatine le nom et le prenom avec un espace entre eux
void Concatiner_Espace(char *Nom ,const char *prenom)
   {
      strcat(Nom ," ");
      strcat(Nom ,prenom);
   }
//Fonction Qui remplit les elements de la liste BD des Livres
Livre* Remplir_Livre(char titre[],char Nom[],char Prenom[],int nbrt)
{
    int taille;
    char Auteur[25];
    Livre *p=creerNoeud_Livre();
    p->File_Emp.queue=NULL;
    p->File_Emp.tete=NULL;
    p->Liste_Empt=NULL;
    p->NbTot=nbrt;
    p->NbDisp=nbrt;
    Concatiner_Espace(Nom,Prenom);
    strcpy(Auteur,Nom);
    taille=strlen(Auteur);
    p->Auteur=(char *)malloc((taille+1)*sizeof(char));    //allouer dynamiquement un espace pr l'auteur
    if (p==NULL) //verification de lallocation dynamique pr l'auteur
       {
          exit(EXIT_FAILURE);
       }
    taille=strlen(titre);
    p->Titre=(char *)malloc((taille+1)*sizeof(char)); //allouer dynamiquement un espace pr le titre
    if (p==NULL) //verification de lallocation
       {
          exit(EXIT_FAILURE);
       }
   strcpy( p->Titre,titre);
   strcpy(p->Auteur,Nom);
return p;
}

//Fonction Qui remplit la liste Bd Generalle Apartir Du fichier
void remplire (ListeLivre Tab[] , char nomf[] )
{
    char titre[25],Nom[25],Prenom[25],auteur[25];
    char Ligne[25];
    int c,i,nbrt;
    Livre *p;
    FILE *f;

    f=fopen(nomf,"rt");
    if( f == NULL )
       {
           puts(" Erreur creation fichier !");
           puts(" Ce fichier n'existe pas !");
           exit(EXIT_FAILURE);
           return NULL ;
       }
    while(fgets(Ligne,25,f)!=NULL)
   {
       sscanf(Ligne, "%s %s %s %d ",titre,Nom,Prenom,&nbrt);//Lire apartir du fichier
       i=indice_car(titre);
       p=Remplir_Livre(titre,Nom,Prenom,nbrt);
       sortedInsert(Tab,p,i);
    }
   fclose(f);
}

//Rechercher un livre
Livre*  Rechercher_livre(char titre[],ListeLivre Tab[])
{
   int i= indice_car(titre);
   Livre *p;
   Livre *r=NULL;
   p=Tab[i].tete;
   printf(" \n\n");
    while (p!=NULL)
   {
   	if(strcmp(p->Titre,titre)>0)
   	{
   		printf("desole ce livre n'existe pas chez nous .\n");
   		return r ;
		//exit(EXIT_FAILURE);

	}
	else
	{
    if (strcmp(p->Titre,titre)==0)
    {
        r=p;
        return r;
    }
    p=p->suivant;
    }
   }
   printf("desole ce livre n'existe pas chez nous .\n");
   //r=NULL;
   exit(EXIT_FAILURE);
   //return r ;

}

//fonction rechrche 2 pour l'utiliser a la fonction qui ajoute un Nv livre a cause d'un prblm
Livre*  Rechercher_livre_2(char titre[],ListeLivre Tab[])
{
   int i= indice_car(titre);
   Livre *p;
   Livre *r=NULL;
   p=Tab[i].tete;
   printf(" \n\n");
    while (p!=NULL)
   {
   	if(strcmp(p->Titre,titre)>0)
   	{
   		printf("desole ce livre n'existe pas chez nous .\n");
   		return r ;
	}
	else
	{
    if (strcmp(p->Titre,titre)==0)
    {
        r=p;
        return r;
    }
    p=p->suivant;
    }
   }
   printf("desole ce livre n'existe pas chez nous .\n");
}

 //fontion qui crée un Noeud pour la création de la liste chainée Emprenteur
empt *CreeNoeudEmpt()
   {
       empt *q=malloc(sizeof(empt));
       if(q == NULL)
       {
           exit(EXIT_FAILURE);
       }
       return q;
   }

//fonction qui crée un Noeud pour la création de la liste chainée Bidirectionnelle File d'attente
File *creerNoeudFile()
{
    File *p;
    p=(File *)malloc(sizeof(File));
    if (p==NULL)
    {
        exit(EXIT_FAILURE);
    }
    return p;
}
char  suprimer_FileAtt(fileAtt *p);

//Fonction qui ajoute une copie d'un livre existant
void ajouter_copie_Livre(ListeLivre Tab[],char titre[])
{
	Livre *p ;
	char T[50] ;
	p=Rechercher_livre(titre, Tab);
	p->NbTot++;
	if (p->File_Emp.tete == NULL)
	{
		p->NbDisp++;
	}
	else
	{
		strcpy(T,suprimer_FileAtt(p->File_Emp.tete));
		ajoutQ(p->Liste_Empt,T);
	}
    printf("Votre Copie a ete ajoute :) \n");
}
//fonction qui supprime un emprenteur du tete du file d'attente
char  suprimer_FileAtt(fileAtt *p)
{
	char NP[50];
    fileAtt *q ;
    q=p;
    p=p->suivant;
    strcpy(NP,q->emprenteur);
    free(q);
    return NP ;
}

//fonction qui ajoute un emprenteur a la liste des emprenteurs
void ajoutQ(empt *q, char NP[])
{
    empt *p ;
    p=q;
    if(p==NULL)
    {
       p=CreeNoeudEmpt();
       p->emprenteur=NP;
       p->suivant=NULL;
    }
    else
    {
    while(p->suivant!=NULL)
    {
        p=p->suivant;
    }
    p->suivant=CreeNoeudEmpt();
    p->suivant->emprenteur=NP;
    }
}

//fonction qui ajoute un nouveau Livre
void ajout_nv_livre(ListeLivre Tab[])
{
	char titre[25],Nom[25],Prenom[25],auteur[25];
    int x,i,nbrt;
    Livre *p;
    Livre *q;

    printf("Donnez Le titre du livre : ");
    scanf("%s",&titre);
    q=Rechercher_livre_2(titre,Tab);
    printf("\nDonnez Le nom de l'auteur' : ");
    scanf("%s",&Nom);
    printf("\nDonnez le prenom de l'auteur' : ");
    scanf("%s",&Prenom);
    printf("\nDonnez Le nombre du livre: ");
    scanf("%d",&nbrt);
    i=indice_car(titre);
    p=Remplir_Livre(titre,Nom,Prenom,nbrt);
    sortedInsert(Tab,p,i);


    if(q!=NULL) //(q!=NULL)
    {
    	printf("ce livre existe deja , voulez vous ajouter une copie de ce livre ?\n si oui tapez : 1 Sinon : 0\n");
    	scanf("%d",&x);
    	if(x==1)
    	{
    		ajouter_copie_Livre(Tab,titre);
		}
		else
		{
		    exit(EXIT_FAILURE);
		}
	}

    printf("\nVotre Livre a ete bien ajoute :) \n");
}

//fonction qui ajoute un emprenteur a la file d'attente
void ajoute_fil(File F, char NP[] )
{
    fileAtt *p=creerNoeudFile() ;
     p->emprenteur=NP;
     p->suivant = NULL;
    /* voir si la file d'attente est vide */
    if (F.queue == NULL)
    {
        F.queue = p;
    }
    else
    {
        F.queue->suivant = p;
    }
    p->precedent = F.queue;
    F.queue = p;
}


//Fonction qui emprente un livre
void ajout_emprenteur(ListeLivre Tab [] ,char NP[] , char titre[])
{
	Livre *q ;
	q=Rechercher_livre(titre,Tab);

	if (q->NbDisp!=0)
	{
		q->NbDisp=q->NbDisp-1;
		ajoutQ(q->Liste_Empt,NP);
		printf("vous devez rendre le livre avant la date xx/xx/xxxx \n");
	}
	else
	{
		ajoute_fil(q->File_Emp,NP);
		printf("%s il ya pas de livre dispo maintenant ,vous etes ajoutes a la file dattente \n",NP);
	}
}

//Fonction recherche dans la liste des emprunteurs
int recherche_empt(char NP[],Liste_Empt p)
{
	empt *q;
	q=p;
	while (q!=NULL)
	{
		if (q->emprenteur==NP)
		{
			return 1 ;
		}
		else
		{
			q=q->suivant;
		}
	}
	return 0;
}


//Fonction qui Supprime un emprunteur de la liste des emprenteurs
void supp_empt(Liste_Empt p , char NP[])
{
	empt *q;
    if ((p->suivant==NULL) && (p->emprenteur==NP))
    {
    	p=NULL;
	}
   	while (p->suivant!=NULL)
   	{
     	q = p->suivant;
     	if (q->emprenteur==NP)
        {
	        p->suivant = q->suivant ;
           free(q) ;
           printf("vous avez rendu votre livre , merci  \n");
        }
        else
       {
    	 p=p->suivant;
     	}
    }
}


//Fonction qui permet de rendre un livre
void rendre_Livre(ListeLivre Tab[],char titre[],char NP[])
{
	Livre *p ;
	char T[50] ;
	p=Rechercher_livre(titre, Tab);
	void supp_empt(p, NP);
	if (p->File_Emp.tete == NULL)
	{
		p->NbDisp++;
	}
	else
	{
		strcpy(T,suprimer_FileAtt(p->File_Emp.tete));
		ajoutQ(p->Liste_Empt,T);
	}
	printf ("vous avez rendu le livre , merci \n");
}

//Menu
int Menu()
{
int choix;
  printf("\t\t*************MENU*************\n");
  printf("1-Afficher la liste de tous les livres sur ecran (4 car pas 4 ) .\n");
  printf("2-Rechercher un livre et afficher toutes ses informations . \n");
  printf("3-Ajouter une copie d un livre existant .\n");
  printf("4-Ajouter un nouveau livre .\n");
  printf("5-Emprunter un livre .\n");
  printf("6-Rendre un Livre .\n");
  printf("7-Quitter le programme .\n\n");
  do
  {
     printf(" Choisissez Votre Choix S'il vous plait : ");
     scanf("%d",&choix);
  }while(choix<=0 || choix>7 );
   return choix;
}

//QUIT
int Quit( ListeLivre *pTab )
   {
       Livre *p = NULL;
       Liste_Empt q;
       File F  ;
       int c= 0, i= 0;
       int y ;

       do
       {
           printf("\n Voulez vous vraiment quitter le programme ? ( 1 : Oui    2 : Non ) : ");
           scanf("%d", &c);
       }
       while( c != 1 && c != 2 );

       if( c == 1 )
       {
       	return 1 ;
	   }
       /*{
           for(i=0;i<26;i++)
           {
               p = pTab[i].tete;
               while( p != NULL ) //On libere les espace alloués pour Le Livre
               {
                    q = p->Liste_Empt; //On libère les espaces alloués pour les Listes emprunteurs
                   while( q != NULL )
                   {
                       q = q->suivant;
                   }
                   F=p->File_Emp;
                   while (F.tete != NULL)
                   {
                   	F.tete->precedent=NULL;
                   	F.tete->suivant=NULL;
				   }
                   free(p->Auteur);
                   free(p->Titre);
                   p = p->suivant;
               }
               free(pTab[i].tete);
               free(pTab[i].queue);
               i++;
           }
           return 1;
       }*/
       else
       {
          return 0 ;
       }
   }


//FIN des Fonctions


int main()
{
    int choixMenu;
    ListeLivre Tab[26];
    init_tab(Tab);
    Livre *p;
    char titre[25];
    char NomFichier[20];
    char DW[25];
    int again =1  ;
    int x;
    ListeLivre *t ;

  printf(" Introduisez le nom de votre fichier svp ( Format 'nomFichier.txt' ) : ");
  scanf("%s",&NomFichier);
  remplire(Tab,NomFichier);

 while (again)
 {
  choixMenu=Menu();
  printf("\n");

  switch (choixMenu)
  {

    case 1:
       Affiche_Liste_Livre(Tab);

        break ;

    case 2:
      printf("donnez le titre du livre s'il vous plait : ");
      scanf("%s",titre);
      p= Rechercher_livre(titre,Tab);
      afficher_livre(p);

        break ;

    case 3:

      printf("donnez le titre du livre pour ajouter une copie  : ");
      scanf("%s",titre);
      ajouter_copie_Livre( Tab, titre);

        break ;

    case 4:

       ajout_nv_livre(Tab);

        break ;

    case 5 :
       printf ("Donnez le titre du livre que vous voulez emprunter :");
       scanf ("%s",&titre);
       printf ("\nDonnez votre Nom Svp : ");
       scanf("%s",&DW);
       ajout_emprenteur( Tab ,DW , titre);
       break ;

    case 6 :
      printf ("Donnez le titre de votre Livre :\n");
      scanf("%s",&titre);
      printf("donnez votre Nom svp : \n");
      scanf("%s",&DW);
      rendre_Livre(Tab,titre,DW);
      break ;

    default:
    	x = Quit(t);
                if( x == 1 )
                    again = 0;
    break;
  }
}
  printf("\n\n");
  printf("\t Programme terminee ! au revoir");

    return 0;
}
