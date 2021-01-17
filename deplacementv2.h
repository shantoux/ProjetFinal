#ifndef DEPLACEMENT
#define DEPLACEMENT

  //------------------------------------||:INCLUDE & DEFINE:||-------------------------------------||

  #include "utilsv2.h"
  #define PROB_ELSE 25
  #define PROB_SAME 75
  #define PROB_DEPLACEMENT 70
  #define PROB_ARRET 30
  #define PROB_VIVRE 90
  #define PROB_MOURIR 10
  #define PROB_MALADE 33
  #define PROB_ASYMPTO 67

  //-------------------------------||:PROTOTYPES DES FONCTIONS:||----------------------------------||
  /*fonction principale de déplacement. Considère les déplacements et
  les conséquences de chacun en fonction de son état, de son statut et de son environnement*/
  void deplacement(Bonhomme lambda[], int *cpt_lambda, Bonhomme soignant[], int *cpt_soignant, Coordonnees virus[], int *cpt_virus, int nrow, int ncol, Case emplacement[nrow][ncol]);

  void quel_chemin(Bonhomme *bonhomme, int indice, int nrow, int ncol, Case emplacement[nrow][ncol]);//permet de choisir d'avancer (même direction ou autre) ou de s'arrêter. Gère les collisions.
  void colision(Dir direction_temp, int nrow, int ncol, Case emplacement[nrow][ncol], Bonhomme *bonhomme, int indice); //fonction appelée par la fonction [quel_chemin] dans un cas de collision. Change de direction ou s'arrête. (gère les collisions multiples).
  void contamination(Bonhomme bonhomme, int nrow, int ncol, Case matrice[nrow][ncol], Case **emplacement_choisi); //cas de dépot d'un virus du fait d'un asymptotique autour de sa case ou sur celle-ci.
  void infection(Bonhomme *bonhomme, int *cpt_infecte); //determine si un bonhomme devient asymptotique ou malade.
  void Changement_coordonnees(Bonhomme *entite, int indice, Dir nouvelle, int Long, int Larg);
  void liberation_case(Case *place);
  void nouvelle_case(Case *place, Bonhomme *entite, int indice);
  Dir pioche_nouvelle(Dir ancienne);
  int obstacle_bonhomme(Dir nouvelle, int Long, int Larg, Case matrice[Long][Larg], Bonhomme *entite, int indice);
  void zone_gradient(Bonhomme bonhomme, int nrow, int ncol, Case emplacement[nrow][ncol], bool etat);
  Dir repulsion(Bonhomme bonhomme[], int indice, int nrow, int ncol, Case emplacement[nrow][ncol]);
  Dir attraction(Bonhomme bonhomme[], int indice, int nrow, int ncol, Case emplacement[nrow][ncol]);
  bool soin(Bonhomme *bonhomme, int nrow, int ncol, Case matrice[nrow][ncol]);


  // On a un soigneur : 1. il n'est pas malade, 2. il analyse son environnement proche
  // Si, dans son environnement, gradient = 1, il se dirige dessus (si non-occupé)
  // Si, il se trouve sur une case 1, il se dirige sur une case 2 (si non-occupé)
  // Sinon : quel_chemin.

#endif
