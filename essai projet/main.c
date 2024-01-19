//
//  main.c
//  essai projet
//
//  Created by kerrouchi maroua on 25/12/2023.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>







#define taille_max 10



typedef struct etudiant {
    char nom[15];
    char prenom[15];
    int matricule ;
}etudiant;
typedef struct bloc {
    etudiant tab[taille_max];
    int nb ;
    int suivant;
}bloc;
typedef struct bloc buffer ;
typedef struct entete{
    int nb_enreg; //nombre totale d'enregistrement
    int nb_bloc; // nombre totale de bloc
    int blocD; //l'adresse de premier bloc
    int blocF; // l'adresse de dernier bloc
}entete;
typedef struct LNOF{
    FILE * fichier;
    entete tete;
}LNOF;

    buffer buff;
    LNOF *f1;


//procedure pour lire un buffer dans fichier
void LireDir(LNOF *F, int i , buffer *buff) {
   fseek(F->fichier , sizeof(entete) + (i - 1) * sizeof(buffer), SEEK_SET);
   fread(buff, sizeof(buffer), 1, F->fichier);
rewind(F->fichier);
}
// procedure pour ecrire un buffer dans fichier
void EcrireDir(LNOF *F, int i, buffer buff) {
   fseek(F->fichier, sizeof(entete) + (i - 1) * sizeof(buffer), SEEK_SET);
   fwrite(&buff, sizeof(buffer), 1, F->fichier);
}
// fonction retourne un champ de l'entete
int entete1(LNOF *F, int i) {
       switch (i) {
              case 1: {
                     return(F->tete.nb_bloc);
                     break;
                  }
              case 2: {
                     return(F->tete.blocD);
                     break;
                  }
              case 3: {
                     return (F->tete.blocF);
                     break;
                  }
              case 4: {
                     return (F->tete.nb_enreg);
                     break;
                  }
              }
           }



 // procedure pour  modifier l'entete
void modifier_entete(LNOF *F,int i , int val) {
   switch (i) {
      case 1: {
         F->tete.nb_bloc = val;
         break;
      }
      case 2: {
         F->tete.blocD = val;
         break;
      }
      case 3: {
         F->tete.blocF= val;
         break;
      }
      case 4:{
         F->tete.nb_enreg= val;
      }

   }
}

// fonction pour ouvrir le fichier
LNOF *Ouvrir(char *nom_fichier, char mode) {
   LNOF *F = (LNOF*) malloc(sizeof(LNOF));
memset(F, 0, sizeof(LNOF));
   if ((mode == 'a') || (mode == 'A')) {
      F->fichier = fopen(nom_fichier, "rb+");
      if (F->fichier!= NULL) {
         rewind(F->fichier);
         fread(&(F->tete), sizeof(entete), 1, F->fichier);
      } else {
         perror("Fichier mal ouvert \n");
         exit(1);
      }
   } else if ((mode == 'n') || (mode == 'N')) {
      F->fichier = fopen(nom_fichier, "wb+");
      if (F->fichier != NULL) {
         modifier_entete(F, 1, 0);
         modifier_entete(F, 2, 1);
         modifier_entete(F, 3, 1);
         modifier_entete(F, 4, 0);
         modifier_entete(F, 5, 0);
         rewind(F->fichier);
         fwrite(&(F->tete), sizeof(entete), 1, F->fichier);
      } else {
         perror("Fichier mal cree \n");
         exit(1);
      }
   } else {
      perror("Format d'ouverture non disponible\n");
      exit(1);
   }
   return (F);
}

