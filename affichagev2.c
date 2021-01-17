#include "affichagev2.h"

//modifier pour ajouter les cas "affectés".

//------------------------------||:FONCTION:||---------------------------------|

void afficher(int nrow, int ncol, Case matrice[nrow][ncol])
{
  int i, j;

  putchar('\n');
  putchar('+');

  for (j = 1; j <= ncol; j++)
      printf("---+");

  putchar('\n');

  for (i = 0; i < nrow; i++)
  {
      putchar('|');

      for (j = 0; j < ncol; j++)
      {
        if (matrice[i][j].lambda_present != NULL)
        {
            if (matrice[i][j].lambda_present->etat == SAIN)
            {
                affichage_direction(matrice[i][j].lambda_present -> direction);
              printf("|");
            }
            else if (matrice[i][j].lambda_present -> etat == ASYMPTO)
            {
              couleur("33");
                affichage_direction(matrice[i][j].lambda_present -> direction);
              couleur("0");
              printf("|");
            }
            else if (matrice[i][j].lambda_present->etat == MALADE)
            {
              couleur("31");
              printf("%2c ", 'M');
              couleur("0");
              printf("|");
            }
          }
          else if (matrice[i][j].soignant_present != NULL)
          {
            if (matrice[i][j].soignant_present->etat == SAIN)
            { couleur("32;1");
              printf("%2c ", 'S');
              couleur("0");
              printf("|");
            }
            else if (matrice[i][j].soignant_present -> etat == ASYMPTO)
            {
              couleur("33;1");
              printf("%2c ", 'S');
              couleur("0");
              printf("|");
            }
            else if (matrice[i][j].soignant_present -> etat == MALADE)
            {
              couleur("31");
              printf("%2c ", 'M');
              couleur("0");
              printf("|");
            }
          }
        else if (matrice[i][j].PV_virus > 0)
        {
          couleur("41");
          printf("   ");
          couleur("0");
          printf("|");
        }
        else
        {
            printf("   |");
        }
      }
      putchar('\n');
      putchar('+');

      for (j = 1; j <= ncol; j++)
        {
          printf("---+");
        }

      putchar('\n');

  }
}


void affichage_direction(Dir direction)
{
  if (direction == NORD) { printf(" ∧ ");}
  else if (direction == SUD) { printf(" ∨ ");}
  else if (direction == EST) { printf(" > ");}
  else if (direction == OUEST) { printf(" < ");}
  else if (direction == NE) { printf(" / ");}
  else if (direction == SO) { printf(" / ");}
  else if (direction == NOUEST) { printf(" \\ ");}
  else if (direction == SE) { printf(" \\ ");}
}
