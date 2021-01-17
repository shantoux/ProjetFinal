#include "initialisationv2.h"


//------------------------------||:FONCTION:||---------------------------------||

void initialisation(Bonhomme soignant[], Bonhomme lambda[], Coordonnees virus[], int taille_tab, int * cpt_lambda, int * cpt_virus, int * cpt_soignant, int nrow, int ncol, Case emplacement[nrow][ncol], int vie_virus[])
{
    //ok donc maintenant que les structures et les listes sont mises en place, y'a plus qu'à.
    //1. on initialise une première grille de N par M cases. Il faut faire le tour de chaque case et faire poper les soigneurs, les virus et les civils.
    //2. à chaque fois qu'un soigneur ou un civil apparait, il faut l'introduire dans Bonhomme/soigneur ou civil pour l'ajouter aux listes.
    //Ne pas oublier d'initialiser chaque bonhomme.

    // INITIALISATION
    int i, j;

    for (i = 0; i < N*M; i++)
    {
      lambda[i].vocation = 0;
      lambda[i].localisation.y = 0;
      lambda[i].localisation.x = 0;
      lambda[i].etat = MORT;
      lambda[i].tmp_infection = 0;
      lambda[i].direction = 0;
      soignant[i].vocation = 0;
      soignant[i].localisation.y = 0;
      soignant[i].localisation.x = 0;
      soignant[i].etat = MORT;
      soignant[i].tmp_infection = 0;
      soignant[i].direction = 0;
    }

    for (i = 0; i < N; i++)
    {
      for (j = 0; j < M; j++)
      {
        emplacement[i][j].PV_virus = vie_virus[0];//initialisation du compteur de vie du virus pour cette case
        emplacement[i][j].occupee = 0;
        emplacement[i][j].gradient = 0;
        emplacement[i][j].virus_present = NULL;
        emplacement[i][j].lambda_present = NULL;
        emplacement[i][j].soignant_present = NULL;

        int resultat = probabilite(PROB_V, PROB_L, PROB_S);
          //si resultat == 4, alors la case reste vide.

        if (resultat == 1)
        {
        //alors présence d'un virus
            emplacement[i][j].PV_virus = vie_virus[4];//initialisation du compteur de vie du virus pour cette case
            emplacement[i][j].virus_present = &(virus[*cpt_virus]);
            virus[*cpt_virus].y = i;
            virus[*cpt_virus].x = j;
            *cpt_virus += 1;

        }
        else if (resultat == 2)
        {
            //alors presence d'un lambda
            emplacement[i][j].occupee = 1;
            emplacement[i][j].lambda_present = &(lambda[*cpt_lambda]);
            lambda[*cpt_lambda].localisation.y = i;
            lambda[*cpt_lambda].localisation.x = j;
            lambda[*cpt_lambda].etat = SAIN;
            lambda[*cpt_lambda].tmp_infection = 0;
            attribution_direction(&(lambda[*cpt_lambda]));
            *cpt_lambda += 1;

        }
        else if (resultat == 3)
        {   //presence d'un soignant
            emplacement[i][j].occupee = 1;
            emplacement[i][j].soignant_present = &(soignant[*cpt_soignant]);
            soignant[*cpt_soignant].localisation.y = i;
            soignant[*cpt_soignant].localisation.x = j;
            soignant[*cpt_soignant].vocation = 1;
            soignant[*cpt_soignant].etat = SAIN;
            soignant[*cpt_soignant].tmp_infection = 0;
            attribution_direction(&(soignant[*cpt_soignant]));
            *cpt_soignant += 1;
        }

        //printf("virus : %d, lambda : %d, soignant : %d\n", *cpt_virus, *cpt_lambda, *cpt_soignant);
       }
     }

}


//-------------------------------||:IMPLEMENTATION DES FONCTIONS:||----------------------------------||

void attribution_direction(Bonhomme *entite) //attribue une direction parmi les 8 possibles
{
  Dir a = 0;
    int res = 0;
    res = pioche(0,7);
    if (res == 0)
        a = NORD;
    else if (res == 1)
        a = SUD;
    else if (res == 2)
        a = EST;
    else if (res == 3)
        a = OUEST;
    else if (res == 4)
        a = NOUEST;
    else if (res == 5)
        a = NE;
    else if (res == 6)
        a = SO;
    else if (res == 7)
        a = SE;
    else
    {   a = 99;
        printf("erreur assignement direction \n");
    }
    entite->direction = a;
}
