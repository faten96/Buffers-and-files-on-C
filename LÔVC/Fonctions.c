#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"
#define b 100
#define x 99999999999999999999


///---------------------------------------------------------------------------------------------------------///
///                                     MANIPULATION DE CHAINE                                              ///
///---------------------------------------------------------------------------------------------------------///

/** Ajoute un certain nombre donné de zéros au début d'une chaine donnée **/

char* ajout_zeros(char* chaine,  int nb)                    // ajout un certain nombres de 0 au début
{
    int i,j;
    char *temp=malloc(sizeof(char)*nb);
    if (strlen(chaine)<nb)
    {
        for (i=0; i<nb-strlen(chaine); i++)
        {
            temp[i]='0';                                    // on ajout des 0 au début
        }
        for (i=nb-strlen(chaine),j=0; i<nb; i++,j++)
        {
            temp[i]=chaine[j];                              // on ajoute les caractère de la chaine correspendants
        }
        temp[nb]='\0';
        return temp;
    }
    else return chaine;
}

/** Concatenation des champ effac,cle et info et obtenir une chaine de forme ( Taille , effac, cle , info) **/
char* concat(char* effac, int cle, char* info)
{
    char* chaine      = calloc( 1 , sizeof(char)*b);
    char* ChampInfo = calloc( 1 , sizeof(char) * 3);
    char* ChampCle  = calloc( 1 , sizeof(char) * 20);
    itoa(cle, ChampCle, 10);                       // convertir la cle(qui est en décimal)en chaine de caractère
    ChampCle = ajout_zeros(ChampCle, 20);           // on ajout 20 zéros au début du champcle
    itoa(3+strlen(effac)+strlen(ChampCle)+strlen(info), ChampInfo, 10);
    ChampInfo = ajout_zeros(ChampInfo, 3);
    strncat(chaine, ChampInfo, strlen(ChampInfo));
    strncat(chaine, effac, strlen(effac));
    strncat(chaine, ChampCle, strlen(ChampCle));
    strncat(chaine, info, strlen(info));
    return chaine;
}

/** Récupération d'une chaine de caractère de longeur (n) à partir d'un fichier LNOVC **/

char* recup_chaine ( int n , int* i , int* j , Buffer* buf , LNOVC* fichier )
{
    int k = 0;
    char* chaine = calloc( 1 , sizeof(char)*b);
    for ( k = 0 ; k < n ; k++ )
    {
        if ( (*j) < b )                              // s'il n'y a pas de chevauchement
        {
            chaine[k] = buf->tab[(*j)];              // on écrit la caractère dans le buffer
            (*j)++;
        }
        else                                         // s'il y a un chevauchement
        {
            (*i) = buf->suivant;                     // on passe au bloc suivant
            LireDir( fichier , (*i) , buf );         // on lit le bloc suivant
            chaine[k] = buf->tab[0];                // on écrit le premier caractère dans le buffer
            (*j) = 1;                               // on met à jour la position (à 1)
        }
    }
    chaine[k] = '\0';                               //on ajoute le caractère de la fin de chaine
    return chaine;                                  // on retourne la chaine
}


/**    Ecrir une chaine de caractères de longeur n dans le buffer    **/
void ecrire_chaine ( char chaine[] , int n , int* i , int* j , Buffer* buf , LNOVC* fichier )
{
    int k = 0;
    int r = entete(fichier,6);
    for ( k = 0 ; k < n ; k++ )
    {
        if ( (*j) < b )                             // s'il n'y a pas de chevauchement
        {
            buf->tab[*j] = chaine[k];               // on écrit la caractère dans le buffer
            (*j)++;
        }
        else                                        // s'il y a un chevauchement
        {
            EcrireDir( fichier , *i , buf );        // on écrit le buffer actuel dans le fichier
            memset( buf , 0 , sizeof(Buffer) );     // on réinitialise le buffer à zéro
            buf->suivant = -1;                      // on on met le champ suivant à -1
            allocBloc(fichier);                     // on alloue un nouveau bloc
            buf->tab[0] = chaine[k];                // on écrit le caractère dans le buffer
            (*j) = 1;                               // on met à jour la position 'j'
            (*i) = entete(fichier,4);        // on met à jour le numero de bloc 'i' au dernier bloc
            affEntete(fichier,1,entete(fichier,1)+1);

        }
    }
    buf->tab[*j] = '\0';                            // on ajoute le caractère de la fin de chaine
    affEntete(fichier,6,r+n);                       // mise à jour de l'entete ( nb_caract_inserés )

}

