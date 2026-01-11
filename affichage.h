/**
 * @file affichage.h
 * @brief Fonctions d'affichage (plateau, scores, bannière)
 */

#ifndef AFFICHAGE_H
#define AFFICHAGE_H

/* --- Affichage --- */
void afficher_banniere(void);
void afficher_plateau(Jeu *jeu, int pos1, int pos2);
void afficher_plateau_triche(Jeu *jeu);
void afficher_scores(Jeu *jeu, int modeSolitaire);
void afficher_fin_partie(Jeu *jeu, int modeSolitaire);

#endif /* AFFICHAGE_H */
