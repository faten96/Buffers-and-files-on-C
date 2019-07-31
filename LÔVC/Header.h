#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#define b 100
typedef struct Tbloc
{
    char tab[b];  /*le tableau de chaine de caracteres*/
    int suivant;    /*le numero du bloc suivant*/
}Tbloc;

typedef struct index
{
    char cle[21];
    int bloc;
    int pos;
}index;

typedef Tbloc Buffer;
typedef struct Entete
{
    int nbrBloc;
    int tete;
    int queue;
    int indice_Libre;
    float nbrSupp;
    float nb_caract_insr;

}Entete;

typedef struct LNOVC
{
    FILE *fich;
    Entete entete;
}LNOVC;

///mettre à jour un element de l'entete
void affEntete(LNOVC *fichier,int i,int valeur);
///retourne la valeur d'un element de l'entete
int entete(LNOVC *fichier,int i);
///lire dans 'buf' le contenu du bloc numéro 'i' du fichier 'f'
void LireDir(LNOVC *fichier,int i, Buffer *Buf);
///écrit le contenu de la variable 'buf' dans le bloc numéro 'i' du fichier
void EcrireDir(LNOVC *fichier,int i, Buffer *Buf);
///Permet d'ouvrir le ficheir
LNOVC *Ouvrir(char nom_physique[],char mode);
///Fermer le fichier
void fermer(LNOVC *fichier);
///Allouer un nouveau bloc
void allocBloc(LNOVC *fichier);


char* ajout_zeros(char* chaine,  int nb) ;
char* concat(char* effac, int cle, char* info);
char* recup_chaine ( int n , int* i , int* j , Buffer* buf , LNOVC* fichier );
char* LireChaine ( int n , int* i , int* j , Buffer* buf , LNOVC * fichier );
void ecrire_chaine ( char chaine[] , int n , int* i , int* j , Buffer* buf , LNOVC* fichier );
void recherche (LNOVC* fichier , int cle , int* trouv , int* i, int* j );
void insertion ( LNOVC* fichier , int cle , char* info );
void creer_fichier(char nomfich[] , int nb_enreg);
void Suppression(LNOVC *fichier);
void reorganisation ( LNOVC* fich , char* nouveau );
void acc();
void acc3();
void signe_esi();
void Afficher(index tab[],int taille);
void Sauvegarder(index tab[],int taille,FILE *fichier_index,LNOVC *fichier);
void turn_to_string(char chaine[], int n, int longueur);
/** Réorganisation du fichier après suppression **/
void reorganisation_avec_index ( LNOVC* fich ,index tab[],int *taille_Index );
void Afficher_Entete (LNOVC* fich);
void Afficher_Bloc ( LNOVC* fich , int bloc );
void Afficher_tout_le_fichier (LNOVC* fich);



#endif // HEADER_H_INCLUDED