///---------------------------------------------------------------------------------------------///
///     Fonctions de Recherche, insertion , supression, réorganisation dans un fichier LNOVC    ///
///---------------------------------------------------------------------------------------------///

/** Recherche d'une clé donné dans un fichier LNOVC retourne trouv , le numéro du bloc
et la position ou se trouv la donnée qu'on cherche **/

void recherche (LNOVC* fichier , int cle , int* trouv , int* i, int* j )
{
    Buffer* buf     = calloc( 1,sizeof(Buffer));
    char* tailleInfo = calloc( 1,sizeof(char)*3 );
    char* champEffac = calloc( 1, sizeof(char)*1 );
    char* champCle   = calloc(  1,sizeof(char)*20 );
    char* champinfo   = calloc( 1, sizeof(char)*(b-24) );
    int stop = 0;
    int bloc , pos;
    if (entete(fichier,1)!=0)                          // si le fichier n'est pas vide
    {
        (*trouv) = 0;
        (*i) = entete(fichier,2);                       // commencer par le premier bloc
        (*j) = 0;                                       // commencer par la première position
         LireDir(fichier , (*i) , buf);                 // lire le premier bloc
        while ( (stop==0) && ((*trouv)==0) )
        {

            bloc = (*i);
            pos  = (*j);
            tailleInfo = recup_chaine( 3 , i , j , buf , fichier);    // récupérer la taille
            champEffac = recup_chaine( 1, i , j , buf , fichier);     // récupérer le booleen
            champCle   = recup_chaine( 20 , i , j , buf , fichier);    // récupérer la clé
            champinfo  = recup_chaine( atoi(tailleInfo)-24 , i , j , buf , fichier ); // récupérer l'info
            if ( cle==atoi(champCle) && strcmp(champEffac,"f")==0)
                // si les cle sont identiques et l'info n'est pas suprimée
            {

                (*trouv) = 1;
            }
            else
            {

                if ( *i == fichier->entete.queue && *j == fichier->entete.indice_Libre )
                {
                    stop = 1;                          // si on est arrivée à la fin du fichier
                }
            }
        }
        (*i) = bloc;
        (*j) = pos;
    }
    else
    {
        printf("Le fichier est vide ");
        (*i)=1;
        (*j)=0;
    }
}

/** Insertion dans un fichier LNOVC **/


void insertion ( LNOVC* fichier , int cle , char* info )
{
    int i , j , trouv = 0 , cpt;
    Buffer* buf = malloc(  sizeof(Buffer));
    char *enreg=malloc((sizeof(char))*b); // la chaine qui va contenir l'enregistrement qui va etre inseré
    char effac[]="f";
    recherche( fichier, cle , &trouv , &i , &j );
    if (trouv == 0)                                // Si la cle n'est pas trouvée dans le fichier
    {
        cpt = entete(fichier,1);
        i = entete(fichier,3);                     // récupérer le numéro du dérnier bloc
        j = entete(fichier,4);                     // récupérer le numéro de la dérnière position
        LireDir(fichier , i ,buf);                 // lire le dernier bloc
        enreg=concat(effac,cle,info);                 // creation de l'enregistrement
        ecrire_chaine( enreg , strlen(enreg) ,&i,&j, buf , fichier );  // écrire la chaine dans le buffer
        EcrireDir(fichier , i , buf);              // écrire le buffer sur le disque
        affEntete(fichier,3,i);
        affEntete(fichier,4,j);
        affEntete(fichier,1,i);
        rewind(fichier->fich);
        fwrite(&(fichier->entete) , sizeof(Entete) , 1 , fichier->fich);
         // On écrit l'entete sur le disque
         printf(" Insertion reussite.\n\n");
    }
    else printf("\tInsertion impossible, cle deja existante .\n\n");
}

