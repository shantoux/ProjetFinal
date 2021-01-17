#include "deplacementv2.h"

//------------------------------||:FONCTION:||---------------------------------|

void deplacement(Bonhomme soignant[], int *cpt_soignant, Bonhomme lambda[],
  int *cpt_lambda, Coordonnees virus[], int *cpt_virus, int nrow, int ncol,
  Case emplacement[nrow][ncol])
{
    int i;

    // [Lecture tableau lambda]

    for (i = 0; i < *cpt_lambda; i++)
    {

        if (lambda[i].etat != MALADE && lambda[i].etat != MORT) //Si lambda est SAIN ou ASYMPTO
        {
            if (emplacement[lambda[i].localisation.y][lambda[i].localisation.x].gradient == 2) // et gradient == 2
            {
                Dir malade_en_vue = attraction(lambda, i, N, M, emplacement); // lambda est repoussé
                if (malade_en_vue != 0)
                {
                    liberation_case(&(emplacement[lambda[i].localisation.y][lambda[i].localisation.x]));
                    Changement_coordonnees(lambda, i, malade_en_vue, N, M);
                    lambda[i].direction = malade_en_vue;
                    nouvelle_case(&(emplacement[lambda[i].localisation.y][lambda[i].localisation.x]), lambda, i);
                }
            }
            else
            {
                Dir malade_en_vue = repulsion(lambda, i, N, M, emplacement);
                if (malade_en_vue != 0)
                {
                    liberation_case(&(emplacement[lambda[i].localisation.y][lambda[i].localisation.x]));
                    Changement_coordonnees(lambda, i, malade_en_vue, N, M);
                    lambda[i].direction = malade_en_vue;
                    nouvelle_case(&(emplacement[lambda[i].localisation.y][lambda[i].localisation.x]), lambda, i);
                }
                else
                    quel_chemin(lambda, i, N, M, emplacement);
             }
        }

        if (lambda[i].etat == MALADE)
        {
            bool soin_bool = soin(&(lambda[i]), N, M, emplacement);
            int mortel = probabilite(PROB_MOURIR, PROB_VIVRE, 0);
            if (mortel == 1)//lambda meurt
            {
                zone_gradient(lambda[i], N, M, emplacement,1); //retrait de zone de gradient
                lambda[i].etat = MORT; //???
                liberation_case(&(emplacement[lambda[i].localisation.y][lambda[i].localisation.x]));
                emplacement[lambda[i].localisation.y][lambda[i].localisation.x].PV_virus = 4;
                emplacement[lambda[i].localisation.y][lambda[i].localisation.x].virus_present = 1;
                *cpt_virus++;
                virus[*cpt_virus].x = lambda[i].localisation.x;
                virus[*cpt_virus].y = lambda[i].localisation.y;

            }
            if (mortel == 2 && soin_bool ==1)
            {
                zone_gradient(lambda[i], N, M, emplacement,1); // retrait de la zone de gradient
                                                              //car soin en cours
            }
        }

        if (lambda[i].etat == ASYMPTO) //si je suis asymptomatique :
        {
            lambda[i].tmp_infection -= 1;
            if (lambda[i].tmp_infection == 0)     //si tmp_infection tombe à 0, il redevient sain
            {
                lambda[i].etat = SAIN;
            }
            else
            {
                Case* choix;
                contamination(lambda[i], N, M, emplacement, &choix); //P chance de contaminer une des cases autour de l'asymptomatique.
                //Retourne la case en question et :
                if (choix->occupee != 0)
                {
                    if (choix->lambda_present != NULL)
                    {
                        if (choix->lambda_present->etat == SAIN)
                        {
                            infection(choix->lambda_present);
                        }
                    }
                    else
                    {
                        if (choix->soignant_present != NULL)
                        {
                            if (choix->soignant_present->etat == SAIN)
                            {
                                choix->PV_virus = 4;
                                choix -> virus_present = 1;
                                *cpt_virus ++;
                                virus[*cpt_virus].x = choix.localisation.x;
                                virus[*cpt_virus].y = choix.localisation.y;

                            }
                        }
                    }
                }
                else
                {
                    choix->PV_virus = 4;
                    choix -> virus_present = 1;
                    *cpt_virus++;
                    virus[*cpt_virus].x = choix.localisation.x;
                    virus[*cpt_virus].y = choix.localisation.y;

                }
            }
        }

        if (emplacement[lambda[i].localisation.y][lambda[i].localisation.x].PV_virus != 0 && lambda[i].etat == SAIN)
        {
            emplacement[lambda[i].localisation.y][lambda[i].localisation.x].PV_virus = 0; //un2moins
            infection(&(lambda[i]));
            zone_gradient(lambda[i], N, M, emplacement, 0);
        }
    }

////////////////////// Lecture du tableau soignant ////////////////////////////////////////////////

    for (i = 0; i < MAX_S; i++)
    {

        if ((*soignant)[i].etat != MALADE) && (*soignant[i]).etat != MORT)) // si soignant ASYMPTO ou SAIN
        {
            if(emplacement[(soignant)[i].localisation.y][(soignant)[i].localisation.x].gradient != 2)
            //si gradient == 2, le soin s'active et il n'a pas à bouger tant que le gradient est présent
            {
                Dir malade_en_vue = attraction(lambda, i, N, M, emplacement);
                if (malade_en_vue != 0)
                {
                  liberation_case(&(emplacement[soignant[i].localisation.y][soignant[i].localisation.x]));
                  Changement_coordonnees(soignant, i, malade_en_vue, N, M);
                  soigneur[i].direction = malade_en_vue;
                  nouvelle_case(&(emplacement[soignant[i].localisation.y][soignant[i].localisation.x]), soignant, i);
                }
                else
                    quel_chemin(soignant, i, N, M, emplacement);
            }

        }

        if (soignant[i].etat == MALADE) // Si soignant Malade
        {
            bool soin_bool = soin(&(soignant[i]), N, M, emplacement);
            int mortel = probabilite(PROB_MOURIR, PROB_VIVRE, 0);
            if (mortel == 1)//soignant meurt
            {
                zone_gradient(soignant[i], N, M, emplacement,1); //retrait de zone de gradient
                soignant[i].etat = MORT;
                liberation_case(&(emplacement[soignant[i].localisation.y][soignant[i].localisation.x]));
                emplacement[soignant[i].localisation.y][soignant[i].localisation.x].PV_virus = 4;
                emplacement[soignant[i].localisation.y][soignant[i].localisation.x].virus_present = 1;
                *cpt_virus++;
                virus[*cpt_virus].x = soignant[i].localisation.x;
                virus[*cpt_virus].y = soignant[i].localisation.y;

            }
            if (mortel == 2 && soin_bool ==1) //si le soignant vit et qu'il y a un soignant à proximité
            {
                zone_gradient(soignant[i], N, M, emplacement,1); // retrait de la zone de gradient
                                                              //car soin en cours
            }
        }

        if (soignant[i].etat == ASYMPTO) //si je suis asymptomatique :
        {
            soignant[i].tmp_infection--;
            if (soignant[i].tmp_infection == 0)     //si tmp_infection tombe à 0, il redevient sain
            {
                soignant[i].etat = SAIN;
            }
            else
            {
                Case* choix;
                contamination(soignant[i], N, M, emplacement, &choix); //P chance de contaminer une des cases autour de l'asymptomatique.
                //Retourne la case en question et :
                if (choix->occupee != 0)
                {
                    if (choix->lambda_present != NULL)
                    {
                        if (choix->lambda_present->etat == SAIN)
                        {
                            infection(choix->lambda_present);
                        }
                    }
                    else
                    {
                        if (choix->soignant_present != NULL)
                        {
                            if (choix->soignant_present->etat == SAIN)
                            {
                                choix->PV_virus = 4;
                                choix -> virus_present = 1;
                                *cpt_virus++;
                                virus[*cpt_virus].x = choix.localisation.x;
                                virus[*cpt_virus].y = choix.localisation.y;
                            }
                        }
                    }
                }
                else
                {
                    choix->PV_virus = 4;

                    *cpt_virus++;
                    virus[*cpt_virus].x = choix.localisation.x;
                    virus[*cpt_virus].y = choix.localisation.y;
                }
            }
        }

        if (emplacement[soignant[i].localisation.y][soignant[i].localisation.x].PV_virus != 0) && (soignant[i].etat == SAIN)) //si je suis sur une case virus et pas infecté
        {
            emplacement[(soignant[i].localisation.y)][(soignant[i].localisation.x)].PV_virus = 0;
            int a = 0;
            for (a = 0 ; a < *cpt_virus ; a++)
            {
                if (emplacement[(soignant[i].localisation.y)] == virus[*cpt_virus].x && emplacement[(soignant[i].localisation.y)] == virus[*cpt_virus].y)
                {
                    virus[*cpt_virus].x = NULL ;
                    virus[*cpt_virus].y = NULL ;
                    *cpt_virus--;
                }
            }
            infection(&(soignant[i]));
            zone_gradient(soignant[i], N, M, emplacement, 0);
        }
    }

    for (i = 0; i < MAX_V ; i++)
    {
        if (emplacement[virus[i].y][virus[i].x]. == 1)
        {
            if (emplacement[virus[i].y][virus[i].x].PV_virus == 0) //cas ou le virus a ete supprimé à cause de l'infection d'un bonhomme sur ce tour
            {
                emplacement[(virus[i].y][virus[i].x].virus_present == 0;
                int a = 0
                virus[*cpt_virus].x = -1 ; //pour indiquer qu'il n'est plus là
                virus[*cpt_virus].y = -1 ;
                *cpt_virus--;
            }
            else
            {
                emplacement[virus[i].y][virus[i].x].PV_virus--;
                if (emplacement[virus[i].y][virus[i].x].PV_virus == 0)
                {
                    emplacement[(virus[i].y][virus[i].x].virus_present == 0;
                    virus[*cpt_virus].x = -1 ;
                    virus[*cpt_virus].y = -1 ;
                    *cpt_virus--;
                }
            }
        }
    }
}

//------------------- implémentation fonctions -----------------------------------------------


//en fonction de la direction de départ, utilise probabilité() et choisi
//une nouvelle direction
void quel_chemin(Bonhomme bonhomme[], int indice, int nrow, int ncol,
  Case emplacement[nrow][ncol])
{

    int resultat = probabilite(PROB_ARRET, PROB_DEPLACEMENT, 0); //ARRET 30%, DEPLACEMENT 70%
    Dir direction_temp = 0;
    //on stock la nouvelle direction de manière temporaire si jamais il y a un obstacle.
    if (resultat == 2) //le Bonhomme se déplace
    {
         resultat = probabilite(PROB_ELSE, PROB_SAME, 0);
         if (resultat == 2) //Garde la même direction
         {
           direction_temp = bonhomme[indice].direction;
         }
         else
         {
           direction_temp = pioche_nouvelle(bonhomme[indice].direction);
         }
         if (obstacle_bonhomme(direction_temp, N, M, emplacement, bonhomme, indice) == 1) //si pas d'obstacle
         {
              liberation_case(&(emplacement[bonhomme[indice].localisation.y][bonhomme[indice].localisation.x]));
              Changement_coordonnees(bonhomme, indice, direction_temp, N, M);
              bonhomme[indice].direction = direction_temp;
              nouvelle_case(&(emplacement[bonhomme[indice].localisation.y][bonhomme[indice].localisation.x]), bonhomme, indice);
         }
         else //si il y a un obstacle
         {
             colision(direction_temp, N, M, emplacement, bonhomme, indice);
         }
    }
}

void colision(Dir direction_temp, int nrow, int ncol, Case emplacement[nrow][ncol], Bonhomme bonhomme[], int indice) //si sur la case de déplacement se trouve quelqu'un, on ne peut pas y deplacer le bonhomme. dans ce cas 50-50 de s'arrêter ou de changer de direction
{
    int libre = 0; //si 0 : la case est prise, si 1 : la case est libre
    Dir new_temp;
    while (libre < 1)
    {
        int resultat = pioche(1,2);
        if (resultat == 1) //le perso s'arrête
        {
            //(*bonhomme)[i].direction = 0;
            libre = 2;
        }
        else
        {
            new_temp = pioche_nouvelle(direction_temp);
            libre = obstacle_bonhomme(new_temp, N, M, emplacement, bonhomme, indice);
        }
    }
    if (libre == 1) //il décide de bouger et la case est libre
    {
      liberation_case(&(emplacement[bonhomme[indice].localisation.y][bonhomme[indice].localisation.x]));
      Changement_coordonnees(bonhomme, indice, new_temp, N, M);
      bonhomme[indice].direction = new_temp;
      nouvelle_case(&(emplacement[bonhomme[indice].localisation.y][bonhomme[indice].localisation.x]), bonhomme, indice);
    }
  }

  void Changement_coordonnees(Bonhomme entite[], int indice, Dir nouvelle, int Long, int Larg)
  {
    if (nouvelle == NORD) //dans le sens des y négatifs
    {
      entite[indice].localisation.y = (entite[indice].localisation.y -1 + Long)%Long;
    }
    else if (nouvelle == SUD)
    {
        entite[indice].localisation.y = (entite[indice].localisation.y +1)%Long;

    }
    else if (nouvelle == EST)
    {
        entite[indice].localisation.x = (entite[indice].localisation.x +1)%Larg;

    }
    else if (nouvelle == OUEST)
    {
        entite[indice].localisation.x = (entite[indice].localisation.x -1 + Larg)%Larg;
    }
    else if (nouvelle == NOUEST)
    {
        entite[indice].localisation.y = (entite[indice].localisation.y -1 + Long)%Long;
        entite[indice].localisation.x = (entite[indice].localisation.x -1 + Larg)%Larg;
    }
    else if (nouvelle == NE)
    {
        entite[indice].localisation.y = (entite[indice].localisation.y -1 + Long)%Long;
        entite[indice].localisation.x = (entite[indice].localisation.x +1)%Larg;
    }
    else if (nouvelle == SO)
    {
        entite[indice].localisation.y = (entite[indice].localisation.y +1)%Long;
        entite[indice].localisation.x = (entite[indice].localisation.x -1 + Larg)%Larg;
    }
    else if (nouvelle == SE)
    {
        entite[indice].localisation.y = (entite[indice].localisation.y +1)%Long;
        entite[indice].localisation.x = (entite[indice].localisation.x +1)%Larg;
    }
  }

  void liberation_case(Case * place)
  {
    place->occupee = 0;
    place->lambda_present = NULL;
    place->soignant_present = NULL;
  }

  void nouvelle_case(Case * place, Bonhomme entite[], int indice)
  {
    place->occupee = 1;
    if (entite[indice].vocation == 1)
    {
      place->soignant_present = &(entite[indice]);
    }
    else
    {
      place->lambda_present = &(entite[indice]);
    }
  }


  Dir pioche_nouvelle(Dir ancienne)
{
    Dir tab[8] = {NORD, SUD, EST, OUEST, NOUEST, NE, SO, SE};
    int a;
    do
    {
      a = pioche(0,7);
    } while (tab[a] == ancienne);
    return tab[a];
}

  int obstacle_bonhomme(Dir nouvelle, int Long, int Larg, Case matrice[Long][Larg], Bonhomme entite[], int indice)
  {
    if (nouvelle == NORD)
    {
      if (matrice[(entite[indice].localisation.y-1+Long)%Long][entite[indice].localisation.x].occupee == 1)
        return 0;
      else
        return 1;
    }
    else if (nouvelle == SUD)
    {
      if (matrice[(entite[indice].localisation.y+1)%Long][entite[indice].localisation.x].occupee == 1)
        return 0;
      else
        return 1;
    }
    else if (nouvelle == EST)
    {
      if (matrice[entite[indice].localisation.y][entite[indice].localisation.x+1].occupee == 1)
        return 0;
      else
        return 1;
    }
    else if (nouvelle == OUEST)
    {
      if (matrice[entite[indice].localisation.y][(entite[indice].localisation.x-1+Larg)%Larg].occupee == 1)
        return 0;
      else
        return 1;
    }
    else if (nouvelle == NOUEST)
    {
      if (matrice[(entite[indice].localisation.y-1+Long)%Long][(entite[indice].localisation.x-1+Larg)%Larg].occupee == 1)
        return 0;
      else
        return 1;
    }
    else if (nouvelle == NE)
    {
      if (matrice[(entite[indice].localisation.y-1+Long)%Long][(entite[indice].localisation.x+1)%Larg].occupee == 1)
        return 0;
      else
        return 1;
    }
    else if (nouvelle == SO)
    {
      if (matrice[(entite[indice].localisation.y+1)%Long][(entite[indice].localisation.x-1+Larg)%Larg].occupee == 1)
        return 0;
      else
        return 1;
    }
    else if (nouvelle == SE)
    {
      if (matrice[(entite[indice].localisation.y+1)%Long][(entite[indice].localisation.x+1)%Larg].occupee == 1)
        return 0;
      else
        return 1;
    }
    else
    {   return 99;
        printf("erreur dans l'obstacle");
    }
  }

  void infection(Bonhomme *bonhomme)
{
  int resultat = probabilite(PROB_MALADE, PROB_ASYMPTO, 0);
  if (resultat == 1)
  {   //tombe malade
      bonhomme->etat = MALADE;
      bonhomme->tmp_infection = 2;
      // zone_gradient
  }
  else //asymptomatique
  {
      bonhomme->etat = ASYMPTO;
      bonhomme->tmp_infection = 7;
  }
}

void contamination(Bonhomme bonhomme, int nrow, int ncol, Case matrice[nrow][ncol], Case **emplacement_choisi)
{
  int resultat = pioche(1,9);
  switch (resultat)
  {
  case 1 :
      *emplacement_choisi = &(matrice[(bonhomme.localisation.y-1+nrow)%nrow][(bonhomme.localisation.x+1)%ncol]);//case au nord est
      break;
  case 2 :
      *emplacement_choisi = &(matrice[bonhomme.localisation.y][(bonhomme.localisation.x+1)%ncol]); //case à l'est
      break;
  case 3 :
      *emplacement_choisi = &(matrice[(bonhomme.localisation.y+1)%nrow][(bonhomme.localisation.x+1)%ncol]); //case au sud-est
      break;
  case 4 :
      *emplacement_choisi = &(matrice[(bonhomme.localisation.y-1+nrow)%nrow][bonhomme.localisation.x]); //case à l'ouest
      break;
  case 5 :
      *emplacement_choisi = &(matrice[bonhomme.localisation.y][bonhomme.localisation.x]); //case de l'asymptomatique
      break;
  case 6 :
      *emplacement_choisi = &(matrice[(bonhomme.localisation.y+1)%nrow][bonhomme.localisation.x]); //case à l'est
      break;
  case 7 :
      *emplacement_choisi = &(matrice[(bonhomme.localisation.y-1+nrow)%nrow][(bonhomme.localisation.x-1+ncol)%ncol]); //case au nord-ouest
      break;
  case 8 :
      *emplacement_choisi = &(matrice[bonhomme.localisation.y][(bonhomme.localisation.x-1+ncol)%ncol]); //case à l'ouest
      break;
  case 9 :
      *emplacement_choisi = &(matrice[(bonhomme.localisation.y+1)%nrow][(bonhomme.localisation.x-1+ncol)%ncol]); //case au sud ouest
      break;
      default :;
  }
}


Dir attraction(Bonhomme bonhomme[], int indice, int nrow, int ncol, Case matrice[nrow][ncol]) //analyse  le signal de gradient pour lui indiquer le chemin.
{
    int n = 0;
    if (matrice[((bonhomme)[indice].localisation.y)][((bonhomme)[indice].localisation.x)].gradient != 1)
      //le gradient sera de 2 pour un lambda qui cherchera à se diriger vers un gradient n = 1 (va à l'opposer du malade)
      //le gradient sera de 0 pour un soignant qui cherchera à se diriger vers un gradient n = 1 (va vers le malade)
        n = 1;
    else //seulement dans le cas soignant
        n = 2;

    Dir un_1 = 0;

    if ((matrice[((bonhomme)[indice].localisation.y)-1][((bonhomme)[indice].localisation.x)+1].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)-1][((bonhomme)[indice].localisation.x)+1].occupee == 0))//case au nord-est
        un_1 = NE;
    else if ((matrice[(bonhomme)[indice].localisation.y][((bonhomme)[indice].localisation.x)+1].gradient == n)
    && (matrice[(bonhomme)[indice].localisation.y][((bonhomme)[indice].localisation.x)+1].occupee == 0)) //case à l'est
        un_1 = EST;
    else if ((matrice[((bonhomme)[indice].localisation.y)+1][((bonhomme)[indice].localisation.x)+1].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)+1][((bonhomme)[indice].localisation.x)+1].occupee == 0)) //case au sud-est
        un_1 = SE;
    else if ((matrice[((bonhomme)[indice].localisation.y)-1][(bonhomme)[indice].localisation.x].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)-1][(bonhomme)[indice].localisation.x].occupee == 0)) //case au nord
        un_1 = NORD;
    else if ((matrice[((bonhomme)[indice].localisation.y)+1][(bonhomme)[indice].localisation.x].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)+1][(bonhomme)[indice].localisation.x].occupee == 0)) //case au sud
        un_1 = SUD;
    else if ((matrice[((bonhomme)[indice].localisation.y)-1][((bonhomme)[indice].localisation.x)-1].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)-1][((bonhomme)[indice].localisation.x)-1].occupee == 0) ) //case au nord-ouest
        un_1 = NOUEST;
    else if ((matrice[(bonhomme)[indice].localisation.y][((bonhomme)[indice].localisation.x)-1].gradient == n)
    && (matrice[(bonhomme)[indice].localisation.y][((bonhomme)[indice].localisation.x)-1].occupee == 0)) //case à l'ouest
        un_1 = OUEST;
    else if ((matrice[((bonhomme)[indice].localisation.y)+1][((bonhomme)[indice].localisation.x)-1].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)+1][((bonhomme)[indice].localisation.x)-1].occupee == 0)) //case au sud-ouest
        un_1 = SO;
    return un_1;
}


