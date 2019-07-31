#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"
#include"conio.h"
#include"conio2.h"
#include"conio.c"

#define b 100
#define x 99999999999999999999



int main()
{

     LNOVC* fich = calloc( 1 , sizeof(LNOVC) );
     LNOVC* fich1 = calloc( 1 , sizeof(LNOVC) );
     LNOVC *fich_index;

    int c, nb_enreg=0 , cle=0 , i , j , trouv=0 , numbloc;
    char* filename = calloc( 1 , sizeof(char)*100 );
    char* filename1= calloc( 1 , sizeof(char)*100 );
    char* info     = calloc( 1 , sizeof(char)*(b-24) );

    int choix,choose,choose1;
    index T[100];
    int taille;
    FILE *fichier=NULL;
    int d;





    system("mode con LINES=50 COLS=100");            //On donne une taille au programme

menup :
    clrscr();
    acc();
    printf("\n \t\t            MENU PRINCIPAL  \n");
    printf("\n \t\t          1) Creation d'un fichier. \n") ;
    printf("\n \t\t          2) Recherche d'une cle dans un fichier.  \n") ;
    printf("\n \t\t          3) Insertion dans un fichier. \n") ;
    printf("\n \t\t          4) Suppression dans un fichier. \n") ;
    printf("\n \t\t          5) Reorganisation d'un fichier. \n") ;
    printf("\n \t\t          6) Reoganisation avec la table d'index. \n") ;
    printf("\n \t\t          7) Afficher la table d'index.\n");
    printf("\n \t\t          8) sauvegarder la table d'index dans un fichier index.\n");
    printf("\n \t\t          9) Affichage du bloc i du fichier.\n");
    printf("\n \t\t          10) Affichage de tout le fichier.\n");
    printf("\n \t\t          11) Quitter.\n");

    printf("\n \n                          Veuillez introduire votre choix:") ;
    scanf("%d",&choix) ;
    printf("\n");
    switch(choix)
    {

        case 1:
            clrscr();
            acc3();
            printf("\n                             Creation d'un fichier   \n");
            printf("-------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n\tVeuillez introduire le nom dU fichier que vous voulez creer : ");
            scanf(" %[^\n]" , filename);
            printf("\n\tVeuillez introduire le nombre d'enregistrement que vous voulez inserer : ");
            scanf("%d",&nb_enreg);
            creer_fichier( filename , nb_enreg );
            system("pause");
            goto menup;
            break;

        case 2:
            clrscr();
            acc3();
            printf("\n                             Recherche d'une clé dans un fichier     \n");
            printf("-------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n\tVeuillez introduire le nom du fichier dans lequel vous voulez effectuer la recherche : ");
            scanf(" %[^\n]" , filename);
                fich = Ouvrir( filename , 'a' );
                do
                {
                    printf("\n\tVeuillez introduire la cle que vous cherchez : ");
                    scanf("%d",&cle);
                }
                while ( cle > x );
                recherche (fich , cle , &trouv , &i , &j );
                if ( trouv == 0 )
                {
                    printf("\n\n\tLa cle que vous cherchez n'existe pas dans le fichier\n\n");
                }
                else
                {
                    printf("\n\n\tLa cle que vous cherchez existe dans le fichier\n");
                    printf("\n\tNumero du bloc\t\t\t: %d\n" , i );
                    printf("\n\tDeplacement dans le bloc\t: %d\n\n" , j );
                }
                fermer(fich);

            system("pause");
            goto menup;
            break;

        case 3:
            clrscr();
            acc3();
            printf("\n                             Insertion dans un fichier     \n");
            printf("-------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n\tVeuillez introduire le nom de fichier dans lequel vous voulez effectuer l'insertion : ");
            scanf(" %[^\n]" , filename);
                fich = Ouvrir( filename , 'a' );
                do
                {
                    printf("\n\tDonnez la cl%c : ",130);
                    scanf("%d",&cle);
                } while ( cle > x || cle < 0 );
                printf("\tDonnez l'info: ");
                scanf(" %56[^\n]" , info);
                insertion( fich , cle , info );
            fermer(fich);
            system("pause");
            goto menup;
            break;

        case 4:
            clrscr();
            acc3();
            printf("\n                             Suppression dans un fichier     \n");
            printf("-------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n\tVeuillez introduire le nom du fichier dans lequel vous voulez effectuer la suppression logique : ");
            scanf(" %[^\n]" , filename);
                fich = Ouvrir( filename , 'a' );
                Suppression(fich);
                printf("\t----------Afficharge de l'entete----------\n");
                Afficher_Entete(fich);
                if((fich->entete.nbrSupp/fich->entete.nb_caract_insr)>0.5)///si le taux de suppression dépasse 50% la réorganisation est lancée automatiquement
                {
                    reorganisation(fich,"org.txt");
                    printf("Une reorganisation automatique a ete lancee! \n");
                }


                fermer(fich);
                system("pause");
                goto menup;
            break;

        case 5:
            clrscr();
            acc3();
            printf("\n                             Reorganisation d'un fichier     \n");
            printf("-------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n\tVeuillez introduire le nom du fichier que vous voulez reorganiser : ");
            scanf(" %[^\n]" , filename);

                fich = Ouvrir( filename , 'a' );
                printf("\n\tVeuillez introduire le nom du nouveau fichier reorganise : ");
                scanf(" %[^\n]" , filename1);
                reorganisation( fich , filename1 );
               system("pause");
               goto menup;
            break;
        case 6 :
            clrscr();
            acc3();
            printf("\n                             Reorganisation d'un fichier avec la table d'index     \n");
            printf("-------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n\tVeuillez introduire le nom du fichier que vous voulez reorganiser : ");
            scanf(" %[^\n]" , filename);

                fich = Ouvrir( filename , 'a' );
                reorganisation_avec_index(fich,T,&taille);
                fermer(fich);
                system("pause");
                goto menup;
            break;
        case 7 :

            clrscr();
            acc3();
            printf("\n                            Afficher la table d'index     \n");
            printf("-------------------------------------------------------------------------------------\n");
            printf("\n");
            Afficher(T,taille);
            system("pause");
                goto menup;
            break;
        case 8 :
            clrscr();
            acc3();
            printf("\n                            Sauvegarder dans la table d'index     \n");
            printf("-------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n\tVeuillez introduire le nom du fichier que vous voulez reorganiser : ");
            scanf(" %[^\n]" , filename);


            fich = Ouvrir( filename , 'a' );
            fich_index=fopen("index.txt","w");
            Sauvegarder(T,taille,fich_index,fich);
            printf("Sauvegarde de la table d'index terminee.\n");
            fclose(fich_index);
            fermer(fich);
            system("pause");
            goto menup;
            break;

        case 9 :
           clrscr();
            acc3();
            printf("\n                            Affichage du Bloc i du fichier      \n");
            printf("-------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n\tVeuillez introduire le nom du fichier : ");
            scanf(" %[^\n]" , filename);
            printf("\n\t Veuillez introduire le numero du bloc que vous voulez afficher: ");
            scanf("%d",&d);
            fich = Ouvrir( filename , 'a' );
            Afficher_Bloc(fich,d);
            fermer(fich);
            system("pause");
            goto menup;
            break;

        case 10 :
           clrscr();
            acc3();
            printf("\n                            Affichage de tout le fichier       \n");
            printf("-------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n\tVeuillez introduire le nom du fichier : ");
            scanf(" %[^\n]" , filename);
            fich = Ouvrir( filename , 'a' );
            Afficher_tout_le_fichier(fich);
            fermer(fich);
            system("pause");
            goto menup;
            break;



       case 11:

                      acc3();
                      exit(0);
                     break ;

        }



















    return 0;
}