/** Création d'un fichier LNOVC contenat un certain nombre d'enregistrements **/
void creer_fichier(char nomfich[] , int nb_enreg)
{
    LNOVC* fichier = calloc( 1 , sizeof(LNOVC));
    char* temp  = calloc( 1 , sizeof(char)*b );           // la chaine qui contient l'enregistrement
    Buffer* buf  = calloc( 1 , sizeof(Buffer) );
    int der_pos = 0;
    char info[b - 24];                                     // La chaine qui va contenir l'info
    char efface[] = "f";                                  // On initialise le champ booléen à faux
    int nb = 1;
    int n ,cle,j,cpt=0;                                         // On initialise nombre de blocs à 1


    fichier = Ouvrir(nomfich , 'n');
    affEntete(fichier,2,1);                              // la tête reçoit 1
    affEntete(fichier,3,1);                              // la queue reçoit 1
    for (n = 0 ; n < nb_enreg ; ++n)
    {
        do
        {
            printf("\n\tDonnez la cl%c : ",130);
            scanf("%d",&cle);
        }
        while ( cle > x );
        printf("\tDonnez l'info : ");
        scanf(" %56[^\n]" , info);
       temp =concat(efface, cle, info);   // création de la chaine qui va contenir l'enregistrement
                                          // à partir de la clé , l'info et le champ effacé
        for ( j = 0 ; j < strlen(temp) ; j++)
        {
            if (der_pos < b)                             // Sans chevauchement
            {
                buf->tab[der_pos] = temp[j];
                der_pos++;
            //    affEntete(fichier,6,entete(fichier,6)+1);
                cpt=cpt+1;
                if (der_pos == b) buf->tab[der_pos] = '\0';
            }
            else                                         // Avec chevauchement
            {
                buf->suivant = nb + 1;                  // mettre à jour le pointeur vers le bloc suivant
                EcrireDir(fichier , nb , buf);           // on écrit le bloc actuel
                nb = nb + 1;                             // on incrémente le nombre de bloc
                affEntete(fichier,3,nb);
                memset(buf , 0 , sizeof(Buffer));        // Remise à zero du buffer
                buf->tab[0] = temp[j];                   // on commence à remplir le buffer
                der_pos = 1;
                affEntete(fichier,6,entete(fichier,6)+1);
                cpt=cpt+1;

            }
        }
    }

    // mise à jour de l'entete
    affEntete(fichier,4,der_pos);
    affEntete(fichier,3,nb);
    affEntete(fichier,1,nb);
    affEntete(fichier,6,cpt);
    affEntete(fichier,5,0);
    if (der_pos < b)
    {
        buf->tab[der_pos] = '\0';
        buf->suivant = -1;                        // mettre à jour le pointeur vers le bloc suivant
        EcrireDir(fichier , nb , buf);            // écrire le dernier bloc (bloc actuel)
    }
    free(buf);
    free(temp);
    fermer(fichier);
}
/**  Suppression d'une cle dans un fichier LNOVC **/

void Suppression(LNOVC *fichier)
{
    int trouv,i,j,cle;
    Buffer buf;
    char *taille_ch,*champs_efface;

    printf("Faites entrer la cle que vous supprimer: ");
    scanf("%d",&cle);

    recherche(fichier,cle,&trouv,&i,&j);
    if(trouv==0)
    {
        printf("La cle n'existe pas dans le fichier \n\n");
    }
    else
    {
        LireDir(fichier,i,&buf);
        taille_ch=recup_chaine(3,&i,&j,&buf,fichier);
        //printf("la taille de la chaine est : %d",atoi(taille_ch));
        buf.tab[j]="1";
        EcrireDir(fichier,i,&buf);
        affEntete(fichier,5,entete(fichier,5)+atoi(taille_ch));
        printf("La cle a ete supprimee avec succes \n\n");


    }

}

