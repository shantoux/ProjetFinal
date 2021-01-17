#ifndef UTILS
#define UTILS

//-------------------------------||:INCLUDE & DEFINE:||----------------------------------||

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#define N 10
#define M 10
#define clrscr() printf("\033[H\033[2J") // la fonction clrscr(); effacera l'écran

//-------------------------------||:STRUCTURES:||----------------------------------||

typedef struct Coordonnees Coordonnees; //les coordonnees determinent une case précise dans la matrice.
struct Coordonnees
{
    int x;
    int y;
};

typedef enum Dir  {
  NORD, SUD, EST, OUEST, NOUEST, NE, SO, SE
} Dir;

typedef enum Statut {
  SAIN, ASYMPTO, MALADE, MORT
} Statut;

typedef struct Bonhomme Bonhomme;
struct Bonhomme
{
    bool vocation; //indique si le bonhomme est un soigneur (1) ou simple civil (0)
    Dir direction; // N, S, E, O, NO, NE, SO, SE//donne la dernière direction du Bonhomme
    Statut etat; //SAIN, ASYMPTO, MALADE, MORT
    Coordonnees localisation; //A voir comment les gerer de manière pertinente. Pointeur ? localisation.x = "N" et localisation.y = "M"
    int tmp_infection; //pour les asymptomatiques, indique le nombre de tour encore infecté.
};

typedef struct Case Case; //utiliser un pointeur pour le décompte des virus.
struct Case
{
    bool occupee; //indique la présence d'une des trois entités possibles
    int gradient; //indique le niveau de charge virale (dû à un malade)
    int  PV_virus; //indique le "temps de vie" restant à un virus sur la case
    Coordonnees* virus_present;
    Bonhomme* lambda_present; //j'ai pas réussi à utiliser les pointeurs ici, il faudra voir si c'est handicapant pour la suite
    Bonhomme* soignant_present;//ici non plus
};


//----------------PROTOTYPES DE FONCTIONS COMMUNES------------------------------------------------------------------
int pioche(int min, int max);
int probabilite(int PROBA1, int PROBA2, int PROBA3); //permet de choisir entre 2 ou 3 options en fonction de leur % de chance de séléction.
int msleep(long msec);


//----------------------------------------------------------------------------------------------------------------------------------------
#endif