//////////////////////////////////////////////////////////////////////////////////////////

Dir repulsion(Bonhomme bonhomme[], int indice, int nrow, int ncol, Case matrice[nrow][ncol]) //analyse autour du lambda le signal de gradient pour lui indiquer le chemin.
{
    int n = 0;
    if (matrice[((bonhomme)[indice].localisation.y)][((bonhomme)[indice].localisation.x)].gradient == 0)
        n = 1;
    else if (matrice[((bonhomme)[indice].localisation.y)][((bonhomme)[indice].localisation.x)].gradient == 1)
        n = 2;

    Dir un_1 = 0;

    if ((matrice[((bonhomme)[indice].localisation.y)-1][((bonhomme)[indice].localisation.x)+1].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)-1][((bonhomme)[indice].localisation.x)+1].occupee == 0))//case au nord-est
        un_1 = SO;
    else if ((matrice[(bonhomme)[indice].localisation.y][((bonhomme)[indice].localisation.x)+1].gradient == n)
    && (matrice[(bonhomme)[indice].localisation.y][((bonhomme)[indice].localisation.x)+1].occupee == 0)) //case à l'est
        un_1 = OUEST;
    else if ((matrice[((bonhomme)[indice].localisation.y)+1][((bonhomme)[indice].localisation.x)+1].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)+1][((bonhomme)[indice].localisation.x)+1].occupee == 0)) //case au sud-est
        un_1 = NOUEST;
    else if ((matrice[((bonhomme)[indice].localisation.y)-1][(bonhomme)[indice].localisation.x].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)-1][(bonhomme)[indice].localisation.x].occupee == 0)) //case au nord
        un_1 = SUD;
    else if ((matrice[((bonhomme)[indice].localisation.y)+1][(bonhomme)[indice].localisation.x].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)+1][(bonhomme)[indice].localisation.x].occupee == 0)) //case au sud
        un_1 = NORD;
    else if ((matrice[((bonhomme)[indice].localisation.y)-1][((bonhomme)[indice].localisation.x)-1].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)-1][((bonhomme)[indice].localisation.x)-1].occupee == 0) ) //case au nord-ouest
        un_1 = SE;
    else if ((matrice[(bonhomme)[indice].localisation.y][((bonhomme)[indice].localisation.x)-1].gradient == n)
    && (matrice[(bonhomme)[indice].localisation.y][((bonhomme)[indice].localisation.x)-1].occupee == 0)) //case à l'ouest
        un_1 = EST;
    else if ((matrice[((bonhomme)[indice].localisation.y)+1][((bonhomme)[indice].localisation.x)-1].gradient == n)
    && (matrice[((bonhomme)[indice].localisation.y)+1][((bonhomme)[indice].localisation.x)-1].occupee == 0)) //case au sud-ouest
        un_1 = NE;
    return un_1;
}