/** Réorganisation du fichier après suppression **/
void reorganisation ( LNOVC* fich , char* nouveau )
{
    LNOVC* tmp = Ouvrir( nouveau , 'n' );
    char* ch_taille = calloc( 1 , sizeof(char)*3 );
    char* ch_efface = calloc( 1 , sizeof(char)*1 );
    char* ch_cle    = calloc( 1 , sizeof(char)*20 );
    char* ch_info   = calloc( 1 , sizeof(char)*(b-24) );
    char* enreg     = calloc( 1 , sizeof(char)*b );

    int bloc1,pos1,bloc2,pos2,cpt;

    Buffer* buf1 = calloc( 1 , sizeof(Buffer) );
    Buffer* buf2 = calloc( 1 , sizeof(Buffer) );

    bloc1 =entete(fich,2); /// avoir le premier bloc du fichier original
    pos1 = 0;
    bloc2 = 1;
    pos2 = 0;

    affEntete(tmp,2,1);                   /// mettre à jour l'entete du nouveau fichier réoragnisé
    affEntete(tmp,3,1);                    /// mettre à jour l'entete du nouveau fichier réoragnisé
    affEntete(tmp,4,0);                   /// mettre à jour l'entete du nouveau fichier réoragnisé
    affEntete(tmp,6,0);               /// mettre à jour l'entete du nouveau fichier réoragnisé
    affEntete(tmp,5,0);                /// mettre à jour l'entete du nouveau fichier réoragnisé
    affEntete(tmp,1,0);                /// mettre à jour l'entete du nouveau fichier réoragnisé

    if ( entete(fich,1 )!=0) /// si le fichier n'est pas vide
    {
        /// tant que on est pas encore arrivé à la fin du fichier original
        while ( bloc1!=entete(fich,3) || pos1!=entete(fich,4) )
        {
            memset( enreg , 0 , sizeof(enreg) ); /// c'est pour remettre la chaine à zéro
            LireDir( fich , bloc1 , buf1 );      /// relir  le premier bloc
            sprintf( ch_taille , recup_chaine( 3 , &bloc1 , &pos1 , buf1 , fich ) );
            sprintf( ch_efface , recup_chaine( 1 , &bloc1 , &pos1 , buf1 , fich ) );
            sprintf( ch_cle    , recup_chaine( 20 , &bloc1 , &pos1 , buf1 , fich ) );
            sprintf( ch_info   , recup_chaine( atoi(ch_taille)-24 , &bloc1 , &pos1 , buf1 , fich ) );
            sprintf( enreg , "%s%s%s%s" , ch_taille , ch_efface , ch_cle , ch_info );
            if ( strcmp(ch_efface , "f")==0 ) /// si l'enregistrement n'est pas supprimé logiquement
            {
                ecrire_chaine( enreg , strlen(enreg) , &bloc2 , &pos2 , buf2 , tmp );
                affEntete(tmp,4,pos2);
                printf("un enregistrement a ete ecrit dans le nouveau fichier \n");
            }
        }
        EcrireDir( tmp , bloc2 , buf2 ); /// on écrit le dernier bloc dans le nouveau fichier réorganisé
    }
    affEntete(tmp,1,bloc2) ;
    affEntete(tmp,2,1);
    affEntete(tmp,3,bloc2);
    affEntete(tmp,6,(bloc2-1)*100+pos2);
    fermer(tmp);
}



///---------------------------------------------------------------------------------------------///
///                                  FONCTIONS D'AFFICHAGE                                      ///
///---------------------------------------------------------------------------------------------///


