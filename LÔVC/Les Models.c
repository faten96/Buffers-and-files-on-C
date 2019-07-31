#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"
#define b 100

/**         mettre à jour un element de l'entete            **/

void affEntete(LNOVC *fichier,int i,int valeur)
{
    switch(i)
    {
        case 1 :
        {
            fichier->entete.nbrBloc=valeur;
        }break;
        case 2 :
        {
            fichier->entete.tete=valeur;
        }break;
        case 3 :
        {
            fichier->entete.queue=valeur;
        }break;
        case 4 :
        {
            fichier->entete.indice_Libre=valeur;
        }break;
        case 5 :
        {
            fichier->entete.nbrSupp=valeur;
        }break;
        case 6 :
        {
            fichier->entete.nb_caract_insr=valeur;
        }
    };
}

/**         retourne la valeur d'un element de l'entete         **/

int entete(LNOVC *fichier,int i)
{
    switch(i)
    {
       case 1 :
           {
               return (fichier->entete.nbrBloc);
           }break;
       case 2 :
           {
               return (fichier->entete.tete);
           }break;
       case 3 :
           {
               return (fichier->entete.queue);
           }break;
       case 4 :
           {
               return (fichier->entete.indice_Libre);
           }break;
       case 5 :
           {
               return (fichier->entete.nbrSupp);
           }break;
       case 6 :
           {
               return (fichier->entete.nb_caract_insr);
           }break;

    }
}

/**     lire dans 'buf' le contenu du bloc numéro 'i' du fichier 'f'  **/

void LireDir(LNOVC *fichier,int i, Buffer *Buf)
{
   fseek(fichier->fich,(sizeof(Entete)+sizeof(Tbloc)*(i-1)),SEEK_SET);
   fread(Buf,sizeof(Buffer),1,fichier->fich);
   rewind(fichier->fich);
}

/**    écrit le contenu de la variable 'buf' dans le bloc numéro 'i' du fichier   **/

void EcrireDir(LNOVC *fichier,int i, Buffer *Buf)
{
    fseek(fichier->fich,(sizeof(Entete)+sizeof(Tbloc)*(i-1)),SEEK_SET);
    fwrite(Buf,sizeof(Buffer),1,fichier->fich);
}


/**    Permet d'ouvrir le ficheir    **/

LNOVC *Ouvrir(char nom_physique[],char mode)
{
    LNOVC *fichier=malloc(sizeof(LNOVC));  //allocation de la memoire
    Buffer buf;

    if((mode=='A') || (mode=='a'))
    {
        fichier->fich=fopen(nom_physique,"rb+");  //mode ancien
        fread(&(fichier->entete),sizeof(Entete),1,fichier->fich); //chargement de l'entete enregistrée en debut de fihcier
    }
    else
    {
        if((mode=='N')  ||  (mode=='n'))   //mode nouveau
        {
            fichier->fich=fopen(nom_physique,"wb+");  //ouverture du fichier en mode binaaire ecriture
            affEntete(fichier,1,0);                    //mise a zero du nombre de bloc si entete(fihcier,1)=0 alors le fichier est vide
            affEntete(fichier,2,1);                   //mettre tete au premier bloc
            affEntete(fichier,3,1);
            affEntete(fichier,4,0);
            affEntete(fichier,5,0);
            affEntete(fichier,6,0);
            fwrite(&(fichier->entete),sizeof(Entete),1,fichier->fich);
            buf.suivant=-1;
            sprintf(buf.tab,"%s","");
            EcrireDir(fichier,1,&buf);

        }
        else
        {
            printf("format d'ouverture impossible");
        }
    }
    return(fichier);
}

/**             Fermer le fichier           **/

void fermer(LNOVC *fichier)
{
    rewind(fichier->fich);
    fwrite(&(fichier->entete),sizeof(Entete),1,fichier->fich);
    rewind(fichier->fich);
    fclose(fichier->fich);

}
/**             Allouer un nouveau bloc         **/

void allocBloc(LNOVC*fichier)
{
    Buffer *buf=malloc(sizeof(Buffer));
    LireDir(fichier,entete(fichier,3),buf);
    buf->suivant=entete(fichier,1)+1;
    EcrireDir(fichier,entete(fichier,3),buf);
    affEntete(fichier,3,entete(fichier,1)+1);
    buf->suivant=-1;
    sprintf(buf->tab,"%s","");
    EcrireDir(fichier,entete(fichier,3),buf);
    affEntete(fichier,1,entete(fichier,1)+1);
}