void zone_gradient(Bonhomme bonhomme, int nrow, int ncol, Case emplacement[nrow][ncol], bool etat) //marque autour du bonhomme la zone de signal avec un gradient en fonction de la proximité.
{
  int u = 0, k = 0, m =0;
  if (etat == 0) //cas le malade est vivant = création du gradient !
  {
      for (u = 0 ; u < 3 ; u++)
      {
          for (k = 0 ; k < 3 ; k++)
          {
              if(u+k > 0)
              {
                  if (u == 2 || k == 2)
                      m = 1;
                  else
                      m = 2;

                  emplacement[(bonhomme.localisation.y-u+nrow)%nrow][(bonhomme.localisation.x+k)%ncol].gradient = m;
                  emplacement[(bonhomme.localisation.y+u)%nrow][(bonhomme.localisation.x-k+ncol)%ncol].gradient = m;
                  emplacement[(bonhomme.localisation.y+u)%nrow][(bonhomme.localisation.x+k)%ncol].gradient = m;
                  emplacement[(bonhomme.localisation.y-u+nrow)%nrow][(bonhomme.localisation.x-k+ncol)%ncol].gradient = m;
              }
          }
      }
  }

  if (etat == 1) //cas ou le malade vient de mourir = supprime le gradient autour de sa case !
  {
    for (u = 0 ; u < 3 ; u++)
      {
          for (k = 0 ; k < 3 ; k++)
          {
            emplacement[(bonhomme.localisation.y-u+nrow)%nrow][(bonhomme.localisation.x+k)%ncol].gradient = 0;
            emplacement[(bonhomme.localisation.y+u)%nrow][(bonhomme.localisation.x-k+ncol)%ncol].gradient = 0;
            emplacement[(bonhomme.localisation.y+u)%nrow][(bonhomme.localisation.x+k)%ncol].gradient = 0;
            emplacement[(bonhomme.localisation.y-u+nrow)%nrow][(bonhomme.localisation.x-k+ncol)%ncol].gradient = 0;
          }
      }
  }
}