/**Donne la forme générale du programme**/
void acc()
{

    system("color f0");     //couleur de fond et de texte
    system("cls");
    gotoxy(1,1);
    printf("_____________________________________________________________________________________");
    gotoxy(1,2);
    signe_esi();
    gotoxy(1,11);
    printf("_____________________________________________________________________________________");
    gotoxy(1,18);

}

void acc3()
{
    system("color f0");     //couleur de fond et de texte
    system("cls");
}


void signe_esi()
{
    printf("\n %c%c%c%c %c%c%c%c %c\n",219,219,219,219,219,219,219,219,177);
    printf(" %c    %c        ECOLE NATIONALE               TP2 SFSD : STRUCTURE DE FICHIER LNOVC  \n");
    printf(" %c    %c    %c                                      \n",219,219,219,219);
    printf(" %c%c%c%c %c%c%c%c %c  SUPERIEURE                     MAALEM FATEN | SADOUD RANIA | Groupe04 \n",219,219,219,219,219,219,219,219,219);
    printf(" %c       %c %c      \n",219,219,219);
    printf(" %c  %c %c  %c %c  D'INFORMATIQUE                             2016/2017  \n",219,219,219,219,219);
    printf(" %c%c%c%c %c%c%c%c %c\n",219,219,219,219,219,219,219,219,219);

}
void Afficher(index tab[],int taille)
{
    int i;
    for(i=0;i<taille;i++) /*parcourir la table d'index*/
    {
        printf("la cle:%s     | le bloc:%d       |la position:%d\n",tab[i].cle,tab[i].bloc,tab[i].pos); /*afficher à l'ecran*/
    }
}

void Sauvegarder(index tab[],int taille,FILE *fichier_index,LNOVC *fichier)
{
    int i=0,j=0;
    char ch_bloc[20],ch_pos[3];

    rewind(fichier_index);
    if(fichier_index!=NULL)
    {
       fputc(entete(fichier,1),fichier_index);  /*sauvegarder l'entete du fichier dans le fichier index*/
       fputc(entete(fichier,2),fichier_index);
       fputc(entete(fichier,3),fichier_index);
       fputc(entete(fichier,4),fichier_index);
       fputc(entete(fichier,5),fichier_index);
       fputc(entete(fichier,6),fichier_index);
       fprintf(fichier_index,"\n");

       for(i=0;i<taille;i++)
       {
          fwrite(tab[i].cle,sizeof(char)*20,1,fichier_index); /* sauvegarder la cle dans le fichier index*/
          turn_to_string(ch_bloc,tab[i].bloc,1); /* convertir le bloc en chaine de char pour utiliser fputs*/
          fputs(ch_bloc,fichier_index);  /* sauvegarder le bloc dans le fichier index*/
          //La position dans le fichier d'index est sur 3 positions*/
          turn_to_string(ch_pos,tab[i].pos,3); /* convertir la position en chaine de char pour utiliser fputs*/
          fputs(ch_pos,fichier_index);  /* sauvegarder la position dans le fichier index*/
          fprintf(fichier_index,"\n"); /*passer à une nouvelle ligne pour ecrire l'index d'un nouveau enregistrement*/
       }
    }

}