// fonction pour fermer un fichier LNOF
void Fermer(LNOF *F) {
   rewind(F->fichier);                                      // repositionnement en debut du fichier
   fwrite(&(F->tete), sizeof(entete), 1, F->fichier);       // enregistrement de l'entete
   rewind(F->fichier);                                      // repositionnement den debut du fichier
   fclose(F->fichier);                                      // fermeture du fichier
}
// procedure pour allouer un bloc dans un fichier LObarreF
void Alloc_Bloc(LNOF *F) {
   buffer *buff = malloc(sizeof(buffer));  // allocation d'un buffer
   LireDir(F, entete1(F, 3), buff);          // lecture du bloc fin
   buff->suivant = entete1(F, 1) + 1;          // mise a jour du bloc queue au nouveau bloc queue
   EcrireDir(F, entete1(F, 3), *buff);      // ecriture du bloc dans le fichier
   modifier_entete(F, 3, entete1(F, 1) + 1);    // mise a jour de la queue dans l'entete
   buff->suivant = -1;                        // mise a NIL du suivant de la queue
   buff->nb= 0;                           // initialisation du nouveau bloc queue
   EcrireDir(F, entete1(F, 3), *buff);      // ecriture du nouveau bloc queue
   modifier_entete(F, 1, entete1(F, 1) + 1);    // mise a jour du nombre de bloc dans l'entete
}
void recherche (LNOF *F,char nom_fich[30],int matricule,int *num_bloc,int *pos,int *trouv)//module de recherche de la cle ds un fichier
{
    bloc buff;//declarer un buffer
    int i,j,n;//donne le nummero de bloc courant

    ouvrir(F,nom_fich,'a');//ouvrir le fichier en mode ancien
    if (F->fichier!=NULL)//tester si le fichier existe
    {
        i=en_tete(F,1);//recuperer le numero du 1 er bloc
        n=en_tete(F,2);//recuperer le nombre d'enregistrements ds le fichier
        if((i==-1)||(n==0))
            {
                printf("\n \t\t<<Le fichier est vide>>");//tester si le fichier n'est pas vide
                trouv=-1;
            } //retourner trouv à faux
        else
        {
            if((i!=-1)&&(n!=0))//sinon
            {
                *trouv=0;*pos=0;j=0;//initialiser les paramettres
                while((i!=-1)&&(*trouv==0))//tq on est pas arrivé à la fin de fichier et on a pas trouvé la clé
                {
                    LireDir(F,i,&buff);//lire le bloc i
                    if(buff.nb !=0)//tester s'il n'est pas vide
                    {
                        while((j<=taille_max)&&(*trouv==0))//tq c'est pas la fin de tab d'enregistrements et non trouv
                        {
                            if((buff.tab[j].matricule==matricule))//verifier chaque case si elle n'est pas effacée
                            {//si vérifié
                                *trouv=1;//mettre trouv à vrai
                                *num_bloc=i;//retourner le numero de bloc
                                *pos=j;
                            }
                            j++;//aller à la prochaine case

                        }
                        if((j>taille_max)&&(*trouv==0))//si on est arrivé à la fin du tableau sans trouver la clé
                        {
                            j=0;//initialiser la position à 0
                        }
                    }
                    i=buff.suivant;//aller au bloc suivant
                }
            }
        }
    }
fermer(F);//fermer le fichier
}


void rech_pos_lib(LNOF *F,char nom_fich[30],int *pos_lib,int *num_bloc,int *trouv)//rechercher la première postion libre
{
    int i,j;
    bloc buff;

    *trouv=0;//initialiser le trouv à faux
    ouvrir(F,nom_fich,'a');//ouvrir le fichier en mode ancien
    if (F->fichier!=NULL)//si le fichier existe
        {
            i=entete1(F,1);//lire la premiere caract
            if(i!=-1)
            {
                j=0;//initialiser j à la premiere case de tableau
                while((i!=-1)&&(*trouv==0))
                {
                LireDir(F,i,&buff);//lire le i ème bloc
                if(buff.nb!=taille_max)//tester s'il n'est pas completement rempli
                {while((j<=buff.nb)&&(*trouv==0))//tq on est pas arrivé à la fin du tableau et on a pas trouvé une case vide
                {

                    {
                        *trouv=1;// trouv=vrai
                        *num_bloc=i;//rerourner le ième bloc et la j ème colonne
                        *pos_lib=j;
                    }

                        j++;//aller à la prochaine case

                }
                }
                i=buff.suivant;//aller au prochain bloc
            }
        }
        }
fermer(F);
}