bool soin(Bonhomme *bonhomme, int nrow, int ncol, Case matrice[nrow][ncol]) //Si malade et soignant sont sur une case voisine, le malade sera guéri au bout de deux tours
{
    int cpt_soin =0;
    if (matrice[(bonhomme->localisation.y-1+nrow)%nrow][(bonhomme->localisation.x+1)%ncol].soignant_present != NULL)
    {
        if ((matrice[(bonhomme->localisation.y-1+nrow)%nrow][(bonhomme->localisation.x+1)%ncol].soignant_present->etat == ASYMPTO)
        || (matrice[(bonhomme->localisation.y-1+nrow)%nrow][(bonhomme->localisation.x+1)%ncol].soignant_present->etat == SAIN))//case au nord-est
        {
            cpt_soin++;

        }
    }
    else if (matrice[bonhomme->localisation.y][(bonhomme->localisation.x+1)%ncol].soignant_present != NULL)
    {
        if ((matrice[bonhomme->localisation.y][(bonhomme->localisation.x+1)%ncol].soignant_present->etat == ASYMPTO)
        || (matrice[bonhomme->localisation.y][(bonhomme->localisation.x+1)%ncol].soignant_present->etat == SAIN)) //case à l'est
        {
            cpt_soin ++;

        }
    }
    else if (matrice[(bonhomme->localisation.y+1)%nrow][(bonhomme->localisation.x+1)%ncol].soignant_present != NULL)
    {
        if ((matrice[(bonhomme->localisation.y+1)%nrow][(bonhomme->localisation.x+1)%ncol].soignant_present->etat == ASYMPTO)
        || (matrice[(bonhomme->localisation.y+1)%nrow][(bonhomme->localisation.x+1)%ncol].soignant_present->etat == SAIN)) //case au sud-est
        {
            cpt_soin ++;

        }
    }
    else if (matrice[(bonhomme->localisation.y-1+nrow)%nrow][bonhomme->localisation.x].soignant_present != NULL)
    {
        if ((matrice[(bonhomme->localisation.y-1+nrow)%nrow][bonhomme->localisation.x].soignant_present->etat == ASYMPTO)
        || (matrice[(bonhomme->localisation.y-1+nrow)%nrow][bonhomme->localisation.x].soignant_present->etat == SAIN)) //case au nord
        {
            cpt_soin ++;

        }
    }
    else if (matrice[(bonhomme->localisation.y+1)%nrow][bonhomme->localisation.x].soignant_present != NULL)
    {
        if ((matrice[(bonhomme->localisation.y+1)%nrow][bonhomme->localisation.x].soignant_present->etat == ASYMPTO)
        || (matrice[(bonhomme->localisation.y+1)%nrow][bonhomme->localisation.x].soignant_present->etat == SAIN)) //case au sud
        {
            cpt_soin ++;

        }
    }
    else if (matrice[(bonhomme->localisation.y-1+nrow)%nrow][(bonhomme->localisation.x-1+ncol)%ncol].soignant_present != NULL)
    {
        if ((matrice[bonhomme->localisation.y-1][(bonhomme->localisation.x-1+ncol)%ncol].soignant_present->etat == ASYMPTO)
        || (matrice[bonhomme->localisation.y-1][(bonhomme->localisation.x-1+ncol)%ncol].soignant_present->etat == SAIN))
        {
            cpt_soin ++;

        }
    }
    else if (matrice[bonhomme->localisation.y][(bonhomme->localisation.x-1+ncol)%ncol].soignant_present != NULL)
    {
        if ((matrice[bonhomme->localisation.y][(bonhomme->localisation.x-1+ncol)%ncol].soignant_present->etat == ASYMPTO)
        || (matrice[bonhomme->localisation.y][(bonhomme->localisation.x-1+ncol)%ncol].soignant_present->etat == SAIN))
        {
            cpt_soin ++;
        }
    }
    else if (matrice[(bonhomme->localisation.y+1)%nrow][(bonhomme->localisation.x-1+ncol)%ncol].soignant_present != NULL)
    {
        if ((matrice[(bonhomme->localisation.y+1)%nrow][(bonhomme->localisation.x-1+ncol)%ncol].soignant_present->etat == ASYMPTO)
        || (matrice[(bonhomme->localisation.y+1)%ncol][(bonhomme->localisation.x-1+ncol)%ncol].soignant_present->etat == SAIN)) //case au sud-ouest
        {
            cpt_soin ++;
        }
    }
    if (cpt_soin > 0)
    {
        bonhomme -> tmp_infection --;
        return 1;
    }
    else
        return 0;
}
