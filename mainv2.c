//-------------------------------||:FICHIER PRINCIPAL:||----------------------------------||

#include "utilsv2.h"
//#include "lancement.h"
#include "initialisationv2.h"
#include "deplacementv2.h"
#include "affichagev2.h"

int main(int argc, char* argv[])
{
    //Déclaration des variables qui seront visibles dans tout le programme (plus simple que extern tout ça)
    int vie_virus[5] = {0,1,2,3,4}; //Tableau vers lequel pointe cpt_virus
    Case emplacement[N][M]; //initialisation de l'environnement avec des valeurs à 0

    //Déclaration des tableaux
    Bonhomme lambda[N*M];
    Bonhomme soignant [N*M];
    Coordonnees virus [N*M];
    int cpt_lambda=0;
    int cpt_virus=0;
    int cpt_soignant=0;
    int cpt_infecte=0;
    int tours_simulation_max = 0;
    srand((unsigned int)time(NULL));


    printf("Veuillez entrer le nombre de tours de simulation que vous souhaitez réaliser : \n");
    scanf("%d", &tours_simulation_max);

    initialisation(soignant, lambda, virus, N*M, &cpt_lambda, &cpt_virus, &cpt_soignant, N, M, emplacement, vie_virus);

    //Affiche la grille initiale
    afficher(N, M, emplacement);
   msleep(100);
   clrscr();
   int i = 0;

    //BOUCLE DE SIMULATION

    for (i = 0; i < tours_simulation_max; i++)
    
    {
      deplacement(soignant, &cpt_soignant, lambda, &cpt_lambda, virus,
                  &cpt_virus, N, M, emplacement);

      afficher(N, M, emplacement);
       msleep(100);
       clrscr();
    }


    return 0;
}