void reglage()//pour le controle de saisie
{
    int caract = 0;
    while ((caract!='\n') && (caract!= EOF))
    {
        caract=getchar();
    }
}
void insertion(LNOF *F,char nom_fich[30],etudiant *n)//inserer un enregistrement ds un fichier
{
    int trouv,trouve,i,j,pos_lib,num_bloc,affirm,k,reg=1;
    bloc buff;
    ouvrir(F,nom_fich,'A');//ouvrir le fichier en mode ancien

    if (F->fichier!=NULL)//si le fichier existe
    {
        recherche(F,nom_fich,n->matricule,&i,&j,&trouve);//on cherche la clé si elle existe déjà
        while(trouve==1)//tant qu'elle existe on boucle
        {
            printf("\n\t\t<<<Le matricule existe deja>>>");
            printf("\ndonner le matricule");
            reg=scanf("%d",&n->matricule);
            while((reg!=1)||(n->matricule<0))
            {
                reglage();
                printf("\nDonner le matricule de nouveau elle est erronee");
                reg=scanf("%d",&n->matricule);
            }
            recherche(F,nom_fich,n->matricule,&i,&j,&trouve);
        }

            rech_pos_lib(F,nom_fich,&pos_lib,&num_bloc,&trouv);
    }//rechercher la premiere position libre ds le fichier

    else//si le fichier n'existe pas
    {
        printf("...voulez vous l'inserer");//on demande une affirmation
        printf("\ntapez 1 si OUI,0 si NON");
        scanf("%d",&affirm);
        if (affirm==0) goto end;
        if (affirm==1)//si c'est confirmé
        {
            ouvrir(F,nom_fich,'n');//on ouvre le fichier en mode nouveau
            fermer(F);
        }
    }
    ouvrir(F,nom_fich,'a');//on le réouvre en mode ancien
    if ((trouv==0)||(affirm==1))//si aprés la recherche d'une position libre on l'a pas trouvé
    {
        num_bloc=entete1(F,3);//recuperer le num de dernier bloc
        if( num_bloc==-1)
        {
            modifier_entete(F,1,0);//si fichier vide,mette le numero du premier bloc à 0
        }
        else
        {
            LireDir(F,num_bloc,&buff);//sinon recuperer le contenu de dernier buffer
            buff.suivant=num_bloc+1;//faire le chainage avec le nouveau bloc
            EcrireDir(F,num_bloc,buff);
        }//sauvegarder les modifications
        num_bloc=num_bloc+1;//incrementer le numero de dernier bloc
        modifier_entete(F,3,num_bloc);//sauvegarder ds l'entete
        alloc_bloc(F,nom_fich,&buff);//initialiser le nouveau bloc
        //n->efface=0;
        buff.tab[0]=*n;//mettre les infos dedans
        buff.nb=1;//initialiser le nombre d'enregistrements ds le bloc
    }
    if(trouv==1)//s'il y'a une case vide
    {
        LireDir(F,num_bloc,&buff);//lire le contenu du bloc correspendant
        //n->efface=0;
        buff.tab[pos_lib]=*n;//mettre les infos à l'endroit qui convient
        buff.nb= buff.nb+1;//incrementer le nombre d'articles
    }
    i=entete1(F,2);//lire le nombre d'articles ds le fichier
    i++;
    modifier_entete(F,2,i);//l'incrementer
    ecrire_dire(F,num_bloc,&buff);//ecrire le buffer
    end:fermer(F);//fermer le fichier
}



void Suppression1(LNOF *F, int i, int j) {
buffer buf1, buf2;
LireDir(F, i, &buff);
if (i == entete1(F, 3)) {
LireDir(F, i, &buff);
buff.tab[j] = buff.tab[buff.nb - 1]; // Ecrasement de l'enregistrement par le dernier enregistrement
buff.nb--;   // Decrementation du nombre d'enregistrements dans le bloc
if (buff.nb > 0) {
EcrireDir(F, i, buff);
} else {
if (entete1(F, 3) == 1) {
EcrireDir(F, i, buff);
} else {
modifier_entete(F, 1, entete1(F, 1) - 1); // Mise a jour du nombre de bloc utilises
modifier_entete(F, 3, entete1(F, 3) - 1); // Mise a jour de la queue
}
}
}
else{
// le bloc concerne n'est pas le bloc queue
LireDir(F, i, &buf1);
LireDir(F, entete1(F, 3), &buf2);
buf1.tab[j] = buf2.tab[buf2.nb - 1];
buf2.nb--;
EcrireDir(F, i, buf1);
if (buf2.nb > 0) {
// cas 1: dernier bloc contient plusieurs personnel ( > 1)
EcrireDir(F, entete1(F, 3), buf2);
} else {
// cas 2: dernier bloc contient un seul personnel
modifier_entete(F, 1, entete1(F, 1) - 1); // Mise a jour du nombre de bloc utilises
modifier_entete(F, 3, entete1(F, 3) - 1); // Mise a jour de la queue
}
}
modifier_entete(F, 5, entete1(F, 5) + 1); // Mise a jour du compteur de suppresssion
}


