#ifndef INITIALISATION
#define INITIALISATION

//------------------------------------||:INCLUDE & DEFINE:||-------------------------------------||

#include "utilsv2.h"
#define PROB_V 5
#define PROB_L 20
#define PROB_S 10

//-------------------------------||:PROTOTYPES DES FONCTIONS:||----------------------------------||

void initialisation(Bonhomme soignant[], Bonhomme lambda[], Coordonnees virus[], int taille_tab, int * cpt_lambda, int * cpt_virus, int * cpt_soignant, int nrow, int ncol, Case emplacement[nrow][ncol], int vie_virus[]);
void attribution_direction(Bonhomme *entite);

//---------------------------------------------------------------------------------------------------

#endif