void turn_to_string(char chaine[], int n, int longueur)
{

    int k;
    for(k=longueur-1;k>=0;k--)          // on commence par le poids faible du nombre qui va etre mi a chaine[longueur-1]
    {
        chaine[k]=(n%10)+'0';           // extraction chiffre par chiffre  grace au mod par 10 et ajout du code ascii du zero afoin d'obtenir le code ascii correspondant au chiffre obtenu
        n=n/10;                        // on passe au chiffre suivant
    }
    chaine[longueur]='\0';             // fin de la chaine construite
}
/** Réorganisation du fichier après suppression **/
void reorganisation_avec_index ( LNOVC* fich ,index tab[],int *taille_Index )
{
    char* ch_taille = calloc( 1 , sizeof(char)*3 );
    char* ch_efface = calloc( 1 , sizeof(char)*1 );
    char* ch_cle    = calloc( 1 , sizeof(char)*20 );
    char* ch_info   = calloc( 1 , sizeof(char)*(b-24) );
    char* enreg     = calloc( 1 , sizeof(char)*b );

    int bloc1,pos1,bloc2,pos2,cpt,i_Inx=0;

    Buffer* buf1 = calloc( 1 , sizeof(Buffer) );

    bloc1 =entete(fich,2); /// avoir le premier bloc du fichier original
    pos1 = 0;
    int bloc_inx,pos_inx;

    if ( entete(fich,1 )!=0) /// si le fichier n'est pas vide
    {
        /// tant que on est pas encore arrivé à la fin du fichier original
        while ( bloc1!=entete(fich,3) || pos1!=entete(fich,4) )
        {
            bloc_inx=bloc1;  ///pour sauvegarder le bloc ou se trouve l'enregisrement
            pos_inx=pos1;    ///pour sauvegarder la position ou se trouve l'enregisrement

            memset( enreg , 0 , sizeof(enreg) ); /// c'est pour remettre la chaine à zéro
            LireDir( fich , bloc1 , buf1 );      /// relir  le premier bloc
            sprintf( ch_taille , recup_chaine( 3 , &bloc1 , &pos1 , buf1 , fich ) );
            sprintf( ch_efface , recup_chaine( 1 , &bloc1 , &pos1 , buf1 , fich ) );
            sprintf( ch_cle    , recup_chaine( 20 , &bloc1 , &pos1 , buf1 , fich ) );
            sprintf( ch_info   , recup_chaine( atoi(ch_taille)-24 , &bloc1 , &pos1 , buf1 , fich ) );
            sprintf( enreg , "%s%s%s%s" , ch_taille , ch_efface , ch_cle , ch_info );
            if ( strcmp(ch_efface , "f")==0 ) /// si l'enregistrement n'est pas supprimé logiquement
            {
                strcpy(tab[i_Inx].cle,ch_cle);
                tab[i_Inx].bloc=bloc_inx;
                tab[i_Inx].pos=pos_inx;
               // printf("la cle:%s     | le bloc:%d       |la position:%d\n",tab[i_Inx].cle,tab[i_Inx].bloc,tab[i_Inx].pos);
                printf("un enregistrement a ete ecrit dans le nouveau fichier \n");
                i_Inx++;

            }
        }
    }
    *taille_Index=i_Inx;
}
void Afficher_Entete (LNOVC* fich)
{
    printf("\n");
    printf("\tNombre de blocs dans le fichier = %d \n", fich->entete.queue - fich->entete.tete + 1 );
    printf("\tPosition libre du dernier bloc  = %d \n", fich->entete.indice_Libre );
    printf("\tNombre de caracteres inseres    = %f \n", fich->entete.nb_caract_insr);
    printf("\tNombre de caracteres supprimes  = %f \n", fich->entete.nbrSupp );
    printf("\tTaux de caracteres supprimes    %f",(fich->entete.nbrSupp/fich->entete.nb_caract_insr));
    printf("\n");
}
void Afficher_Bloc ( LNOVC* fich , int bloc )
{
    int i;
    if ( bloc >= 1 && bloc <= fich->entete.queue ) /// si le numero du bloc est valide
    {
        Buffer* buf = calloc( 1 , sizeof(Buffer));
        LireDir(fich , bloc , buf);
        printf("\n\tLe contenu du bloc %d :|" , bloc);
        for (i = 0 ; i < b ; i++) printf("%c" , buf->tab[i]);
        printf("|Suivant:%d\n" , buf->suivant);
    }
}
void Afficher_tout_le_fichier (LNOVC* fich)
{
    if ( fich->entete.tete==-1 || fich->entete.queue==-1 || fich==NULL) /// si le fichier est vide
        printf("\n\tErreur !!! Fichier inexistant ou vide");
    else
    {
        int i;
        for (i = fich->entete.tete ; i <= fich->entete.queue ; i++)
        {
            Afficher_Bloc(fich , i);
        }
    }
}