void affichage (LNOF *F,char nom_fichier[30])
{int i,j,n;
bloc buff;
    ouvrir(F,nom_fichier,'A');//ouvrir le fichier en mode ancien
    if(F->fichier!=NULL)//si le fichier existe
    {i=entete1(F,1);//recupérer le numéro du premier bloc
     n=entete1(F,2);//recupérer le nombre d'enregistrements

    if (n!=0)//si le fichier n'est pas vide
    {
printf("\t  L'Affichage  \n");

printf("\t\t+----------+----------+\n");
printf("\t\t|  Le matricule | le nom |le prenom\n");
printf("\t\t+----------+----------+\n");
        while(i!=-1)//tant qu'on est pas arrivé à la fin de fichier
    {
        LireDir(F,i,&buff);//lire le buf i
        if (buff.nb!=0)
        {
            for (j=0;j<buff.nb;j++)//parcourir tout le tableau
            {

                printf("\t\t| %-8s | %-8s |%d\n",buff.tab[j].nom,buff.tab[j].prenom,buff.tab[j].matricule);
                printf("\t\t+----------+----------+\n");

            }
        }
        i=buff.suivant;//aller au prochain bloc
    }
    }
    else printf("\n \t\t<<Le fichier est vide>>");}
    fermer(F);



int main()
{
    LNOF *F;
    int nb_enreg;
    int trouv,pos,num_bloc;

    char nom_fichier[30];
    etudiant n ;
    while(1){
        printf("les options sont :\n");
        printf("\t1) pour la creation d'un fichier \n");
        printf("\t2) pour la insertinon d'une element d'un fichier\n ");
        printf("\t3) pour la recherche d'un fichier \n");
        printf("\t4) pour la supression d'un fichier \n");
        printf("\t5) pour l'affichage d'un fichier \n");
        printf("\t6) pour quitter \n");
        printf("\n\n");
        printf("veuillez choisir une option\n ");
        int option;
scanf("%d", &option);
switch(option) {
case 1: {
   printf("\n\t\t<<CREERTION D'UN NOUVEAU FICHIER>>");
                printf("\nDonnez le nom de fichier a creer:");
                scanf("%s",&nom_fichier);
                ouvrir(F,nom_fichier,'N');
                fermer(F);
}
break;
            case 2 : {
                printf("\n\t\t<<INSERTION D'UN NOUVEAU ELEMENT>>");
                printf("\nDonnez le nom de fichier vous allez inserer:");
                scanf("%s",&nom_fichier);
                printf("\n combien d'enregistrements voulez vous inserer?");
                printf("\n le nombre= ");
                scanf("%d",&nb_enreg);
                for(int i=0 ; i<nb_enreg ; i++){
                    printf("\nDonnez l'enregistrement");
                    printf("\n le matricule est ");
                    scanf("%d",&n.matricule);
                    printf("\nle nom est ");
                    scanf("%s",&n.nom);
                    printf("\nle prenom est ");
                    scanf("%s",&n.prenom);
                    insertion(F,nom_fichier,&n);
                }
            }
            break ;
            case 3: {
                printf("\n\t\t<<RECHERCHE D'UN ENREGISTREMENT>>");
                printf("\nDonnez le nom de fichier ou vous allez chercher la cle:");
                scanf("%s",&nom_fichier);
                printf("\n Donnez le matricule de l'enregistrement a rechercher");
                printf("\n Le matricule=");
                recherche(F,nom_fichier,n.matricule,&num_bloc,&pos,&trouv);
                if (trouv==0){
                    printf("\nCette enregistrement n'existe pas");
                }
                else
                    printf("\n L'enregistrement existe dans le bloc %d a la position %d",num_bloc,pos);

            }
            break;

            case 4: {
                printf("\n\t\t<<SUPPRESSION LOGIQUE>>");
                printf("\nDonnez le nom de fichier d'ou vous supprimerez:");
                scanf("%s",&nom_fichier);
                printf("\n Donnez le matricule de l'enregistrement a supprimer");
                printf("\n Le matricule=");
                scanf("%d",&n.matricule);
                supprimer(F,nom_fichier,n.matricule);
            }
            break;

            case 5:
            {
                printf("\n\t\t<<AFFICHAGE>>");
                printf("\nDonnez le nom de fichier a afficher:");
                scanf("%s",&nom_fichier);
                affichage(F,nom_fichier);
            }
            break;
            case 6 : return 0; break;

        }
    }

    return 0;
}
